
from __future__ import division, print_function, absolute_import

import os
import time

import numpy as np
import tensorflow as tf
from sklearn.base import BaseEstimator, ClassifierMixin
from sklearn.metrics import accuracy_score
from tensorflow.contrib.layers import dropout
from tqdm import tqdm


import numpy
from collections import OrderedDict
from sklearn.base import ClassifierMixin, BaseEstimator, clone
from hep_ml.commonutils import to_pandas_dataframe, check_xyw, check_sample_weight, weighted_quantile


class DNNSoftmaxClf(BaseEstimator, ClassifierMixin):

    def __init__(
            self,
            input_size=11,
            hidden_layers=2,
            layer_size=10,
            output_classes=2,
            act_fun=tf.nn.elu,
            weights_initializer=tf.contrib.layers.variance_scaling_initializer(),
            keep_prob=1.0,
            reg=0.0,
            learning_rate=1e-3,
            optimizer=tf.train.AdamOptimizer,
            n_epochs=10,
            batch_size=100,
            logdir='models/',
            close_after_fit=False
    ):

        self.input_size = input_size
        self.hidden_layers = hidden_layers
        self.layer_size = layer_size
        self.output_classes = output_classes
        self.act_fun = act_fun
        self.weights_initializer = weights_initializer
        self.keep_prob = keep_prob
        self.reg = reg
        self.learning_rate = learning_rate
        self.optimizer = optimizer
        self.n_epochs = n_epochs
        self.batch_size = batch_size
        self.logdir = os.path.join(logdir)
        self.classes_ = ['background', 'signal']
        self.close_after_fit = close_after_fit

        self.saved_path = None
        self.model_name = None
        self.has_model = False
        self.session = None

    def _inference(self, inputs):

        with tf.name_scope("inputs"):
            is_training = tf.placeholder(tf.bool, shape=(), name='is_training')
            input_drop = dropout(inputs,  1.0, is_training=is_training)
        
        hidden_layer_dropped = input_drop

        for i in range(self.hidden_layers):
            hidden_layer = tf.layers.dense(
                    hidden_layer_dropped,
                    self.layer_size,
                    name='hidden_{}'.format(i),
                    kernel_initializer=self.weights_initializer
            )

            batch_normed = tf.layers.batch_normalization(
                    hidden_layer,
                    training=is_training,
                    momentum=0.9
            )

            activated = self.act_fun(batch_normed)
            hidden_layer_dropped = dropout(
                    activated,
                    self.keep_prob,
                    is_training=is_training
            )

        with tf.name_scope("outputs"):
            logits_unnormalized = tf.layers.dense(
                    hidden_layer_dropped,
                    self.output_classes,
                    name="outputs",
                    kernel_initializer=self.weights_initializer
            )

            logits = tf.layers.batch_normalization(
                    logits_unnormalized,
                    training=is_training,
                    momentum=0.9
            )

            extra_update_ops = tf.get_collection(tf.GraphKeys.UPDATE_OPS)
            
        return logits, extra_update_ops, is_training

    def fit(self, X, y=None):
        
        tf.reset_default_graph()
        
        self.model_name = str(time.ctime())
        model_dir = os.path.join(self.logdir, self.model_name)
        try:
            os.makedirs(model_dir)
        except OSError as error:
            print(error)
        
        def downstream_classifier_next_train_batch():
            batch_x = tf.constant(
                    np.array(X, dtype=np.float32),
                    dtype=tf.float32,
                    shape=(len(X), self.input_size)
            )

            batch_y = tf.constant(
                    np.array(y, dtype=np.int32),
                    dtype=tf.int32,
                    shape=[len(X)]
            )

            return [batch_x, batch_y]

        with tf.name_scope("train_input"):
            train_x, train_y = tf.train.shuffle_batch(
                downstream_classifier_next_train_batch(),
                enqueue_many=True,
                batch_size=self.batch_size,
                capacity=500,
                min_after_dequeue=50,
                allow_smaller_final_batch=False
            )

        with tf.variable_scope("DNN"):
            logits, extra_update_ops, is_training_var = self._inference(train_x)

        with tf.name_scope("accuracy"):
            correct_train = tf.nn.in_top_k(tf.cast(logits, tf.float32), train_y, 1)
            accuracy_train = tf.reduce_mean(tf.cast(correct_train, tf.float32))
            accuracy_train_summary = tf.summary.scalar('accuracy_train', accuracy_train)

        with tf.name_scope("Loss"):
            reg_losses = tf.get_collection(tf.GraphKeys.REGULARIZATION_LOSSES)

            cross_entropy_train = tf.nn.sparse_softmax_cross_entropy_with_logits(
                    labels=train_y,
                    logits=logits
            )

            base_loss_train = tf.reduce_mean(cross_entropy_train, name="xe_loss_train")
            base_loss_train_summary = tf.summary.scalar('base_loss_train', base_loss_train)

            loss_train = tf.add_n([base_loss_train] + reg_losses, name="loss")
            loss_train_summary = tf.summary.scalar('loss_train', loss_train)

        with tf.name_scope("train"):
            optimizer = self.optimizer(self.learning_rate)
            training_op = optimizer.minimize(loss_train)

        file_writer = tf.summary.FileWriter(model_dir + '/', tf.get_default_graph())
        n_iterations = int(np.ceil(len(X)/self.batch_size))

        global_step = 0

        init = tf.global_variables_initializer()
        saver = tf.train.Saver()
        coord = tf.train.Coordinator()
        self.session = tf.Session()
        self.has_model = True

        with self.session.as_default():
            init.run()
            summary_op = tf.summary.merge_all()
            threads = tf.train.start_queue_runners(sess=self.session, coord=coord)

            for epoch in tqdm(range(self.n_epochs)):
                for i in range(n_iterations):
                    self.session.run([extra_update_ops, training_op], feed_dict={is_training_var: True})
                    if i % 10 == 0:
                        summary_str = self.session.run(summary_op, feed_dict={is_training_var: False})
                        file_writer.add_summary(summary_str, global_step)

                    global_step += self.batch_size

                summary_str = self.session.run(summary_op, feed_dict={is_training_var: False})
                file_writer.add_summary(summary_str, global_step)

                if epoch % 10 == 0:
                    self.saved_path = saver.save(self.session, model_dir + '/')

            coord.request_stop()
            coord.join(threads)
            
            self.saved_path = saver.save(self.session, model_dir + '/')
            with open(os.path.join(model_dir, "parameters_info.txt"), 'w') as file:
                print(self.get_params(), file=file)
                
            file_writer.close()

        if self.close_after_fit:
            self.close_session()
    
    def restore(self, path):
        if self.has_model:
            print("Cannot overwrite existing model!")
            return
        else:
            tf.reset_default_graph()
            self.saved_path = path
            dummy_features = np.zeros((1, self.input_size), dtype=np.float32)

            with tf.variable_scope("DNN") as scope:
                _, __, ___ = self._inference(dummy_features)
            self.session = tf.Session()
            with self.session.as_default():
                new_saver = tf.train.Saver()
                new_saver.restore(self.session, self.saved_path)

            self.model_name = "_".join([time.ctime(), '(restored from: {})'.format(self.saved_path)])
            self.has_model = True
        
    def save(self, path):

        if self.has_model:
            model_dir = os.path.join(path, self.model_name)
            try:
                os.makedirs(model_dir)
            except OSError as error:
                print(error)

            with self.session.as_default():
                saver = tf.train.Saver()
                self.saved_path = saver.save(self.session, model_dir + '/')

                with open(os.path.join(model_dir, "parameters_info.txt"), 'w') as file:
                    print(self.get_params(), file=file)

                return self.saved_path
        else:
            print("No model to save!")
            return

    def predict(self, X):

        probabilities = self.predict_proba(X)
        return np.argmax(probabilities, axis=1)
    
    def predict_proba(self, X):

        data = X.copy()
        features = np.array(X, dtype=np.float32)

        if not self.has_model:
            new_saver = tf.train.Saver()
            new_saver.restore(self.session, self.saved_path)

        with self.session.as_default():
            with tf.variable_scope("DNN", reuse=True) as scope:
                logits, _, is_training_var = self._inference(features)
                predictions = tf.nn.softmax(logits)

            return predictions.eval(feed_dict={is_training_var: False})
    
    def score(self, X, y, sample_weight=None):

        pred = self.predict(X)
        return accuracy_score(y, np.array(pred, dtype=np.int64))

    def close_session(self):
        self.session.close()
        self.has_model = False


