# DownstreamTracking-SeedCLF

The repository contains code for my engineering thesis "Track classification using machine learning
techniques for LHCb experiment"

[Diploma thesis PDF](track-classification-artur-kucia.pdf)

Contents:
1. Seed Classifier Development - Python code and notebooks for building, training and evaluating the Neural Network model for T-seed classification. 
2. Seed Classifier Implementation - C++ code which implements the exported model inside the PatLongLivedTracking algorithm[1][2] using the LWTNN library[4]
3. Seed Classifier Performance Analysis - evaluation of Downstream Tracking efficiency with the new algorithm
4. data - input training files and ROOT -> Python converter
5. models - final model json files


[1]( http://cds.cern.ch/record/2240723?ln=en) PatLongLivedTracking

[2]( https://github.com/adendek/DownstreamTracking) Downstream Tracking study

[3]( https://twiki.cern.ch/twiki/bin/view/LHCb/LHCbTrackingStrategies#Downstream_tracking) LHCb Tracking 

[4]( https://github.com/lwtnn/lwtnn "LWTNN") LWTNN
