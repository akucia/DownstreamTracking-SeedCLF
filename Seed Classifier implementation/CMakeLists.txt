cmake_minimum_required(VERSION 3.8)
project(Classifier_in_cpp)

set(CMAKE_CXX_STANDARD 14)

find_package(lwtnn)

#set (CMAKE_CXX_FLAGS "-Wl,--allow-multiple-definition -Wl,--whole-archive")

set(SOURCE_FILES
        src/myconstants.h
        src/TFBinaryClassifier.cpp src/TFBinaryClassifier.h
#        src/PatNeuralNetworkTupledSeedClassifier.cpp src/PatNeuralNetworkTupledSeedClassifier.h
#        src/PatBBDTSeedClassifier.cpp src/PatBBDTSeedClassifier.h
        main.cpp
        src/LWTNNClassifier.cpp src/LWTNNClassifier.h
        )



set(EXECUTABLE Classifier_in_cpp)


add_executable(${EXECUTABLE} ${SOURCE_FILES})


set(TF_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tensorflow_cc/tensorflow_include)
set(TF_PYTHON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tensorflow_cc/tensorflow_python_include)


include_directories(
        ${TF_INCLUDE_DIR}/tensorflow
        ${TF_INCLUDE_DIR}/tensorflow/bazel-genfiles
        ${TF_INCLUDE_DIR}/tensorflow/tensorflow/contrib/makefile/downloads
        ${TF_INCLUDE_DIR}/tensorflow/tensorflow/contrib/makefile/downloads/eigen
        ${TF_INCLUDE_DIR}/tensorflow/tensorflow/contrib/makefile/downloads/gemmlowp
        ${TF_INCLUDE_DIR}/tensorflow/tensorflow/contrib/makefile/nsync/public
        ${TF_INCLUDE_DIR}/tensorflow/tensorflow/contrib/makefile/gen/protobuf-host/include
        ${TF_PYTHON_INCLUDE_DIR}/include/
        ${lwtnn_INCLUD_DIRS}

)

set(TF_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tensorflow_cc)

target_link_libraries(
        ${EXECUTABLE}
        ${TF_LIB_DIR}/libprotobuf.a
        ${TF_LIB_DIR}/libtensorflow_cc.so
        ${TF_LIB_DIR}/libtensorflow_framework.so
        ${lwtnn_LIBRARIES}
)

