cmake_minimum_required(VERSION 3.5)
project(Classifier_in_cpp)

set(CMAKE_CXX_STANDARD 14)

find_package(lwtnn)


set(SOURCE_FILES
        src/LWTNNClassifier.cpp src/LWTNNClassifier.h
        main.cpp
        )


set(EXECUTABLE Classifier_in_cpp)


add_executable(${EXECUTABLE} ${SOURCE_FILES})


include_directories(
        ${lwtnn_INCLUD_DIRS}

)


target_link_libraries(
        ${EXECUTABLE}
        ${lwtnn_LIBRARIES}
)

set_target_properties(Classifier_in_cpp
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/"
        )

