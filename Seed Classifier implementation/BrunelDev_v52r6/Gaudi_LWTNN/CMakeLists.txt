gaudi_subdir(lwtnn v1)
# Set the minimum CMake version required to build the project.
cmake_minimum_required( VERSION 3.1 )

# Set the project's name and version.
project( lwtnn VERSION 2.4 )

# Look for Boost on the build system.
find_package( Boost 1.54.0 REQUIRED )
# Look for Eigen on the build system.
find_package( Eigen REQUIRED )

#list( INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake )

# Public header files for the shared/static library.
set( lwtnn_lib_headers src/Exceptions.hh src/Graph.hh
		src/InputPreprocessor.hh src/LightweightGraph.hh
		src/LightweightNeuralNetwork.hh src/NNLayerConfig.hh
		src/NanReplacer.hh src/Source.hh src/Stack.hh
		src/Stack.hh src/lightweight_network_config.hh
		src/lightweight_nn_streamers.hh src/parse_json.hh)
# Source files for the shared/static library.
set( lwtnn_lib_sources src/Exceptions.cxx src/Graph.cxx src/InputPreprocessor.cxx
		src/LightweightGraph.cxx src/LightweightNeuralNetwork.cxx src/NanReplacer.cxx
		src/Stack.cxx src/lightweight_nn_streamers.cxx src/parse_json.cxx
		src/test_utilities.hh src/test_utilities.cxx)

# Build the shared library.
set(lwtnn_INCLUDE_DIR ${CMAKE_CURRENT_DIR}/include/lwtnn/)
gaudi_add_library(lwtnn
		${lwtnn_lib_sources}
		PUBLIC_HEADERS ${lwtnn_lib_headers_dir} ${EIGEN_INCLUDE_DIR}
		INCLUDE_DIRS Boost ${EIGEN_INCLUDE_DIR} lwtnn
		LINK_LIBRARIES Boost Eigen
)
