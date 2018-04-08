# DownstreamTracking-SeedCLF

The repository contains code for my engineering thesis "Track classification using machine learning
techniques for LHCb experiment"

[Diploma thesis PDF](track-classification-artur-kucia.pdf)

Contents:
1. Seed Classifier Development - Python code and notebooks for building, training and evaluating the Neural Network model for T-seed classification. 
2. Seed Classifier Implementation - C++ code which implements the exported model inside the PatLongLivedTracking algorithm[1][2] using the LWTNN library[3]
3. Seed Classifier Performance Analysis - evaluation of Downstream Tracking efficiency with the new algorithm
4. data - input training files and ROOT -> Python converter
5. models - final model json files

Read more:

1.[PatLongLivedTracking](http://cds.cern.ch/record/2240723?ln=en)

2.[Downstream Tracking study](https://github.com/adendek/DownstreamTracking) 

3.[LWTNN](https://github.com/lwtnn/lwtnn "LWTNN") 

4.[LHCb Tracking ](https://twiki.cern.ch/twiki/bin/view/LHCb/LHCbTrackingStrategies#Downstream_tracking)

