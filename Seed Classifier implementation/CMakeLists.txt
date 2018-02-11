cmake_minimum_required(VERSION 3.8)
project(Classifier_in_cpp)

set(CMAKE_CXX_STANDARD 14)

find_package(lwtnn)


set(SOURCE_FILES
        src/LWTNNClassifier.cpp src/LWTNNClassifier.h
        src/LWTNNClassifier.cpp src/LWTNNClassifier.h
        main.cpp
        )



set(EXECUTABLE Classifier_in_cpp)


add_executable(${EXECUTABLE} ${SOURCE_FILES})


set(TF_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tensorflow_cc/tensorflow_include)
set(TF_PYTHON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tensorflow_cc/tensorflow_python_include)


include_directories(
        ${lwtnn_INCLUD_DIRS}

)

set(TF_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tensorflow_cc)

target_link_libraries(
        ${EXECUTABLE}
        ${lwtnn_LIBRARIES}
)

