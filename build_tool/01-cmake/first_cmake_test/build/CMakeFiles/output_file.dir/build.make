# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = /home/ziad/.espressif/tools/cmake/3.30.2/bin/cmake

# The command to remove a file.
RM = /home/ziad/.espressif/tools/cmake/3.30.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ziad/Ziad_WS/Embedded_Interns/Ziad_Studies/01-cmake

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ziad/Ziad_WS/Embedded_Interns/Ziad_Studies/01-cmake/build

# Include any dependencies generated for this target.
include CMakeFiles/output_file.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/output_file.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/output_file.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/output_file.dir/flags.make

CMakeFiles/output_file.dir/main.cpp.o: CMakeFiles/output_file.dir/flags.make
CMakeFiles/output_file.dir/main.cpp.o: /home/ziad/Ziad_WS/Embedded_Interns/Ziad_Studies/01-cmake/main.cpp
CMakeFiles/output_file.dir/main.cpp.o: CMakeFiles/output_file.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ziad/Ziad_WS/Embedded_Interns/Ziad_Studies/01-cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/output_file.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/output_file.dir/main.cpp.o -MF CMakeFiles/output_file.dir/main.cpp.o.d -o CMakeFiles/output_file.dir/main.cpp.o -c /home/ziad/Ziad_WS/Embedded_Interns/Ziad_Studies/01-cmake/main.cpp

CMakeFiles/output_file.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/output_file.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ziad/Ziad_WS/Embedded_Interns/Ziad_Studies/01-cmake/main.cpp > CMakeFiles/output_file.dir/main.cpp.i

CMakeFiles/output_file.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/output_file.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ziad/Ziad_WS/Embedded_Interns/Ziad_Studies/01-cmake/main.cpp -o CMakeFiles/output_file.dir/main.cpp.s

CMakeFiles/output_file.dir/src/calc.cpp.o: CMakeFiles/output_file.dir/flags.make
CMakeFiles/output_file.dir/src/calc.cpp.o: /home/ziad/Ziad_WS/Embedded_Interns/Ziad_Studies/01-cmake/src/calc.cpp
CMakeFiles/output_file.dir/src/calc.cpp.o: CMakeFiles/output_file.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ziad/Ziad_WS/Embedded_Interns/Ziad_Studies/01-cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/output_file.dir/src/calc.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/output_file.dir/src/calc.cpp.o -MF CMakeFiles/output_file.dir/src/calc.cpp.o.d -o CMakeFiles/output_file.dir/src/calc.cpp.o -c /home/ziad/Ziad_WS/Embedded_Interns/Ziad_Studies/01-cmake/src/calc.cpp

CMakeFiles/output_file.dir/src/calc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/output_file.dir/src/calc.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ziad/Ziad_WS/Embedded_Interns/Ziad_Studies/01-cmake/src/calc.cpp > CMakeFiles/output_file.dir/src/calc.cpp.i

CMakeFiles/output_file.dir/src/calc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/output_file.dir/src/calc.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ziad/Ziad_WS/Embedded_Interns/Ziad_Studies/01-cmake/src/calc.cpp -o CMakeFiles/output_file.dir/src/calc.cpp.s

# Object files for target output_file
output_file_OBJECTS = \
"CMakeFiles/output_file.dir/main.cpp.o" \
"CMakeFiles/output_file.dir/src/calc.cpp.o"

# External object files for target output_file
output_file_EXTERNAL_OBJECTS =

output_file: CMakeFiles/output_file.dir/main.cpp.o
output_file: CMakeFiles/output_file.dir/src/calc.cpp.o
output_file: CMakeFiles/output_file.dir/build.make
output_file: CMakeFiles/output_file.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/ziad/Ziad_WS/Embedded_Interns/Ziad_Studies/01-cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable output_file"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/output_file.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/output_file.dir/build: output_file
.PHONY : CMakeFiles/output_file.dir/build

CMakeFiles/output_file.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/output_file.dir/cmake_clean.cmake
.PHONY : CMakeFiles/output_file.dir/clean

CMakeFiles/output_file.dir/depend:
	cd /home/ziad/Ziad_WS/Embedded_Interns/Ziad_Studies/01-cmake/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ziad/Ziad_WS/Embedded_Interns/Ziad_Studies/01-cmake /home/ziad/Ziad_WS/Embedded_Interns/Ziad_Studies/01-cmake /home/ziad/Ziad_WS/Embedded_Interns/Ziad_Studies/01-cmake/build /home/ziad/Ziad_WS/Embedded_Interns/Ziad_Studies/01-cmake/build /home/ziad/Ziad_WS/Embedded_Interns/Ziad_Studies/01-cmake/build/CMakeFiles/output_file.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/output_file.dir/depend

