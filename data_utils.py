from sklearn.base import BaseEstimator, TransformerMixin
import numpy as np

class addAngle(BaseEstimator, TransformerMixin):
    def __init__(self):
        pass
    def fit(self, X, y=None):
        return self
    def transform(self, X, y=None):
        data = X.copy()
        data['seed_angle'] = np.arctan(data.seed_y/data.seed_x) 
        data['seed_angle'] /= np.std(data['seed_angle'])
        return data
        
        
class addPR(BaseEstimator, TransformerMixin):
    def __init__(self):
        pass
    def fit(self, X, y=None):
        return self
    def transform(self, X, y=None):
        data = X.copy()
        data['seed_pr'] = np.arctanh(data.seed_pt/ data.seed_p) 
        data['seed_pr'] -= np.mean(data['seed_pr'])
        data['seed_pr'] /= np.std(data['seed_pr'])
        return data
        

class addR(BaseEstimator, TransformerMixin):
    def __init__(self):
        pass
    def fit(self, X, y=None):
        return self
    def transform(self, X, y=None):
        data = X.copy()
        x = data['seed_x']
        y = data['seed_y']
        data['seed_r'] = np.sqrt(np.square(x) + np.square(y))
        data['seed_r'] -= np.mean(data['seed_r'] )
        data['seed_r'] /= np.std(data['seed_r'])
        return data
        
        
class addZ(BaseEstimator, TransformerMixin):
    def __init__(self):
        pass
    def fit(self, X, y=None):
        return self
    def transform(self, X, y=None):
        data = X.copy()
        data_unnormalized = (5376.8 - 3895.12* np.square(data['seed_ty']) + 309.877 * np.square(data['seed_tx']) + 89.901/data['seed_p']-4732)/(5870-4732)
        data['seed_z_magnet'] = (data_unnormalized - np.mean(data_unnormalized))
        return data
        
        
class chi2PerDoFScaler(BaseEstimator, TransformerMixin):
    def __init__(self):
        pass
    def fit(self, X, y=None):
        return self
    def transform(self, X, y=None):
        data = X.copy()
        chi2 = data['seed_chi2PerDoF']
        chi2 = np.sqrt(chi2)
        chi2 -= np.mean(chi2)
        chi2 /= np.std(chi2)
        data['seed_chi2PerDoF'] = chi2
        return data
        
        
class dropFalseMCParticles(BaseEstimator, TransformerMixin):
    def __init__(self):
        pass
    def fit(self, X, y=None):
        return self
    def transform(self, X, y=None):
        data = X.copy()
        data = data.drop(data[~data['has_MCParticle']].index, axis=0)
        return data  
        
        
class dropUnnecessaryFeatures(BaseEstimator, TransformerMixin):
    def __init__(self):
        pass
    def fit(self, X, y=None):
        return self
    def transform(self, X, y=None):
        data = X.copy()
        data = data.drop(['has_MCParticle','has_MCParticle_not_electron', 'is_downstream_reconstructible_not_electron', 'is_true_seed'], axis=1)
        return data 
        
        
class nbITScale(BaseEstimator, TransformerMixin):
    def __init__(self):
        pass
    def fit(self, X, y=None):
        return self
    def transform(self, X, y=None):
        data = X.copy()
        return data
        
        
class nLayersScale(BaseEstimator, TransformerMixin):
    def __init__(self):
        pass
    def fit(self, X, y=None):
        return self
    def transform(self, X, y=None):
        data = X.copy()
        return data
        
        
class nLHCbIDsScale(BaseEstimator, TransformerMixin):
    def __init__(self):
        pass
    def fit(self, X, y=None):
        return self
    def transform(self, X, y=None):
        data = X.copy()
        return data
        
        
class pScale(BaseEstimator, TransformerMixin):
    def __init__(self):
        pass
    def fit(self, X, y=None):
        return self
    def transform(self, X, y=None):
        data = X.copy()
        p = np.log10(data['seed_p'])
        p -= np.mean(p)
#         p /= np.std(p)
        data['seed_p'] = p
        return data
        
        
class ptScale(BaseEstimator, TransformerMixin):
    def __init__(self):
        pass
    def fit(self, X, y=None):
        return self
    def transform(self, X, y=None):
        data = X.copy()
        pt = np.log10(data['seed_pt'])
        pt -= np.mean(pt)
        pt /= np.std(pt)
        data['seed_pt'] = pt
        return data
        
        
class txScale(BaseEstimator, TransformerMixin):
    def __init__(self):
        pass
    def fit(self, X, y=None):
        return self
    def transform(self, X, y=None):
        data = X.copy()
        tx = data['seed_tx']
        tx -= np.mean(tx)
        tx /= np.std(tx)
        data['seed_tx'] = tx
        return data
        

class tyScale(BaseEstimator, TransformerMixin):
    def __init__(self):
        pass
    def fit(self, X, y=None):
        return self
    def transform(self, X, y=None):
        data = X.copy()
        ty = data['seed_ty']
        ty -= np.mean(ty)
        ty /= np.std(ty)
        data['seed_ty'] = ty
        return data
        
        
class xScale(BaseEstimator, TransformerMixin):
    def __init__(self):
        pass
    def fit(self, X, y=None):
        return self
    def transform(self, X, y=None):
        data = X.copy()
        x = data['seed_x']
        x -= np.mean(x)
        x /= np.std(x)
        data['seed_x'] = x
        return data
        
        
class yScale(BaseEstimator, TransformerMixin):
    def __init__(self):
        pass
    def fit(self, X, y=None):
        return self
    def transform(self, X, y=None):
        data = X.copy()
        y = data['seed_y']
        y -= np.mean(y)
        y /= np.std(y)
        data['seed_y'] = y
        return data
        
