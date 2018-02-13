import os
import time

import keras
import numpy as np
import tensorflow as tf
from keras.wrappers.scikit_learn import KerasClassifier
from sklearn.base import ClassifierMixin, BaseEstimator
from sklearn.metrics import accuracy_score
from tensorflow.contrib.layers import dropout
from tqdm import tqdm


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
            input_drop = dropout(inputs, 1.0, is_training=is_training)

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
            base_loss_train_summary = tf.summary.scalar('base_loss_train',
                                                        base_loss_train)

            loss_train = tf.add_n([base_loss_train] + reg_losses, name="loss")
            loss_train_summary = tf.summary.scalar('loss_train', loss_train)

        with tf.name_scope("train"):
            optimizer = self.optimizer(self.learning_rate)
            training_op = optimizer.minimize(loss_train)

        file_writer = tf.summary.FileWriter(model_dir + '/', tf.get_default_graph())
        n_iterations = int(np.ceil(len(X) / self.batch_size))

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
                    self.session.run([extra_update_ops, training_op],
                                     feed_dict={is_training_var: True})
                    if i % 10 == 0:
                        summary_str = self.session.run(summary_op,
                                                       feed_dict={is_training_var: False})
                        file_writer.add_summary(summary_str, global_step)

                    global_step += self.batch_size

                summary_str = self.session.run(summary_op,
                                               feed_dict={is_training_var: False})
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

            self.model_name = "_".join(
                [time.ctime(), '(restored from: {})'.format(self.saved_path)]
            )
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

                with open(os.path.join(model_dir, "parameters_info.txt"), 'w') \
                    as file:
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
            layer = keras.layers.Dense(
                self.neurons,
                activation=self.activation,
                kernel_initializer=self.kernel_initializer
            )(layer)
            if self.batch_norm:
                layer = keras.layers.BatchNormalization()(layer)
            if self.dropout > 0:
                layer = keras.layers.core.Dropout(self.dropout)(layer)

        layer = keras.layers.Dense(
            self.output_shape[-1],
            activation=self.last_layer_act,
            kernel_initializer=self.kernel_initializer
        )(layer)

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
            layer = keras.layers.Dense(
                self.neurons,
                kernel_initializer=self.kernel_initializer
            )(layer)
            if self.batch_norm:
                layer = keras.layers.BatchNormalization()(layer)
            layer = keras.layers.Activation(self.activation)(layer)
            if self.dropout > 0:
                layer = keras.layers.core.Dropout(self.dropout)(layer)

        layer = keras.layers.Dense(
            self.output_shape[-1],
            activation=self.last_layer_act,
            kernel_initializer=self.kernel_initializer
        )(layer)

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
