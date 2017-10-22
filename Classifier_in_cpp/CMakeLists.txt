cmake_minimum_required(VERSION 3.8)
project(Classifier_in_cpp)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -pedantic")

set(SOURCE_FILES
        main.cpp
        src/myconstants.h
        src/BinaryClassifier.cpp src/BinaryClassifier.h
#        src/PatNeuralNetworkSeedClassifier.cpp src/PatNeuralNetworkSeedClassifier.h
        )

set(EXECUTABLE Classifier_in_cpp)

find_package(TensorflowCC COMPONENTS Shared)
include_directories(tf_includes)
add_executable(${EXECUTABLE} ${SOURCE_FILES})

target_link_libraries(${EXECUTABLE} TensorflowCC::Shared)