import keras

from keras.wrappers.scikit_learn import KerasClassifier
import keras
from sklearn.base import BaseEstimator

class KerasDNN(BaseEstimator, KerasClassifier):
    def __init__(self, input_shape, output_shape,
                 layers=3,
                 neurons=100,
                 activation='relu',
                 loss_metric='ce',
                 optimizer='adam',
                 batch_norm=True,
                 dropout=0.0,
                 metrics=['accuracy'],
                 last_layer_act='softmax',
                 kernel_initializer='normal',
                 **kwargs
                 ):
        self.input_shape = input_shape
        self.output_shape = output_shape
        self.layers = layers
        self.neurons = neurons
        self.activation = activation
        self.loss_metric = loss_metric
        self.optimizer = optimizer
        self.batch_norm = batch_norm
        self.dropout = dropout
        self.kernel_initializer = kernel_initializer
        self.metrics = metrics
        self.last_layer_act = last_layer_act

        super().__init__(**kwargs)


    def __call__(self):
        inp = keras.layers.Input(self.input_shape)

        layer = inp
        for i in range(self.layers):
            layer = keras.layers.Dense(self.neurons, activation=self.activation, kernel_initializer=self.kernel_initializer)(layer)
            if self.batch_norm:
                layer = keras.layers.BatchNormalization()(layer)
            if self.dropout > 0:
                layer = keras.layers.core.Dropout(self.dropout)(layer)

        layer = keras.layers.Dense(self.output_shape[-1], activation=self.last_layer_act, kernel_initializer=self.kernel_initializer)(layer)

        model = keras.models.Model(inputs=[inp], outputs=[layer])
        model.compile(optimizer=self.optimizer, loss=self.loss_metric,
                      metrics=self.metrics)
        self.model = model
        return model

    def predict_proba(self, x, **kwargs):
        return self.model.predict(x)

    def predict(self, x, **kwargs):
        predictions = self.predict_proba(x, **kwargs)
        return np.argmax(predictions, axis=1)

