
from sklearn.pipeline import Pipeline

from sklearn.base import BaseEstimator, TransformerMixin

from sklearn.preprocessing import StandardScaler
import numpy as np


class addAngle(BaseEstimator, TransformerMixin):
    def __init__(self):
        self.scaler = StandardScaler()

    def fit(self, X, y=None):
        seed_angle = np.arctan(X.seed_y / X.seed_x)
        self.scaler.fit(seed_angle.values.reshape(-1, 1))
        return self

    def transform(self, X, y=None):
        data = X.copy()
        seed_angle = np.arctan(data.seed_y / data.seed_x)
        data['seed_angle'] = self.scaler.transform(np.array(seed_angle).reshape(-1, 1))
        return data


class addPR(BaseEstimator, TransformerMixin):
    def __init__(self):
        self.scaler = StandardScaler()
        pass

    def fit(self, X, y=None):
        pr = np.arctanh(X.seed_pt / X.seed_p)
        self.scaler.fit(pr.values.reshape(-1,1))
        return self

    def transform(self, X, y=None):
        data = X.copy()
        pr = np.arctanh(data.seed_pt / data.seed_p)
        data['seed_pr'] = self.scaler.transform(pr.values.reshape(-1,1))
        return data


class addR(BaseEstimator, TransformerMixin):
    def __init__(self):
        self.scaler = StandardScaler()
        pass

    def fit(self, X, y=None):
        r = np.sqrt(np.square(X['seed_x']) + np.square(X['seed_y']))
        self.scaler.fit(np.array(r).reshape(-1,1))
        return self

    def transform(self, X, y=None):
        data = X.copy()
        x = data['seed_x']
        y = data['seed_y']
        r = np.array(np.sqrt(np.square(x) + np.square(y)))
        data['seed_r'] = self.scaler.transform(r.reshape(-1,1))
        return data


class addZ(BaseEstimator, TransformerMixin):
    def __init__(self):
        self.scaler = StandardScaler()
        pass


    def fit(self, X, y=None):
        data_unnormalized = (5376.8 - 3895.12 * np.square(
                X['seed_ty']) + 309.877 * np.square(X['seed_tx']) + 89.901 / X[
                                 'seed_p'] - 4732) / (5870 - 4732)
        self.scaler.fit(np.array(data_unnormalized).reshape(-1,1))
        return self

    def transform(self, X, y=None):
        data = X.copy()
        data_unnormalized = (5376.8 - 3895.12 * np.square(
                data['seed_ty']) + 309.877 * np.square(data['seed_tx']) + 89.901 / data[
                                 'seed_p'] - 4732) / (5870 - 4732)
        data['seed_z_magnet'] = self.scaler.transform(np.array(data_unnormalized).reshape(-1,1))
        return data


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


class chi2PerDoFScaler(BaseEstimator, TransformerMixin):
    def __init__(self):
        self.scaler = StandardScaler()
        pass

    def fit(self, X, y=None):
        self.scaler.fit(np.sqrt(X['seed_chi2PerDoF']).values.reshape(-1, 1))
        return self

    def transform(self, X, y=None):
        data = X.copy()
        chi2 = data['seed_chi2PerDoF']
        chi2 = np.sqrt(chi2).values.reshape(-1, 1)
        data['seed_chi2PerDoF'] = self.scaler.transform(chi2)
        return data


class nbITScale(BaseEstimator, TransformerMixin):
    def __init__(self):
        self.scaler = StandardScaler()

    def fit(self, X, y=None):
        self.scaler.fit(X['seed_nbIT'].values.reshape(-1, 1))
        return self

    def transform(self, X, y=None):
        data = X.copy()
        nbIT = data['seed_nbIT']
        nbIT = nbIT.values.reshape(-1, 1)
        data['seed_nbIT'] = self.scaler.transform(nbIT)
        return data


class nLayersScale(BaseEstimator, TransformerMixin):
    def __init__(self):
        self.scaler = StandardScaler()

    def fit(self, X, y=None):
        self.scaler.fit(X['seed_nLayers'].values.reshape(-1, 1))
        return self

    def transform(self, X, y=None):
        data = X.copy()
        nLayers = data['seed_nLayers']
        nLayers = nLayers.values.reshape(-1, 1)
        data['seed_nLayers'] = self.scaler.transform(nLayers)
        return data


class nLHCbIDsScale(BaseEstimator, TransformerMixin):
    def __init__(self):
        self.scaler = StandardScaler()

    def fit(self, X, y=None):
        self.scaler.fit(X['seed_nLHCbIDs'].values.reshape(-1, 1))
        return self

    def transform(self, X, y=None):
        data = X.copy()
        nLHCbIDs = data['seed_nLHCbIDs']
        nLHCbIDs = nLHCbIDs.values.reshape(-1, 1)
        data['seed_nLHCbIDs'] = self.scaler.transform(nLHCbIDs)
        return data


class pScale(BaseEstimator, TransformerMixin):
    def __init__(self):
        self.scaler = StandardScaler()
        pass

    def fit(self, X, y=None):
        self.scaler.fit(np.log10(X['seed_p']).values.reshape(-1, 1))
        return self

    def transform(self, X, y=None):
        data = X.copy()
        p = np.log10(data['seed_p']).values.reshape(-1, 1)
        data['seed_p'] = self.scaler.transform(p)
        return data


