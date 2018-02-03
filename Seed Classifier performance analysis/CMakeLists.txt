cmake_minimum_required(VERSION 3.0)

project(Seed-Clf-Evaluation)

set(CMAKE_CXX_STANDARD 14)
message(${CMAKE_BINARY_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

find_package(ROOT REQUIRED Core Gui Net Hist Graf RooFitCore RooFit RIO RooStats )
message($ENV{LD_LIBRARY_PATH})
set(CMAKE_CXX_FLAGS
        "-O2 -Wall -pedantic${ROOT_CXX_FLAGS}")

include(${ROOT_USE_FILE})
message(${CMAKE_CXX_FLAGS})
message(${ROOT_LIBRARIES})
message(${ROOT_INCLUDE_DIRS})

set(SOURCE_FILES
        src/main.cpp
        src/ratioplot.C
        include/InputData_K.h
        include/InputData_L.h
        include/Counters.h
        include/Histograms.h
        include/ratioplot.h
        include/fitMass.h
        src/fitMass.cpp
        )

set(EXECUTABLE Seed-Clf-Evaluation)

include_directories(include)

add_executable(${EXECUTABLE} ${SOURCE_FILES})


target_link_libraries(Seed-Clf-Evaluation ${ROOT_LIBRARIES})

add_executable(roo_fit_example src/roo_fit_example.cpp)
target_link_libraries(roo_fit_example ${ROOT_LIBRARIES})