Contents:
1. Seed Classifier - local - Example of simple usage of LWTTN in C++
    - Requires LWTNN to be installed and available to cmake via `find_package(lwtnn)`
    
2. BrunelDev_v52r6 - implementation of Neural Network in PatLongLivedTracking algorithm
    - 2.1.Gaudi LWTNN/ - LWTNN modified to allow installation in Gaudi framework
    - 2.2 data/ - model directory
    - 2.3 Tf/PatAlgorithms/src/* - modified PatLongLivedTracking files
    
    **Running classifier with Gaudi requires environmental variable `"LOCALPARAMFILESROOT"` to be set to the data dir**  