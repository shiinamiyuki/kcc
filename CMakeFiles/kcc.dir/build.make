# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.14

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2019.1.4\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2019.1.4\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\xiaoc\source\repos\shiinamiyuki\kcc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\xiaoc\source\repos\shiinamiyuki\kcc

# Include any dependencies generated for this target.
include CMakeFiles/kcc.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/kcc.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/kcc.dir/flags.make

CMakeFiles/kcc.dir/src/ast.cc.obj: CMakeFiles/kcc.dir/flags.make
CMakeFiles/kcc.dir/src/ast.cc.obj: CMakeFiles/kcc.dir/includes_CXX.rsp
CMakeFiles/kcc.dir/src/ast.cc.obj: src/ast.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/kcc.dir/src/ast.cc.obj"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\kcc.dir\src\ast.cc.obj -c C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\ast.cc

CMakeFiles/kcc.dir/src/ast.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kcc.dir/src/ast.cc.i"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\ast.cc > CMakeFiles\kcc.dir\src\ast.cc.i

CMakeFiles/kcc.dir/src/ast.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kcc.dir/src/ast.cc.s"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\ast.cc -o CMakeFiles\kcc.dir\src\ast.cc.s

CMakeFiles/kcc.dir/src/compile.cc.obj: CMakeFiles/kcc.dir/flags.make
CMakeFiles/kcc.dir/src/compile.cc.obj: CMakeFiles/kcc.dir/includes_CXX.rsp
CMakeFiles/kcc.dir/src/compile.cc.obj: src/compile.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/kcc.dir/src/compile.cc.obj"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\kcc.dir\src\compile.cc.obj -c C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\compile.cc

CMakeFiles/kcc.dir/src/compile.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kcc.dir/src/compile.cc.i"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\compile.cc > CMakeFiles\kcc.dir\src\compile.cc.i

CMakeFiles/kcc.dir/src/compile.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kcc.dir/src/compile.cc.s"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\compile.cc -o CMakeFiles\kcc.dir\src\compile.cc.s

CMakeFiles/kcc.dir/src/cpp.cc.obj: CMakeFiles/kcc.dir/flags.make
CMakeFiles/kcc.dir/src/cpp.cc.obj: CMakeFiles/kcc.dir/includes_CXX.rsp
CMakeFiles/kcc.dir/src/cpp.cc.obj: src/cpp.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/kcc.dir/src/cpp.cc.obj"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\kcc.dir\src\cpp.cc.obj -c C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\cpp.cc

CMakeFiles/kcc.dir/src/cpp.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kcc.dir/src/cpp.cc.i"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\cpp.cc > CMakeFiles\kcc.dir\src\cpp.cc.i

CMakeFiles/kcc.dir/src/cpp.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kcc.dir/src/cpp.cc.s"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\cpp.cc -o CMakeFiles\kcc.dir\src\cpp.cc.s

CMakeFiles/kcc.dir/src/lex.cc.obj: CMakeFiles/kcc.dir/flags.make
CMakeFiles/kcc.dir/src/lex.cc.obj: CMakeFiles/kcc.dir/includes_CXX.rsp
CMakeFiles/kcc.dir/src/lex.cc.obj: src/lex.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/kcc.dir/src/lex.cc.obj"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\kcc.dir\src\lex.cc.obj -c C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\lex.cc

CMakeFiles/kcc.dir/src/lex.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kcc.dir/src/lex.cc.i"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\lex.cc > CMakeFiles\kcc.dir\src\lex.cc.i

CMakeFiles/kcc.dir/src/lex.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kcc.dir/src/lex.cc.s"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\lex.cc -o CMakeFiles\kcc.dir\src\lex.cc.s

CMakeFiles/kcc.dir/src/main.cc.obj: CMakeFiles/kcc.dir/flags.make
CMakeFiles/kcc.dir/src/main.cc.obj: CMakeFiles/kcc.dir/includes_CXX.rsp
CMakeFiles/kcc.dir/src/main.cc.obj: src/main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/kcc.dir/src/main.cc.obj"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\kcc.dir\src\main.cc.obj -c C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\main.cc

CMakeFiles/kcc.dir/src/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kcc.dir/src/main.cc.i"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\main.cc > CMakeFiles\kcc.dir\src\main.cc.i

CMakeFiles/kcc.dir/src/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kcc.dir/src/main.cc.s"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\main.cc -o CMakeFiles\kcc.dir\src\main.cc.s

CMakeFiles/kcc.dir/src/parse.cc.obj: CMakeFiles/kcc.dir/flags.make
CMakeFiles/kcc.dir/src/parse.cc.obj: CMakeFiles/kcc.dir/includes_CXX.rsp
CMakeFiles/kcc.dir/src/parse.cc.obj: src/parse.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/kcc.dir/src/parse.cc.obj"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\kcc.dir\src\parse.cc.obj -c C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\parse.cc

CMakeFiles/kcc.dir/src/parse.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kcc.dir/src/parse.cc.i"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\parse.cc > CMakeFiles\kcc.dir\src\parse.cc.i

CMakeFiles/kcc.dir/src/parse.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kcc.dir/src/parse.cc.s"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\parse.cc -o CMakeFiles\kcc.dir\src\parse.cc.s

CMakeFiles/kcc.dir/src/sema.cc.obj: CMakeFiles/kcc.dir/flags.make
CMakeFiles/kcc.dir/src/sema.cc.obj: CMakeFiles/kcc.dir/includes_CXX.rsp
CMakeFiles/kcc.dir/src/sema.cc.obj: src/sema.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/kcc.dir/src/sema.cc.obj"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\kcc.dir\src\sema.cc.obj -c C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\sema.cc

