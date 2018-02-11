# DownstreamTracking-SeedCLF

The repository contains code for my engineering thesis "Track classification using machine learning
techniques for LHCb experiment"

[Diploma thesis PDf](track-classification-artur-kucia.pdf)

Contents:
1. [Seed Classifier Development](Seed Classifier development/) - Python code and notebooks for building, training and evaluating the Neural Network model for T-seed classification. 
2. [Seed Classifier Implementation](Seed Classifier implementation/) - C++ code which implements the exported model inside the PatLongLivedTracking algorithm[1][2] using the LWTNN library[4] 
3. [Seed Classifier Performance Analysis](Seed Classifier performance analysis/) - evaluation of Downstream Tracking efficiency with the new algorithm
4. [data](data/) - input training files and ROOT -> Python converter
5. [models](models/) - final model json files

****

[1]: http://cds.cern.ch/record/2240723?ln=en
[2]: https://github.com/adendek/DownstreamTracking
[3]: https://twiki.cern.ch/twiki/bin/view/LHCb/LHCbTrackingStrategies#Downstream_tracking
[4]: lwtnn/lwtnn
