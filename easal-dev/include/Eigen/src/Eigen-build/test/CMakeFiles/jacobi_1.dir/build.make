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
include test/CMakeFiles/jacobi_1.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/jacobi_1.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/jacobi_1.dir/flags.make

test/CMakeFiles/jacobi_1.dir/jacobi.cpp.o: test/CMakeFiles/jacobi_1.dir/flags.make
test/CMakeFiles/jacobi_1.dir/jacobi.cpp.o: /home/muskaan/easal-dev/include/Eigen/src/Eigen/test/jacobi.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/muskaan/easal-dev/include/Eigen/src/Eigen-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/jacobi_1.dir/jacobi.cpp.o"
	cd /home/muskaan/easal-dev/include/Eigen/src/Eigen-build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/jacobi_1.dir/jacobi.cpp.o -c /home/muskaan/easal-dev/include/Eigen/src/Eigen/test/jacobi.cpp

test/CMakeFiles/jacobi_1.dir/jacobi.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jacobi_1.dir/jacobi.cpp.i"
	cd /home/muskaan/easal-dev/include/Eigen/src/Eigen-build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/muskaan/easal-dev/include/Eigen/src/Eigen/test/jacobi.cpp > CMakeFiles/jacobi_1.dir/jacobi.cpp.i

test/CMakeFiles/jacobi_1.dir/jacobi.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jacobi_1.dir/jacobi.cpp.s"
	cd /home/muskaan/easal-dev/include/Eigen/src/Eigen-build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/muskaan/easal-dev/include/Eigen/src/Eigen/test/jacobi.cpp -o CMakeFiles/jacobi_1.dir/jacobi.cpp.s

# Object files for target jacobi_1
jacobi_1_OBJECTS = \
"CMakeFiles/jacobi_1.dir/jacobi.cpp.o"

# External object files for target jacobi_1
jacobi_1_EXTERNAL_OBJECTS =

test/jacobi_1: test/CMakeFiles/jacobi_1.dir/jacobi.cpp.o
test/jacobi_1: test/CMakeFiles/jacobi_1.dir/build.make
test/jacobi_1: test/CMakeFiles/jacobi_1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/muskaan/easal-dev/include/Eigen/src/Eigen-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable jacobi_1"
	cd /home/muskaan/easal-dev/include/Eigen/src/Eigen-build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/jacobi_1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/jacobi_1.dir/build: test/jacobi_1

.PHONY : test/CMakeFiles/jacobi_1.dir/build

test/CMakeFiles/jacobi_1.dir/clean:
	cd /home/muskaan/easal-dev/include/Eigen/src/Eigen-build/test && $(CMAKE_COMMAND) -P CMakeFiles/jacobi_1.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/jacobi_1.dir/clean

test/CMakeFiles/jacobi_1.dir/depend:
	cd /home/muskaan/easal-dev/include/Eigen/src/Eigen-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/muskaan/easal-dev/include/Eigen/src/Eigen /home/muskaan/easal-dev/include/Eigen/src/Eigen/test /home/muskaan/easal-dev/include/Eigen/src/Eigen-build /home/muskaan/easal-dev/include/Eigen/src/Eigen-build/test /home/muskaan/easal-dev/include/Eigen/src/Eigen-build/test/CMakeFiles/jacobi_1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/jacobi_1.dir/depend

