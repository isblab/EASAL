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
CMAKE_SOURCE_DIR = /home/muskaan/easal-dev/include/glog/src/glog

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/muskaan/easal-dev/include/glog/src/glog-build

# Include any dependencies generated for this target.
include CMakeFiles/glogtest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/glogtest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/glogtest.dir/flags.make

# Object files for target glogtest
glogtest_OBJECTS =

# External object files for target glogtest
glogtest_EXTERNAL_OBJECTS = \
"/home/muskaan/easal-dev/include/glog/src/glog-build/CMakeFiles/glog_internal.dir/src/demangle.cc.o" \
"/home/muskaan/easal-dev/include/glog/src/glog-build/CMakeFiles/glog_internal.dir/src/logging.cc.o" \
"/home/muskaan/easal-dev/include/glog/src/glog-build/CMakeFiles/glog_internal.dir/src/raw_logging.cc.o" \
"/home/muskaan/easal-dev/include/glog/src/glog-build/CMakeFiles/glog_internal.dir/src/symbolize.cc.o" \
"/home/muskaan/easal-dev/include/glog/src/glog-build/CMakeFiles/glog_internal.dir/src/utilities.cc.o" \
"/home/muskaan/easal-dev/include/glog/src/glog-build/CMakeFiles/glog_internal.dir/src/vlog_is_on.cc.o" \
"/home/muskaan/easal-dev/include/glog/src/glog-build/CMakeFiles/glog_internal.dir/src/signalhandler.cc.o"

libglogtest.a: CMakeFiles/glog_internal.dir/src/demangle.cc.o
libglogtest.a: CMakeFiles/glog_internal.dir/src/logging.cc.o
libglogtest.a: CMakeFiles/glog_internal.dir/src/raw_logging.cc.o
libglogtest.a: CMakeFiles/glog_internal.dir/src/symbolize.cc.o
libglogtest.a: CMakeFiles/glog_internal.dir/src/utilities.cc.o
libglogtest.a: CMakeFiles/glog_internal.dir/src/vlog_is_on.cc.o
libglogtest.a: CMakeFiles/glog_internal.dir/src/signalhandler.cc.o
libglogtest.a: CMakeFiles/glogtest.dir/build.make
libglogtest.a: CMakeFiles/glogtest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/muskaan/easal-dev/include/glog/src/glog-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Linking CXX static library libglogtest.a"
	$(CMAKE_COMMAND) -P CMakeFiles/glogtest.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/glogtest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/glogtest.dir/build: libglogtest.a

.PHONY : CMakeFiles/glogtest.dir/build

CMakeFiles/glogtest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/glogtest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/glogtest.dir/clean

CMakeFiles/glogtest.dir/depend:
	cd /home/muskaan/easal-dev/include/glog/src/glog-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/muskaan/easal-dev/include/glog/src/glog /home/muskaan/easal-dev/include/glog/src/glog /home/muskaan/easal-dev/include/glog/src/glog-build /home/muskaan/easal-dev/include/glog/src/glog-build /home/muskaan/easal-dev/include/glog/src/glog-build/CMakeFiles/glogtest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/glogtest.dir/depend

