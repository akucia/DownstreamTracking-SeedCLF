# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/kuciu/uczelnia/DownstreamTracking-SeedCLF/Seed Classifier implementation/Seed Classifier - local"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/kuciu/uczelnia/DownstreamTracking-SeedCLF/Seed Classifier implementation/Seed Classifier - local/build"

# Include any dependencies generated for this target.
include CMakeFiles/Classifier_in_cpp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Classifier_in_cpp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Classifier_in_cpp.dir/flags.make

CMakeFiles/Classifier_in_cpp.dir/src/LWTNNClassifier.cpp.o: CMakeFiles/Classifier_in_cpp.dir/flags.make
CMakeFiles/Classifier_in_cpp.dir/src/LWTNNClassifier.cpp.o: ../src/LWTNNClassifier.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/kuciu/uczelnia/DownstreamTracking-SeedCLF/Seed Classifier implementation/Seed Classifier - local/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Classifier_in_cpp.dir/src/LWTNNClassifier.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Classifier_in_cpp.dir/src/LWTNNClassifier.cpp.o -c "/home/kuciu/uczelnia/DownstreamTracking-SeedCLF/Seed Classifier implementation/Seed Classifier - local/src/LWTNNClassifier.cpp"

CMakeFiles/Classifier_in_cpp.dir/src/LWTNNClassifier.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Classifier_in_cpp.dir/src/LWTNNClassifier.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/kuciu/uczelnia/DownstreamTracking-SeedCLF/Seed Classifier implementation/Seed Classifier - local/src/LWTNNClassifier.cpp" > CMakeFiles/Classifier_in_cpp.dir/src/LWTNNClassifier.cpp.i

CMakeFiles/Classifier_in_cpp.dir/src/LWTNNClassifier.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Classifier_in_cpp.dir/src/LWTNNClassifier.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/kuciu/uczelnia/DownstreamTracking-SeedCLF/Seed Classifier implementation/Seed Classifier - local/src/LWTNNClassifier.cpp" -o CMakeFiles/Classifier_in_cpp.dir/src/LWTNNClassifier.cpp.s

CMakeFiles/Classifier_in_cpp.dir/src/LWTNNClassifier.cpp.o.requires:

.PHONY : CMakeFiles/Classifier_in_cpp.dir/src/LWTNNClassifier.cpp.o.requires

CMakeFiles/Classifier_in_cpp.dir/src/LWTNNClassifier.cpp.o.provides: CMakeFiles/Classifier_in_cpp.dir/src/LWTNNClassifier.cpp.o.requires
	$(MAKE) -f CMakeFiles/Classifier_in_cpp.dir/build.make CMakeFiles/Classifier_in_cpp.dir/src/LWTNNClassifier.cpp.o.provides.build
.PHONY : CMakeFiles/Classifier_in_cpp.dir/src/LWTNNClassifier.cpp.o.provides

CMakeFiles/Classifier_in_cpp.dir/src/LWTNNClassifier.cpp.o.provides.build: CMakeFiles/Classifier_in_cpp.dir/src/LWTNNClassifier.cpp.o


CMakeFiles/Classifier_in_cpp.dir/main.cpp.o: CMakeFiles/Classifier_in_cpp.dir/flags.make
CMakeFiles/Classifier_in_cpp.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/kuciu/uczelnia/DownstreamTracking-SeedCLF/Seed Classifier implementation/Seed Classifier - local/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Classifier_in_cpp.dir/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Classifier_in_cpp.dir/main.cpp.o -c "/home/kuciu/uczelnia/DownstreamTracking-SeedCLF/Seed Classifier implementation/Seed Classifier - local/main.cpp"

CMakeFiles/Classifier_in_cpp.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Classifier_in_cpp.dir/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/kuciu/uczelnia/DownstreamTracking-SeedCLF/Seed Classifier implementation/Seed Classifier - local/main.cpp" > CMakeFiles/Classifier_in_cpp.dir/main.cpp.i

CMakeFiles/Classifier_in_cpp.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Classifier_in_cpp.dir/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/kuciu/uczelnia/DownstreamTracking-SeedCLF/Seed Classifier implementation/Seed Classifier - local/main.cpp" -o CMakeFiles/Classifier_in_cpp.dir/main.cpp.s

CMakeFiles/Classifier_in_cpp.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/Classifier_in_cpp.dir/main.cpp.o.requires

CMakeFiles/Classifier_in_cpp.dir/main.cpp.o.provides: CMakeFiles/Classifier_in_cpp.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/Classifier_in_cpp.dir/build.make CMakeFiles/Classifier_in_cpp.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/Classifier_in_cpp.dir/main.cpp.o.provides

CMakeFiles/Classifier_in_cpp.dir/main.cpp.o.provides.build: CMakeFiles/Classifier_in_cpp.dir/main.cpp.o


# Object files for target Classifier_in_cpp
Classifier_in_cpp_OBJECTS = \
"CMakeFiles/Classifier_in_cpp.dir/src/LWTNNClassifier.cpp.o" \
"CMakeFiles/Classifier_in_cpp.dir/main.cpp.o"

# External object files for target Classifier_in_cpp
Classifier_in_cpp_EXTERNAL_OBJECTS =

../bin/Classifier_in_cpp: CMakeFiles/Classifier_in_cpp.dir/src/LWTNNClassifier.cpp.o
../bin/Classifier_in_cpp: CMakeFiles/Classifier_in_cpp.dir/main.cpp.o
../bin/Classifier_in_cpp: CMakeFiles/Classifier_in_cpp.dir/build.make
../bin/Classifier_in_cpp: /usr/local/lib/liblwtnn.so
../bin/Classifier_in_cpp: CMakeFiles/Classifier_in_cpp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/kuciu/uczelnia/DownstreamTracking-SeedCLF/Seed Classifier implementation/Seed Classifier - local/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../bin/Classifier_in_cpp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Classifier_in_cpp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Classifier_in_cpp.dir/build: ../bin/Classifier_in_cpp

.PHONY : CMakeFiles/Classifier_in_cpp.dir/build

CMakeFiles/Classifier_in_cpp.dir/requires: CMakeFiles/Classifier_in_cpp.dir/src/LWTNNClassifier.cpp.o.requires
CMakeFiles/Classifier_in_cpp.dir/requires: CMakeFiles/Classifier_in_cpp.dir/main.cpp.o.requires

.PHONY : CMakeFiles/Classifier_in_cpp.dir/requires

CMakeFiles/Classifier_in_cpp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Classifier_in_cpp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Classifier_in_cpp.dir/clean

CMakeFiles/Classifier_in_cpp.dir/depend:
	cd "/home/kuciu/uczelnia/DownstreamTracking-SeedCLF/Seed Classifier implementation/Seed Classifier - local/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/kuciu/uczelnia/DownstreamTracking-SeedCLF/Seed Classifier implementation/Seed Classifier - local" "/home/kuciu/uczelnia/DownstreamTracking-SeedCLF/Seed Classifier implementation/Seed Classifier - local" "/home/kuciu/uczelnia/DownstreamTracking-SeedCLF/Seed Classifier implementation/Seed Classifier - local/build" "/home/kuciu/uczelnia/DownstreamTracking-SeedCLF/Seed Classifier implementation/Seed Classifier - local/build" "/home/kuciu/uczelnia/DownstreamTracking-SeedCLF/Seed Classifier implementation/Seed Classifier - local/build/CMakeFiles/Classifier_in_cpp.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Classifier_in_cpp.dir/depend

