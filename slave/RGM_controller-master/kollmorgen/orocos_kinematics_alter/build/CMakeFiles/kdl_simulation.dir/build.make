# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.11

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
CMAKE_COMMAND = /home/thy/app/cmake-3.11.0/bin/cmake

# The command to remove a file.
RM = /home/thy/app/cmake-3.11.0/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/thy/app/orocos_kinematics_alter

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/thy/app/orocos_kinematics_alter/build

# Include any dependencies generated for this target.
include CMakeFiles/kdl_simulation.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/kdl_simulation.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/kdl_simulation.dir/flags.make

CMakeFiles/kdl_simulation.dir/examples/RGMtrajTest0.c.o: CMakeFiles/kdl_simulation.dir/flags.make
CMakeFiles/kdl_simulation.dir/examples/RGMtrajTest0.c.o: ../examples/RGMtrajTest0.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/thy/app/orocos_kinematics_alter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/kdl_simulation.dir/examples/RGMtrajTest0.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/kdl_simulation.dir/examples/RGMtrajTest0.c.o   -c /home/thy/app/orocos_kinematics_alter/examples/RGMtrajTest0.c

CMakeFiles/kdl_simulation.dir/examples/RGMtrajTest0.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kdl_simulation.dir/examples/RGMtrajTest0.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/thy/app/orocos_kinematics_alter/examples/RGMtrajTest0.c > CMakeFiles/kdl_simulation.dir/examples/RGMtrajTest0.c.i

CMakeFiles/kdl_simulation.dir/examples/RGMtrajTest0.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kdl_simulation.dir/examples/RGMtrajTest0.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/thy/app/orocos_kinematics_alter/examples/RGMtrajTest0.c -o CMakeFiles/kdl_simulation.dir/examples/RGMtrajTest0.c.s

# Object files for target kdl_simulation
kdl_simulation_OBJECTS = \
"CMakeFiles/kdl_simulation.dir/examples/RGMtrajTest0.c.o"

# External object files for target kdl_simulation
kdl_simulation_EXTERNAL_OBJECTS =

kdl_simulation: CMakeFiles/kdl_simulation.dir/examples/RGMtrajTest0.c.o
kdl_simulation: CMakeFiles/kdl_simulation.dir/build.make
kdl_simulation: src/liborocos_src.a
kdl_simulation: models/liborocos_model.a
kdl_simulation: src/utilities/liborocos_uti.a
kdl_simulation: models/liborocos_model.a
kdl_simulation: src/liborocos_src.a
kdl_simulation: src/utilities/liborocos_uti.a
kdl_simulation: CMakeFiles/kdl_simulation.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/thy/app/orocos_kinematics_alter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable kdl_simulation"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/kdl_simulation.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/kdl_simulation.dir/build: kdl_simulation

.PHONY : CMakeFiles/kdl_simulation.dir/build

CMakeFiles/kdl_simulation.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/kdl_simulation.dir/cmake_clean.cmake
.PHONY : CMakeFiles/kdl_simulation.dir/clean

CMakeFiles/kdl_simulation.dir/depend:
	cd /home/thy/app/orocos_kinematics_alter/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/thy/app/orocos_kinematics_alter /home/thy/app/orocos_kinematics_alter /home/thy/app/orocos_kinematics_alter/build /home/thy/app/orocos_kinematics_alter/build /home/thy/app/orocos_kinematics_alter/build/CMakeFiles/kdl_simulation.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/kdl_simulation.dir/depend

