""" Contains pre processing pipeline - feature transformations and creation"""
from abc import ABC, abstractmethod
from typing import List

import numpy as np
import pandas as pd
from sklearn.base import BaseEstimator, TransformerMixin
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import StandardScaler


class FeatureTransform(BaseEstimator, TransformerMixin, ABC):
    _scaler = StandardScaler()

    @staticmethod
    @abstractmethod
    def _transformation(X: pd.DataFrame) -> np.ndarray:
        pass

    @property
    @abstractmethod
    def _input_features(self) -> List[str]:
        pass

    @property
    @abstractmethod
    def _output_feature(self) -> str:
        pass

    def _apply_transformaton(self, X: pd.DataFrame) -> pd.DataFrame:
        if self._transformation is not None:
            data = self._transformation(X[self._input_features].copy())
        else:
            data = X[self._input_features].copy()
        return data.values.reshape(-1, 1)

    def fit(self, X: pd.DataFrame) -> 'FeatureTransform':
        data = self._apply_transformaton(X)
        self._scaler.fit(data)
        return self

    def transform(self, X: pd.DataFrame) -> pd.DataFrame:
        data = self._apply_transformaton(X)
        X[self._output_feature] = self._scaler.transform(data)
        return X


class addAngle(FeatureTransform):
    _input_features = ['seed_y', 'seed_x']
    _output_feature = 'seed_angle'

    @staticmethod
    def _transformation(X: pd.DataFrame):
        return np.arctan(X['seed_y'] / X['seed_x'])


class addPR(FeatureTransform):
    _input_features = ['seed_pt', 'seed_p']
    _output_feature = 'seed_pr'

    @staticmethod
    def _transformation(X: pd.DataFrame):
        return np.arctanh(X.seed_pt / X.seed_p)


class addR(FeatureTransform):
    _input_features = ['seed_x', 'seed_y']
    _output_feature = 'seed_r'

    @staticmethod
    def _transformation(X: pd.DataFrame):
        return np.sqrt(np.square(X['seed_x']) + np.square(X['seed_y']))


class addZ(FeatureTransform):
    _input_features = ['seed_tx', 'seed_ty', 'seed_p']
    _output_feature = 'seed_z_magnet'

    @staticmethod
    def _transformation(X: pd.DataFrame):
        return (
                   5376.8 - 3895.12 * np.square(X['seed_ty'])
                   + 309.877 * np.square(X['seed_tx'])
                   + 89.901 / X['seed_p'] - 4732
               ) / (5870 - 4732)


class dropFalseMCParticles(BaseEstimator, TransformerMixin):
    def __init__(self):
        pass

    def fit(self, X, y=None):
        return self

    def transform(self, X, y=None):
        data = X.copy()
        data = data[data['has_MCParticle'] > 0.5]
        del data['has_MCParticle']
        return data


class dropUnnecessaryFeatures(BaseEstimator, TransformerMixin):
    def __init__(self):
        pass

    def fit(self, X, y=None):
        return self

    def transform(self, X, y=None):
        data = X.copy()
        data = data.drop(['has_MCParticle', 'has_MCParticle_not_electron',
                          'is_downstream_reconstructible_not_electron', 'is_true_seed'],
                         axis=1)
        return data


class chi2PerDoFScaler(FeatureTransform):
    _input_features = ['seed_chi2PerDoF']
    _output_feature = 'seed_chi2PerDoF'

    @staticmethod
    def _transformation(X: pd.DataFrame):
        return np.sqrt(X['seed_chi2PerDoF'])


class nbITScale(FeatureTransform):
    transformation = None
    _input_features = ['seed_nbIT']
    _output_feature = 'seed_nbIT'


class nLayersScale(FeatureTransform):
    transformation = None
    _input_features = ['seed_nLayers']
    _output_feature = 'seed_nLayers'


class nLHCbIDsScale(FeatureTransform):
    transformation = None
    _input_features = ['seed_nLHCbIDs']
    _output_feature = 'seed_nLHCbIDs'


class pScale(FeatureTransform):
    _input_features = ['seed_p']
    _output_feature = 'seed_p'

    @staticmethod
    def _transformation(X: pd.DataFrame):
        return np.log10(X['seed_p'])


class ptScale(FeatureTransform):
    _input_features = ['seed_pt']
    _output_feature = 'seed_pt'

    @staticmethod
    def _transformation(X: pd.DataFrame):
        return np.log10(X['seed_pt'])


class txScale(FeatureTransform):
    _transformation = None
    _input_features = ['seed_tx']
    _output_feature = 'seed_tx'


class tyScale(FeatureTransform):
    _transformation = None
    _input_features = ['seed_ty']
    _output_feature = 'seed_ty'


class xScale(FeatureTransform):
    _transformation = None
    _input_features = ['seed_x']
    _output_feature = 'seed_x'


class yScale(FeatureTransform):
    _transformation = None
    _input_features = ['seed_y']
    _output_feature = 'seed_y'