class KerasDNN2(BaseEstimator, KerasClassifier):
    def __init__(self, input_shape, output_shape,
                 layers=3,
                 neurons=100,
                 activation='relu',
                 loss_metric='ce',
                 optimizer='adam',
                 batch_norm=True,
                 dropout=0.0,
                 metrics=['accuracy'],
                 last_layer_act='softmax',
                 kernel_initializer='normal',
                 **kwargs
                 ):
        self.input_shape = input_shape
        self.output_shape = output_shape
        self.layers = layers
        self.neurons = neurons
        self.activation = activation
        self.loss_metric = loss_metric
        self.optimizer = optimizer
        self.batch_norm = batch_norm
        self.dropout = dropout
        self.kernel_initializer = kernel_initializer
        self.metrics = metrics
        self.last_layer_act = last_layer_act

        super().__init__(**kwargs)


    def __call__(self):
        inp = keras.layers.Input(self.input_shape)

        layer = inp
        if self.batch_norm:
            layer = keras.layers.BatchNormalization()(layer)
        for i in range(self.layers):
            layer = keras.layers.Dense(self.neurons, kernel_initializer=self.kernel_initializer)(layer)
            if self.batch_norm:
                layer = keras.layers.BatchNormalization()(layer)
            layer = keras.layers.Activation(self.activation)(layer)
            if self.dropout > 0:
                layer = keras.layers.core.Dropout(self.dropout)(layer)

        layer = keras.layers.Dense(self.output_shape[-1], activation=self.last_layer_act, kernel_initializer=self.kernel_initializer)(layer)

        model = keras.models.Model(inputs=[inp], outputs=[layer])
        model.compile(optimizer=self.optimizer, loss=self.loss_metric,
                      metrics=self.metrics)
        self.model = model
        return model

    def predict_proba(self, x, **kwargs):
        return self.model.predict(x)

    def predict(self, x, **kwargs):
        predictions = self.predict_proba(x, **kwargs)
        return np.argmax(predictions, axis=1)

