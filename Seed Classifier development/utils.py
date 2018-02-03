import os

import tensorflow as tf
import keras.backend as K

from sklearn.metrics import roc_curve
from sklearn.metrics import roc_auc_score

import numpy as np
import cufflinks as cf
import plotly.offline as py
py.init_notebook_mode()
cf.go_offline()
import plotly.graph_objs as go

import matplotlib.pyplot as plt

from sklearn.metrics import confusion_matrix

def export_binary_clf_as_SavedModel(keras_model, export_dir):
    K.set_learning_phase(0)

    inputs = tf.placeholder(tf.float32, shape=keras_model.layers[0].output.shape)
    predictions = keras_model(inputs)
    scores = tf.nn.softmax(predictions)
    classes = tf.argmax(predictions, axis=1)

    classify_signature = tf.saved_model.signature_def_utils.predict_signature_def(
            {'inputs': inputs}, {'classes': classes}
    )

    score_signature = tf.saved_model.signature_def_utils.predict_signature_def(
            {'inputs': inputs}, {'scores': scores}
    )
    if tf.saved_model.signature_def_utils.is_valid_signature(
            classify_signature) and tf.saved_model.signature_def_utils.is_valid_signature(
            score_signature):
        signature_def_map = {
            "classify"           :
                classify_signature,
            'predict_probability':
                score_signature
        }

    builder = tf.saved_model.builder.SavedModelBuilder(
        os.path.join(export_dir, 'saved_model'))

    tag_constants = [tf.saved_model.tag_constants.SERVING]

    builder.add_meta_graph_and_variables(
            K.get_session(),
            tag_constants,
            signature_def_map=signature_def_map,
            assets_collection=tf.get_collection(tf.GraphKeys.ASSET_FILEPATHS),
    )
    builder.save()

import itertools

def plot_confusion_matrix(cm, classes,
                          normalize=False,
                          title='Confusion matrix',
                          cmap=plt.cm.Blues):
    if normalize:
        cm = cm.astype('float') / cm.sum(axis=1)[:, np.newaxis]
    plt.imshow(cm, interpolation='nearest', cmap=cmap)
    plt.title(title)
    plt.colorbar()
    tick_marks = np.arange(len(classes))
    plt.xticks(tick_marks, classes, rotation=45)
    plt.yticks(tick_marks, classes)

    thresh = cm.max() / 2.
    fmt = '.4f' if normalize else 'd'
    for i, j in itertools.product(range(cm.shape[0]), range(cm.shape[1])):
        plt.text(j, i, format(cm[i, j], fmt),
                 horizontalalignment="center",
                 color="white" if cm[i, j] > thresh else "black")

    plt.tight_layout()
    plt.ylabel('True label')
    plt.xlabel('Predicted label')


def plot_true_positives_and_negatives(
        y_true,
        probabilities,
        normalize=False,
        step=0.1,
        title='True positives and true negatives vs threshold', ):

    thresholds = np.arange(0.0, 1.0, step)
    true_positives_rate = np.empty(thresholds.shape)
    true_negatives_rate = np.empty(thresholds.shape)
    for i, threshold in enumerate(thresholds):
        classified_examples = np.array(
            probabilities[:, 1] > threshold, dtype=int)
        cm = confusion_matrix(y_true, classified_examples)
        if normalize:
            cm = cm.astype('float') / cm.sum(axis=1)[:, np.newaxis]
        true_positives_rate[i] = cm[1, 1]
        true_negatives_rate[i] = cm[0, 0]

    plots = [
        go.Scatter(x=thresholds, y=true_positives_rate, name='true positives'),
        go.Scatter(x=thresholds, y=true_negatives_rate, name='true negatives'),
    ]
    layout = go.Layout(
        title=title,
        xaxis=dict(title='threshold'),
    )
    fig = go.Figure(data=plots, layout=layout)

    py.iplot(fig)

def plot_true_positives_and_negatives2(
        y_true,
        probabilities,
        normalize=False,
        step=0.1,
        title='True positives and true negatives vs threshold', ):

    thresholds = np.arange(0.0, 1.0, step)
    true_positives_rate = np.empty(thresholds.shape)
    true_negatives_rate = np.empty(thresholds.shape)
    for i, threshold in enumerate(thresholds):
        classified_examples = np.array(
            probabilities[:, 0] > threshold, dtype=int)
        cm = confusion_matrix(y_true, classified_examples)
        if normalize:
            cm = cm.astype('float') / cm.sum(axis=1)[:, np.newaxis]
        true_positives_rate[i] = cm[1, 1]
        true_negatives_rate[i] = cm[0, 0]

    plots = [
        go.Scatter(x=thresholds, y=true_positives_rate, name='true positives'),
        go.Scatter(x=thresholds, y=true_negatives_rate, name='true negatives'),
    ]
    layout = go.Layout(
        title=title,
        xaxis=dict(title='threshold'),
    )
    fig = go.Figure(data=plots, layout=layout)

    py.iplot(fig)


def plot_roc_curve(fpr, tpr, label=None):
    plt.plot(fpr, tpr, linewidth=2, label=label)
    plt.plot([0, 1], [0, 1], 'k--')
    plt.axis([0, 1, 0, 1])
    plt.xlabel('False Positive Rate')
    plt.ylabel('True Positive Rate')