CMakeFiles/kcc.dir/src/sema.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kcc.dir/src/sema.cc.i"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\sema.cc > CMakeFiles\kcc.dir\src\sema.cc.i

CMakeFiles/kcc.dir/src/sema.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kcc.dir/src/sema.cc.s"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\sema.cc -o CMakeFiles\kcc.dir\src\sema.cc.s

CMakeFiles/kcc.dir/src/type.cc.obj: CMakeFiles/kcc.dir/flags.make
CMakeFiles/kcc.dir/src/type.cc.obj: CMakeFiles/kcc.dir/includes_CXX.rsp
CMakeFiles/kcc.dir/src/type.cc.obj: src/type.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/kcc.dir/src/type.cc.obj"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\kcc.dir\src\type.cc.obj -c C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\type.cc

CMakeFiles/kcc.dir/src/type.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kcc.dir/src/type.cc.i"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\type.cc > CMakeFiles\kcc.dir\src\type.cc.i

CMakeFiles/kcc.dir/src/type.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kcc.dir/src/type.cc.s"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\type.cc -o CMakeFiles\kcc.dir\src\type.cc.s

CMakeFiles/kcc.dir/src/fmt/format.cc.obj: CMakeFiles/kcc.dir/flags.make
CMakeFiles/kcc.dir/src/fmt/format.cc.obj: CMakeFiles/kcc.dir/includes_CXX.rsp
CMakeFiles/kcc.dir/src/fmt/format.cc.obj: src/fmt/format.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/kcc.dir/src/fmt/format.cc.obj"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\kcc.dir\src\fmt\format.cc.obj -c C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\fmt\format.cc

CMakeFiles/kcc.dir/src/fmt/format.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kcc.dir/src/fmt/format.cc.i"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\fmt\format.cc > CMakeFiles\kcc.dir\src\fmt\format.cc.i

CMakeFiles/kcc.dir/src/fmt/format.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kcc.dir/src/fmt/format.cc.s"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\fmt\format.cc -o CMakeFiles\kcc.dir\src\fmt\format.cc.s

CMakeFiles/kcc.dir/src/fmt/posix.cc.obj: CMakeFiles/kcc.dir/flags.make
CMakeFiles/kcc.dir/src/fmt/posix.cc.obj: CMakeFiles/kcc.dir/includes_CXX.rsp
CMakeFiles/kcc.dir/src/fmt/posix.cc.obj: src/fmt/posix.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/kcc.dir/src/fmt/posix.cc.obj"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\kcc.dir\src\fmt\posix.cc.obj -c C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\fmt\posix.cc

CMakeFiles/kcc.dir/src/fmt/posix.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kcc.dir/src/fmt/posix.cc.i"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\fmt\posix.cc > CMakeFiles\kcc.dir\src\fmt\posix.cc.i

CMakeFiles/kcc.dir/src/fmt/posix.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kcc.dir/src/fmt/posix.cc.s"
	C:\PROGRA~1\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\src\fmt\posix.cc -o CMakeFiles\kcc.dir\src\fmt\posix.cc.s

# Object files for target kcc
kcc_OBJECTS = \
"CMakeFiles/kcc.dir/src/ast.cc.obj" \
"CMakeFiles/kcc.dir/src/compile.cc.obj" \
"CMakeFiles/kcc.dir/src/cpp.cc.obj" \
"CMakeFiles/kcc.dir/src/lex.cc.obj" \
"CMakeFiles/kcc.dir/src/main.cc.obj" \
"CMakeFiles/kcc.dir/src/parse.cc.obj" \
"CMakeFiles/kcc.dir/src/sema.cc.obj" \
"CMakeFiles/kcc.dir/src/type.cc.obj" \
"CMakeFiles/kcc.dir/src/fmt/format.cc.obj" \
"CMakeFiles/kcc.dir/src/fmt/posix.cc.obj"

# External object files for target kcc
kcc_EXTERNAL_OBJECTS =

kcc.exe: CMakeFiles/kcc.dir/src/ast.cc.obj
kcc.exe: CMakeFiles/kcc.dir/src/compile.cc.obj
kcc.exe: CMakeFiles/kcc.dir/src/cpp.cc.obj
kcc.exe: CMakeFiles/kcc.dir/src/lex.cc.obj
kcc.exe: CMakeFiles/kcc.dir/src/main.cc.obj
kcc.exe: CMakeFiles/kcc.dir/src/parse.cc.obj
kcc.exe: CMakeFiles/kcc.dir/src/sema.cc.obj
kcc.exe: CMakeFiles/kcc.dir/src/type.cc.obj
kcc.exe: CMakeFiles/kcc.dir/src/fmt/format.cc.obj
kcc.exe: CMakeFiles/kcc.dir/src/fmt/posix.cc.obj
kcc.exe: CMakeFiles/kcc.dir/build.make
kcc.exe: CMakeFiles/kcc.dir/linklibs.rsp
kcc.exe: CMakeFiles/kcc.dir/objects1.rsp
kcc.exe: CMakeFiles/kcc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX executable kcc.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\kcc.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/kcc.dir/build: kcc.exe

.PHONY : CMakeFiles/kcc.dir/build

CMakeFiles/kcc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\kcc.dir\cmake_clean.cmake
.PHONY : CMakeFiles/kcc.dir/clean

CMakeFiles/kcc.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\xiaoc\source\repos\shiinamiyuki\kcc C:\Users\xiaoc\source\repos\shiinamiyuki\kcc C:\Users\xiaoc\source\repos\shiinamiyuki\kcc C:\Users\xiaoc\source\repos\shiinamiyuki\kcc C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\CMakeFiles\kcc.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/kcc.dir/depend