class LookupClassifier(BaseEstimator, ClassifierMixin):
    def __init__(self, base_estimator, n_bins=16, max_cells=500000000, keep_trained_estimator=True):
        """
        LookupClassifier splits each of features into bins, trains a base_estimator to use this data.
        To predict class for new observation, results of base_estimator are kept for all possible combinations of bins,
        and saved together
        :param base_estimator: classifier used to build predictions
        :param n_bins:
            * int: how many bins to use for each axis
            * dict: feature_name -> int, specialize how many bins to use for each axis
            * dict: feature_name -> list of floats, set manually edges of bins
            By default, the (weighted) quantiles are used to compute bin edges.
        :type n_bins: int | dict
        :param int max_cells: raise error if lookup table will have more items.
        :param bool keep_trained_estimator: if True, trained estimator will be saved.
        See also: this idea is used inside LHCb triggers, see V. Gligorov, M. Williams, 'Bonsai BDT'
        Resulting formula is very simple and can be rewritten in other language or environment (C++, CUDA, etc).
        """
        self.base_estimator = base_estimator
        self.n_bins = n_bins
        self.max_cells = max_cells
        self.keep_trained_estimator = keep_trained_estimator

    def check_dimensions(self, bin_edges):
        cumulative_size = numpy.cumprod([len(bin_edge) + 1 for name, bin_edge in bin_edges.items()])
        if numpy.any(cumulative_size > self.max_cells):
            raise ValueError('the total size of lookup table exceeds {}, '
                             'reduce n_bins or number of features in use'.format(self.max_cells))

    def fit(self, X, y, sample_weight=None, **fit_params):
        """Train a classifier and collect predictions for all possible combinations.
        :param X: pandas.DataFrame or numpy.array with data of shape [n_samples, n_features]
        :param y: array with labels of shape [n_samples]
        :param sample_weight: None or array of shape [n_samples] with weights of events
        :return: self
        """
        self.classes_ = numpy.unique(y)
        X, y, normed_weights = check_xyw(X, y, sample_weight=sample_weight, classification=True)
        X = to_pandas_dataframe(X)
        normed_weights = check_sample_weight(y, sample_weight=normed_weights, normalize_by_class=True, normalize=True)

        self.bin_edges = self._compute_bin_edges(X, normed_weights=normed_weights)
        self.check_dimensions(self.bin_edges)

        n_parameter_combinations = numpy.prod([len(bin_edge) + 1 for name, bin_edge in self.bin_edges.items()])

        transformed_data = self.transform(X)
        # fit_params = {}
        if sample_weight is not None:
            fit_params['sample_weights'] = sample_weight

        self.base_estimator.fit(transformed_data, y, **fit_params)

        all_lookup_indices = numpy.arange(int(n_parameter_combinations))
        all_combinations = self.convert_lookup_index_to_bins(all_lookup_indices)
        self._lookup_table = self.base_estimator.predict_proba(all_combinations)


        return self

    def _compute_bin_edges(self, X, normed_weights):
        """
        Compute edges of bins, weighted quantiles are used,
        """
        bins_over_axis = OrderedDict()
        for column in X.columns:
            if isinstance(self.n_bins, int):
                bins_over_axis[column] = self.n_bins
            else:
                bins_over_axis[column] = self.n_bins[column]

        bin_edges = OrderedDict()
        for column, column_bins in bins_over_axis.items():
            if isinstance(column_bins, int):
                quantiles = numpy.linspace(0., 1., column_bins + 1)[1:-1]
                bin_edges[column] = weighted_quantile(X[column], quantiles=quantiles, sample_weight=normed_weights)
            else:
                bin_edges[column] = numpy.array(list(column_bins))

        return bin_edges

    def convert_bins_to_lookup_index(self, bins_indices):
        """
        :param bins_indices: numpy.array of shape [n_samples, n_columns], filled with indices of bins.
        :return: numpy.array of shape [n_samples] with corresponding index in lookup table
        """
        lookup_indices = numpy.zeros(len(bins_indices), dtype=int)
        bins_indices = numpy.array(bins_indices)
        assert bins_indices.shape[1] == len(self.bin_edges)
        for i, (column_name, bin_edges) in enumerate(self.bin_edges.items()):
            lookup_indices *= len(bin_edges) + 1
            lookup_indices += bins_indices[:, i]
        return lookup_indices

    def convert_lookup_index_to_bins(self, lookup_indices):
        """
        :param lookup_indices: array of shape [n_samples] with positions at lookup table
        :return: array of shape [n_samples, n_features] with indices of bins.
        """

        result = numpy.zeros([len(lookup_indices), len(self.bin_edges)], dtype='uint8')
        for i, (column_name, bin_edges) in list(enumerate(self.bin_edges.items()))[::-1]:
            n_columns = len(bin_edges) + 1
            result[:, i] = lookup_indices % n_columns
            lookup_indices = lookup_indices // n_columns

        return result

    def transform(self, X):
        """Convert data to bin indices.
        :param X: pandas.DataFrame or numpy.array with data
        :return: numpy.array, where each column is replaced with index of bin
        """
        X = to_pandas_dataframe(X)
        assert list(X.columns) == list(self.bin_edges.keys()), 'passed dataset with wrong columns'
        result = numpy.zeros(X.shape, dtype='uint8')
        for i, column in enumerate(X.columns):
            edges = self.bin_edges[column]
            result[:, i] = numpy.searchsorted(edges, X[column])

        return result

    def predict(self, X):
        """Predict class for each event
        :param X: pandas.DataFrame with data
        :return: array of shape [n_samples] with predicted class labels.
        """
        return self.classes_[numpy.argmax(self.predict_proba(X), axis=1)]

    def predict_proba(self, X):
        """ Predict probabilities for new observations
        :param X: pandas.DataFrame with data
        :return: probabilities, array of shape [n_samples, n_classes]
        """
        bins_indices = self.transform(X)
        lookup_indices = self.convert_bins_to_lookup_index(bins_indices)
        return self._lookup_table[lookup_indices]
