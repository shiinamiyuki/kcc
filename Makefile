# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = /home/shiinamiyuki/Documents/kcc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/shiinamiyuki/Documents/kcc

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/shiinamiyuki/Documents/kcc/CMakeFiles /home/shiinamiyuki/Documents/kcc/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/shiinamiyuki/Documents/kcc/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named kcc

# Build rule for target.
kcc: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 kcc
.PHONY : kcc

# fast build rule for target.
kcc/fast:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/build
.PHONY : kcc/fast

src/ast.o: src/ast.cc.o

.PHONY : src/ast.o

# target to build an object file
src/ast.cc.o:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/ast.cc.o
.PHONY : src/ast.cc.o

src/ast.i: src/ast.cc.i

.PHONY : src/ast.i

# target to preprocess a source file
src/ast.cc.i:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/ast.cc.i
.PHONY : src/ast.cc.i

src/ast.s: src/ast.cc.s

.PHONY : src/ast.s

# target to generate assembly for a file
src/ast.cc.s:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/ast.cc.s
.PHONY : src/ast.cc.s

src/compile.o: src/compile.cc.o

.PHONY : src/compile.o

# target to build an object file
src/compile.cc.o:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/compile.cc.o
.PHONY : src/compile.cc.o

src/compile.i: src/compile.cc.i

.PHONY : src/compile.i

# target to preprocess a source file
src/compile.cc.i:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/compile.cc.i
.PHONY : src/compile.cc.i

src/compile.s: src/compile.cc.s

.PHONY : src/compile.s

# target to generate assembly for a file
src/compile.cc.s:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/compile.cc.s
.PHONY : src/compile.cc.s

src/cpp.o: src/cpp.cc.o

.PHONY : src/cpp.o

# target to build an object file
src/cpp.cc.o:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/cpp.cc.o
.PHONY : src/cpp.cc.o

src/cpp.i: src/cpp.cc.i

.PHONY : src/cpp.i

# target to preprocess a source file
src/cpp.cc.i:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/cpp.cc.i
.PHONY : src/cpp.cc.i

src/cpp.s: src/cpp.cc.s

.PHONY : src/cpp.s

# target to generate assembly for a file
src/cpp.cc.s:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/cpp.cc.s
.PHONY : src/cpp.cc.s

src/fmt/format.o: src/fmt/format.cc.o

.PHONY : src/fmt/format.o

# target to build an object file
src/fmt/format.cc.o:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/fmt/format.cc.o
.PHONY : src/fmt/format.cc.o

src/fmt/format.i: src/fmt/format.cc.i

.PHONY : src/fmt/format.i

# target to preprocess a source file
src/fmt/format.cc.i:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/fmt/format.cc.i
.PHONY : src/fmt/format.cc.i

src/fmt/format.s: src/fmt/format.cc.s

.PHONY : src/fmt/format.s

# target to generate assembly for a file
src/fmt/format.cc.s:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/fmt/format.cc.s
.PHONY : src/fmt/format.cc.s

src/fmt/posix.o: src/fmt/posix.cc.o

.PHONY : src/fmt/posix.o

# target to build an object file
src/fmt/posix.cc.o:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/fmt/posix.cc.o
.PHONY : src/fmt/posix.cc.o

src/fmt/posix.i: src/fmt/posix.cc.i

.PHONY : src/fmt/posix.i

# target to preprocess a source file
src/fmt/posix.cc.i:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/fmt/posix.cc.i
.PHONY : src/fmt/posix.cc.i

src/fmt/posix.s: src/fmt/posix.cc.s

.PHONY : src/fmt/posix.s

# target to generate assembly for a file
src/fmt/posix.cc.s:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/fmt/posix.cc.s
.PHONY : src/fmt/posix.cc.s

src/lex.o: src/lex.cc.o

.PHONY : src/lex.o

# target to build an object file
src/lex.cc.o:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/lex.cc.o
.PHONY : src/lex.cc.o

src/lex.i: src/lex.cc.i

.PHONY : src/lex.i

# target to preprocess a source file
src/lex.cc.i:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/lex.cc.i
.PHONY : src/lex.cc.i

src/lex.s: src/lex.cc.s

.PHONY : src/lex.s

# target to generate assembly for a file
src/lex.cc.s:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/lex.cc.s
.PHONY : src/lex.cc.s

src/main.o: src/main.cc.o

.PHONY : src/main.o

# target to build an object file
src/main.cc.o:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/main.cc.o
.PHONY : src/main.cc.o

src/main.i: src/main.cc.i

.PHONY : src/main.i

# target to preprocess a source file
src/main.cc.i:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/main.cc.i
.PHONY : src/main.cc.i

src/main.s: src/main.cc.s

.PHONY : src/main.s

# target to generate assembly for a file
src/main.cc.s:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/main.cc.s
.PHONY : src/main.cc.s

src/parse.o: src/parse.cc.o

.PHONY : src/parse.o

# target to build an object file
src/parse.cc.o:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/parse.cc.o
.PHONY : src/parse.cc.o

src/parse.i: src/parse.cc.i

.PHONY : src/parse.i

# target to preprocess a source file
src/parse.cc.i:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/parse.cc.i
.PHONY : src/parse.cc.i

src/parse.s: src/parse.cc.s

.PHONY : src/parse.s

# target to generate assembly for a file
src/parse.cc.s:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/parse.cc.s
.PHONY : src/parse.cc.s

src/sema.o: src/sema.cc.o

.PHONY : src/sema.o

# target to build an object file
src/sema.cc.o:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/sema.cc.o
.PHONY : src/sema.cc.o

src/sema.i: src/sema.cc.i

.PHONY : src/sema.i

# target to preprocess a source file
src/sema.cc.i:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/sema.cc.i
.PHONY : src/sema.cc.i

src/sema.s: src/sema.cc.s

.PHONY : src/sema.s

# target to generate assembly for a file
src/sema.cc.s:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/sema.cc.s
.PHONY : src/sema.cc.s

src/type.o: src/type.cc.o

.PHONY : src/type.o

# target to build an object file
src/type.cc.o:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/type.cc.o
.PHONY : src/type.cc.o

src/type.i: src/type.cc.i

.PHONY : src/type.i

# target to preprocess a source file
src/type.cc.i:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/type.cc.i
.PHONY : src/type.cc.i

src/type.s: src/type.cc.s

.PHONY : src/type.s

# target to generate assembly for a file
src/type.cc.s:
	$(MAKE) -f CMakeFiles/kcc.dir/build.make CMakeFiles/kcc.dir/src/type.cc.s
.PHONY : src/type.cc.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... kcc"
	@echo "... edit_cache"
	@echo "... src/ast.o"
	@echo "... src/ast.i"
	@echo "... src/ast.s"
	@echo "... src/compile.o"
	@echo "... src/compile.i"
	@echo "... src/compile.s"
	@echo "... src/cpp.o"
	@echo "... src/cpp.i"
	@echo "... src/cpp.s"
	@echo "... src/fmt/format.o"
	@echo "... src/fmt/format.i"
	@echo "... src/fmt/format.s"
	@echo "... src/fmt/posix.o"
	@echo "... src/fmt/posix.i"
	@echo "... src/fmt/posix.s"
	@echo "... src/lex.o"
	@echo "... src/lex.i"
	@echo "... src/lex.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
	@echo "... src/parse.o"
	@echo "... src/parse.i"
	@echo "... src/parse.s"
	@echo "... src/sema.o"
	@echo "... src/sema.i"
	@echo "... src/sema.s"
	@echo "... src/type.o"
	@echo "... src/type.i"
	@echo "... src/type.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

