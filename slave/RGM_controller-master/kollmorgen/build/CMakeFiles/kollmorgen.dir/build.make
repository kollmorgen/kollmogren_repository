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
CMAKE_SOURCE_DIR = /home/mfs/桌面/2-下位机/RGM_controller-master/kollmorgen

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mfs/桌面/2-下位机/RGM_controller-master/kollmorgen/build

# Include any dependencies generated for this target.
include CMakeFiles/kollmorgen.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/kollmorgen.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/kollmorgen.dir/flags.make

CMakeFiles/kollmorgen.dir/kollmorgen.c.o: CMakeFiles/kollmorgen.dir/flags.make
CMakeFiles/kollmorgen.dir/kollmorgen.c.o: ../kollmorgen.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mfs/桌面/2-下位机/RGM_controller-master/kollmorgen/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/kollmorgen.dir/kollmorgen.c.o"
	arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/kollmorgen.dir/kollmorgen.c.o   -c /home/mfs/桌面/2-下位机/RGM_controller-master/kollmorgen/kollmorgen.c

CMakeFiles/kollmorgen.dir/kollmorgen.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kollmorgen.dir/kollmorgen.c.i"
	arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/mfs/桌面/2-下位机/RGM_controller-master/kollmorgen/kollmorgen.c > CMakeFiles/kollmorgen.dir/kollmorgen.c.i

CMakeFiles/kollmorgen.dir/kollmorgen.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kollmorgen.dir/kollmorgen.c.s"
	arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/mfs/桌面/2-下位机/RGM_controller-master/kollmorgen/kollmorgen.c -o CMakeFiles/kollmorgen.dir/kollmorgen.c.s

CMakeFiles/kollmorgen.dir/kollmorgen.c.o.requires:

.PHONY : CMakeFiles/kollmorgen.dir/kollmorgen.c.o.requires

CMakeFiles/kollmorgen.dir/kollmorgen.c.o.provides: CMakeFiles/kollmorgen.dir/kollmorgen.c.o.requires
	$(MAKE) -f CMakeFiles/kollmorgen.dir/build.make CMakeFiles/kollmorgen.dir/kollmorgen.c.o.provides.build
.PHONY : CMakeFiles/kollmorgen.dir/kollmorgen.c.o.provides

CMakeFiles/kollmorgen.dir/kollmorgen.c.o.provides.build: CMakeFiles/kollmorgen.dir/kollmorgen.c.o


# Object files for target kollmorgen
kollmorgen_OBJECTS = \
"CMakeFiles/kollmorgen.dir/kollmorgen.c.o"

# External object files for target kollmorgen
kollmorgen_EXTERNAL_OBJECTS =

kollmorgen: CMakeFiles/kollmorgen.dir/kollmorgen.c.o
kollmorgen: CMakeFiles/kollmorgen.dir/build.make
kollmorgen: canopen/src/libcanfestival.a
kollmorgen: canopen/drivers/unix/libcanfestival_unix.a
kollmorgen: canopen/rgm6/librgm6.a
kollmorgen: canopen/tcp/libtcp.a
kollmorgen: canopen/mthread/libmthread.a
kollmorgen: canopen/src/libcanfestival.a
kollmorgen: canopen/drivers/unix/libcanfestival_unix.a
kollmorgen: canopen/drivers/can_socket/libcanfestival_unix_socket.a
kollmorgen: canopen/rgm6/librgm6.a
kollmorgen: canopen/tcp/libtcp.a
kollmorgen: CMakeFiles/kollmorgen.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mfs/桌面/2-下位机/RGM_controller-master/kollmorgen/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable kollmorgen"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/kollmorgen.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/kollmorgen.dir/build: kollmorgen

.PHONY : CMakeFiles/kollmorgen.dir/build

CMakeFiles/kollmorgen.dir/requires: CMakeFiles/kollmorgen.dir/kollmorgen.c.o.requires

.PHONY : CMakeFiles/kollmorgen.dir/requires

CMakeFiles/kollmorgen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/kollmorgen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/kollmorgen.dir/clean

CMakeFiles/kollmorgen.dir/depend:
	cd /home/mfs/桌面/2-下位机/RGM_controller-master/kollmorgen/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mfs/桌面/2-下位机/RGM_controller-master/kollmorgen /home/mfs/桌面/2-下位机/RGM_controller-master/kollmorgen /home/mfs/桌面/2-下位机/RGM_controller-master/kollmorgen/build /home/mfs/桌面/2-下位机/RGM_controller-master/kollmorgen/build /home/mfs/桌面/2-下位机/RGM_controller-master/kollmorgen/build/CMakeFiles/kollmorgen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/kollmorgen.dir/depend

