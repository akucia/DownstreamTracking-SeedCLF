
from sklearn.pipeline import Pipeline
from data_utils import *
def data_pipeline():
    num_pipeline = Pipeline([
        ('dropFalseMCParticles',dropFalseMCParticles()),
        ('dropUnnecessaryFeatures',dropUnnecessaryFeatures()),
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
        ('nLHCbIDs',nLHCbIDsScale()),    
        ('pt', ptScale()),
        ('p', pScale()),
        ('chi2PerDoF', chi2PerDoFScaler()),            
    ])
    return num_pipeline