def data_pipeline():
    num_pipeline = Pipeline([
        # ('dropFalseMCParticles',dropFalseMCParticles()),
        # ('dropUnnecessaryFeatures',dropUnnecessaryFeatures()),
        ('angle', addAngle()),
        ('pseudo rapidity', addPR()),
        ('z magnet', addZ()),
        ('radius', addR()),
        ('y', yScale()),
        ('x', xScale()),
        ('ty', tyScale()),
        ('tx', txScale()),
        ('nbIT', nbITScale()),
        ('nLayers', nLayersScale()),
        ('nLHCbIDs', nLHCbIDsScale()),
        ('pt', ptScale()),
        ('p', pScale()),
        ('chi2PerDoF', chi2PerDoFScaler()),
    ])
    return num_pipeline


def data_pipeline_limited():
    num_pipeline = Pipeline([
        ('dropFalseMCParticles', dropFalseMCParticles()),
        # ('dropUnnecessaryFeatures',dropUnnecessaryFeatures()),
    ])
    return num_pipeline


class txScale_abs(txScale):
    @staticmethod
    def _transformation(X: pd.DataFrame):
        return np.abs(X)


class tyScale_abs(txScale):
    @staticmethod
    def _transformation(X: pd.DataFrame):
        return np.abs(X)


class xScale_abs(txScale):
    @staticmethod
    def _transformation(X: pd.DataFrame):
        return np.abs(X)


class yScale_abs(txScale):
    @staticmethod
    def _transformation(X: pd.DataFrame):
        return np.abs(X)


class plog(BaseEstimator, TransformerMixin):
    def __init__(self):
        pass

    def fit(self, X, y=None):
        return self

    def transform(self, X, y=None):
        data = X.copy()
        p = np.log10(data['seed_p']).values.reshape(-1, 1)
        data['seed_p'] = p
        return data


class ptlog(BaseEstimator, TransformerMixin):
    def __init__(self):
        pass

    def fit(self, X, y=None):
        return self

    def transform(self, X, y=None):
        data = X.copy()
        p = np.log10(data['seed_pt']).values.reshape(-1, 1)
        data['seed_pt'] = p
        return data


class chi2sqrt(BaseEstimator, TransformerMixin):
    def __init__(self):
        self.scaler = StandardScaler()
        pass

    def fit(self, X, y=None):
        return self

    def transform(self, X, y=None):
        data = X.copy()
        chi2 = data['seed_chi2PerDoF']
        chi2 = np.sqrt(chi2).values.reshape(-1, 1)
        data['seed_chi2PerDoF'] = chi2
        return data


def data_pipeline_basic():
    num_pipeline = Pipeline([
        # ('dropFalseMCParticles', dropFalseMCParticles()),
        # ('dropUnnecessaryFeatures', dropUnnecessaryFeatures()),
        # ('pseudo rapidity', addPR()),
        # ('radius', addR()),
        ('ptlog', ptlog()),
        ('plog', plog()),
        ('x_abs', xScale_abs()),
        ('y_abs', yScale_abs()),
        ('tx_abs', txScale_abs()),
        ('ty_abs', tyScale_abs()),
        ('chi2sqrt', chi2sqrt()),
    ])
    return num_pipeline


def data_pipeline_basic_3():
    num_pipeline = Pipeline([
        ('pseudo rapidity', addPR()),
        ('radius', addR()),
        ('ptlog', ptlog()),
        ('plog', plog()),
        ('angle', addAngle()),
        ('chi2sqrt', chi2sqrt()),
    ])
    return num_pipeline


def data_pipeline_basic_2():
    num_pipeline = Pipeline([
        ('pt', ptScale()),
        ('p', pScale()),
        ('y', yScale()),
        ('x', xScale()),
        ('ty', tyScale()),
        ('tx', txScale()),
        ('chi2PerDoF', chi2PerDoFScaler()),
    ])
    return num_pipeline


def data_pipeline_3():
    num_pipeline = Pipeline([
        ('angle', addAngle()),
        ('pseudo rapidity', addPR()),
        ('radius', addR()),
        ('y', yScale()),
        ('x', xScale()),
        ('ty', tyScale()),
        ('tx', txScale()),
        ('pt', ptScale()),
        ('p', pScale()),
        ('chi2PerDoF', chi2PerDoFScaler()),
    ])
    return num_pipeline


def data_pipeline_all_labels():
    num_pipeline = Pipeline([
        ('angle', addAngle()),
        ('pseudo rapidity', addPR()),
        # ('z magnet',addZ()),
        ('radius', addR()),
        ('y', yScale()),
        ('x', xScale()),
        ('ty', tyScale()),
        ('tx', txScale()),
        ('pt', ptScale()),
        ('p', pScale()),
        ('chi2PerDoF', chi2PerDoFScaler()),
    ])
    return num_pipeline
