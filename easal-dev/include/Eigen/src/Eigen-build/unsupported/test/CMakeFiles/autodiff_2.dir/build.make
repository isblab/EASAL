# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/muskaan/easal-dev/include/Eigen/src/Eigen

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/muskaan/easal-dev/include/Eigen/src/Eigen-build

# Include any dependencies generated for this target.
include unsupported/test/CMakeFiles/autodiff_2.dir/depend.make

# Include the progress variables for this target.
include unsupported/test/CMakeFiles/autodiff_2.dir/progress.make

# Include the compile flags for this target's objects.
include unsupported/test/CMakeFiles/autodiff_2.dir/flags.make

unsupported/test/CMakeFiles/autodiff_2.dir/autodiff.cpp.o: unsupported/test/CMakeFiles/autodiff_2.dir/flags.make
unsupported/test/CMakeFiles/autodiff_2.dir/autodiff.cpp.o: /home/muskaan/easal-dev/include/Eigen/src/Eigen/unsupported/test/autodiff.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/muskaan/easal-dev/include/Eigen/src/Eigen-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object unsupported/test/CMakeFiles/autodiff_2.dir/autodiff.cpp.o"
	cd /home/muskaan/easal-dev/include/Eigen/src/Eigen-build/unsupported/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/autodiff_2.dir/autodiff.cpp.o -c /home/muskaan/easal-dev/include/Eigen/src/Eigen/unsupported/test/autodiff.cpp

unsupported/test/CMakeFiles/autodiff_2.dir/autodiff.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/autodiff_2.dir/autodiff.cpp.i"
	cd /home/muskaan/easal-dev/include/Eigen/src/Eigen-build/unsupported/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/muskaan/easal-dev/include/Eigen/src/Eigen/unsupported/test/autodiff.cpp > CMakeFiles/autodiff_2.dir/autodiff.cpp.i

unsupported/test/CMakeFiles/autodiff_2.dir/autodiff.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/autodiff_2.dir/autodiff.cpp.s"
	cd /home/muskaan/easal-dev/include/Eigen/src/Eigen-build/unsupported/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/muskaan/easal-dev/include/Eigen/src/Eigen/unsupported/test/autodiff.cpp -o CMakeFiles/autodiff_2.dir/autodiff.cpp.s

# Object files for target autodiff_2
autodiff_2_OBJECTS = \
"CMakeFiles/autodiff_2.dir/autodiff.cpp.o"

# External object files for target autodiff_2
autodiff_2_EXTERNAL_OBJECTS =

unsupported/test/autodiff_2: unsupported/test/CMakeFiles/autodiff_2.dir/autodiff.cpp.o
unsupported/test/autodiff_2: unsupported/test/CMakeFiles/autodiff_2.dir/build.make
unsupported/test/autodiff_2: unsupported/test/CMakeFiles/autodiff_2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/muskaan/easal-dev/include/Eigen/src/Eigen-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable autodiff_2"
	cd /home/muskaan/easal-dev/include/Eigen/src/Eigen-build/unsupported/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/autodiff_2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
unsupported/test/CMakeFiles/autodiff_2.dir/build: unsupported/test/autodiff_2

.PHONY : unsupported/test/CMakeFiles/autodiff_2.dir/build

unsupported/test/CMakeFiles/autodiff_2.dir/clean:
	cd /home/muskaan/easal-dev/include/Eigen/src/Eigen-build/unsupported/test && $(CMAKE_COMMAND) -P CMakeFiles/autodiff_2.dir/cmake_clean.cmake
.PHONY : unsupported/test/CMakeFiles/autodiff_2.dir/clean

unsupported/test/CMakeFiles/autodiff_2.dir/depend:
	cd /home/muskaan/easal-dev/include/Eigen/src/Eigen-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/muskaan/easal-dev/include/Eigen/src/Eigen /home/muskaan/easal-dev/include/Eigen/src/Eigen/unsupported/test /home/muskaan/easal-dev/include/Eigen/src/Eigen-build /home/muskaan/easal-dev/include/Eigen/src/Eigen-build/unsupported/test /home/muskaan/easal-dev/include/Eigen/src/Eigen-build/unsupported/test/CMakeFiles/autodiff_2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : unsupported/test/CMakeFiles/autodiff_2.dir/depend

