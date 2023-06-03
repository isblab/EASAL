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
include doc/examples/CMakeFiles/make_circulant2.dir/depend.make

# Include the progress variables for this target.
include doc/examples/CMakeFiles/make_circulant2.dir/progress.make

# Include the compile flags for this target's objects.
include doc/examples/CMakeFiles/make_circulant2.dir/flags.make

doc/examples/CMakeFiles/make_circulant2.dir/make_circulant2.cpp.o: doc/examples/CMakeFiles/make_circulant2.dir/flags.make
doc/examples/CMakeFiles/make_circulant2.dir/make_circulant2.cpp.o: /home/muskaan/easal-dev/include/Eigen/src/Eigen/doc/examples/make_circulant2.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/muskaan/easal-dev/include/Eigen/src/Eigen-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object doc/examples/CMakeFiles/make_circulant2.dir/make_circulant2.cpp.o"
	cd /home/muskaan/easal-dev/include/Eigen/src/Eigen-build/doc/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/make_circulant2.dir/make_circulant2.cpp.o -c /home/muskaan/easal-dev/include/Eigen/src/Eigen/doc/examples/make_circulant2.cpp

doc/examples/CMakeFiles/make_circulant2.dir/make_circulant2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/make_circulant2.dir/make_circulant2.cpp.i"
	cd /home/muskaan/easal-dev/include/Eigen/src/Eigen-build/doc/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/muskaan/easal-dev/include/Eigen/src/Eigen/doc/examples/make_circulant2.cpp > CMakeFiles/make_circulant2.dir/make_circulant2.cpp.i

doc/examples/CMakeFiles/make_circulant2.dir/make_circulant2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/make_circulant2.dir/make_circulant2.cpp.s"
	cd /home/muskaan/easal-dev/include/Eigen/src/Eigen-build/doc/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/muskaan/easal-dev/include/Eigen/src/Eigen/doc/examples/make_circulant2.cpp -o CMakeFiles/make_circulant2.dir/make_circulant2.cpp.s

# Object files for target make_circulant2
make_circulant2_OBJECTS = \
"CMakeFiles/make_circulant2.dir/make_circulant2.cpp.o"

# External object files for target make_circulant2
make_circulant2_EXTERNAL_OBJECTS =

doc/examples/make_circulant2: doc/examples/CMakeFiles/make_circulant2.dir/make_circulant2.cpp.o
doc/examples/make_circulant2: doc/examples/CMakeFiles/make_circulant2.dir/build.make
doc/examples/make_circulant2: doc/examples/CMakeFiles/make_circulant2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/muskaan/easal-dev/include/Eigen/src/Eigen-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable make_circulant2"
	cd /home/muskaan/easal-dev/include/Eigen/src/Eigen-build/doc/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/make_circulant2.dir/link.txt --verbose=$(VERBOSE)
	cd /home/muskaan/easal-dev/include/Eigen/src/Eigen-build/doc/examples && ./make_circulant2 >/home/muskaan/easal-dev/include/Eigen/src/Eigen-build/doc/examples/make_circulant2.out

# Rule to build all files generated by this target.
doc/examples/CMakeFiles/make_circulant2.dir/build: doc/examples/make_circulant2

.PHONY : doc/examples/CMakeFiles/make_circulant2.dir/build

doc/examples/CMakeFiles/make_circulant2.dir/clean:
	cd /home/muskaan/easal-dev/include/Eigen/src/Eigen-build/doc/examples && $(CMAKE_COMMAND) -P CMakeFiles/make_circulant2.dir/cmake_clean.cmake
.PHONY : doc/examples/CMakeFiles/make_circulant2.dir/clean

doc/examples/CMakeFiles/make_circulant2.dir/depend:
	cd /home/muskaan/easal-dev/include/Eigen/src/Eigen-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/muskaan/easal-dev/include/Eigen/src/Eigen /home/muskaan/easal-dev/include/Eigen/src/Eigen/doc/examples /home/muskaan/easal-dev/include/Eigen/src/Eigen-build /home/muskaan/easal-dev/include/Eigen/src/Eigen-build/doc/examples /home/muskaan/easal-dev/include/Eigen/src/Eigen-build/doc/examples/CMakeFiles/make_circulant2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : doc/examples/CMakeFiles/make_circulant2.dir/depend