class ptScale(BaseEstimator, TransformerMixin):
    def __init__(self):
        self.scaler = StandardScaler()
        pass

    def fit(self, X, y=None):
        self.scaler.fit(np.log10(X['seed_pt']).values.reshape(-1, 1))
        return self

    def transform(self, X, y=None):
        data = X.copy()
        pt = np.log10(data['seed_pt'].values.reshape(-1, 1))
        data['seed_pt'] = self.scaler.transform(pt)
        return data


class txScale(BaseEstimator, TransformerMixin):
    def __init__(self):
        self.scaler = StandardScaler()
        pass

    def fit(self, X, y=None):
        self.scaler.fit(X['seed_tx'].values.reshape(-1, 1))
        return self

    def transform(self, X, y=None):
        data = X.copy()
        tx = data['seed_tx'].values.reshape(-1, 1)
        data['seed_tx'] = self.scaler.transform(tx)
        return data


class tyScale(BaseEstimator, TransformerMixin):
    def __init__(self):
        self.scaler = StandardScaler()
        pass

    def fit(self, X, y=None):
        self.scaler.fit(X['seed_ty'].values.reshape(-1, 1))
        return self

    def transform(self, X, y=None):
        data = X.copy()
        ty = data['seed_ty'].values.reshape(-1, 1)
        data['seed_ty'] = self.scaler.transform(ty)
        return data


class xScale(BaseEstimator, TransformerMixin):
    def __init__(self):
        self.scaler = StandardScaler()
        pass

    def fit(self, X, y=None):
        self.scaler.fit(X['seed_x'].values.reshape(-1, 1))
        return self

    def transform(self, X, y=None):
        data = X.copy()
        x = data['seed_x'].values.reshape(-1, 1)
        data['seed_x'] = self.scaler.transform(x)
        return data


class yScale(BaseEstimator, TransformerMixin):
    def __init__(self):
        self.scaler = StandardScaler()
        pass

    def fit(self, X, y=None):
        self.scaler.fit(X['seed_y'].values.reshape(-1, 1))
        return self

    def transform(self, X, y=None):
        data = X.copy()
        y = data['seed_y'].values.reshape(-1, 1)
        data['seed_y'] = self.scaler.transform(y)
        return data

def data_pipeline():
    num_pipeline = Pipeline([
        # ('dropFalseMCParticles',dropFalseMCParticles()),
        # ('dropUnnecessaryFeatures',dropUnnecessaryFeatures()),
        ('angle', addAngle()),
        ('pseudo rapidity', addPR()),
        ('z magnet',addZ()),
        ('radius', addR()),
        ('y',yScale()),
        ('x',xScale()),
        ('ty',tyScale()),
        ('tx',txScale()),
        ('nbIT',nbITScale()),
        ('nLayers',nLayersScale()),
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

from sklearn.preprocessing import QuantileTransformer

class txScale_abs(BaseEstimator, TransformerMixin):
    def __init__(self):
        self.scaler = StandardScaler()
        pass

    def fit(self, X, y=None):
        self.scaler.fit(X['seed_tx'].values.reshape(-1, 1))
        return self

    def transform(self, X, y=None):
        data = X.copy()
        tx = data['seed_tx'].values.reshape(-1, 1)
        data['seed_tx'] = np.abs(self.scaler.transform(tx))
        return data


class tyScale_abs(BaseEstimator, TransformerMixin):
    def __init__(self):
        self.scaler = StandardScaler()
        pass

    def fit(self, X, y=None):
        self.scaler.fit(X['seed_ty'].values.reshape(-1, 1))
        return self

    def transform(self, X, y=None):
        data = X.copy()
        ty = data['seed_ty'].values.reshape(-1, 1)
        data['seed_ty'] = np.abs(self.scaler.transform(ty))
        return data


class xScale_abs(BaseEstimator, TransformerMixin):
    def __init__(self):
        self.scaler = StandardScaler()
        pass

    def fit(self, X, y=None):
        self.scaler.fit(X['seed_x'].values.reshape(-1, 1))
        return self

    def transform(self, X, y=None):
        data = X.copy()
        x = data['seed_x'].values.reshape(-1, 1)
        data['seed_x'] = np.abs(self.scaler.transform(x))
        return data


class yScale_abs(BaseEstimator, TransformerMixin):
    def __init__(self):
        self.scaler = StandardScaler()
        pass

    def fit(self, X, y=None):
        self.scaler.fit(X['seed_y'].values.reshape(-1, 1))
        return self

    def transform(self, X, y=None):
        data = X.copy()
        y = data['seed_y'].values.reshape(-1, 1)
        data['seed_y'] = np.abs(self.scaler.transform(y))
        return data

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
        ('y',yScale()),
        ('x',xScale()),
        ('ty',tyScale()),
        ('tx',txScale()),
        ('pt', ptScale()),
        ('p', pScale()),
        ('chi2PerDoF', chi2PerDoFScaler()),
    ])
    return num_pipeline