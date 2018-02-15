Contents:
1. Seed Classifier - local - Example of simple usage of LWTTN in C++
    - Requires LWTNN to be installed and available to cmake via `find_package(lwtnn)`
    - Run with:
    ```
    cd build
    cmake ..
    cd ../bin
    ./Classifier_in_cpp
    ```
    
2. BrunelDev_v52r6 - implementation of Neural Network in PatLongLivedTracking algorithm
    - 2.1.Gaudi LWTNN/ - LWTNN modified to allow installation in Gaudi framework
    - 2.2 data/ - model directory
    - 2.3 Tf/PatAlgorithms/src/* - modified PatLongLivedTracking files
    
    **Running classifier with Gaudi requires environmental variable `"LOCALPARAMFILESROOT"` to be set to the data dir**
    
    Installation:  
    1. Set up Brunel
    ```
    lb-dev Brunel v52r6
    cd BrunelDev_v52r6/
    git lb-use Rec
    git lb-checkout Rec/master Tf/PatAlgorithms 
    git lb-checkout Rec/master Tf/PatKernel 
    git lb-checkout Rec/master Tr/TrackMCTools 
    ```
    2. Merge the contents of `BrunelDev_v52r6` from this repository, with the `lb-dev` version.
    3. Clone LWTNN
    `git clone https://github.com/lwtnn/lwtnn.git`
    4. Replace `CMakeLists.txt` from LWTNN with the file `Gaudi_LWTNN/CMakeLists.txt` and move all LWTNN source files to `lwtnn/src/` directory (see `CMakeLists.txt` for the full list of files).
    
    5. `cd ../` and `make -j`
    
    You can now use the Brunel framework with new Downstream Tracking algorithm
