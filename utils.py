import os

import tensorflow as tf
import keras.backend as K


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

def test_model(model, X, y):
    pass