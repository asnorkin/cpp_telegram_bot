# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /opt/clion-2017.2.2/bin/cmake/bin/cmake

# The command to remove a file.
RM = /opt/clion-2017.2.2/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/alexaxnder/Developer/DAS/C++/repo/shad-cpp/bot

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alexaxnder/Developer/DAS/C++/repo/shad-cpp/bot/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/bot.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/bot.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bot.dir/flags.make

CMakeFiles/bot.dir/telegram/main.cpp.o: CMakeFiles/bot.dir/flags.make
CMakeFiles/bot.dir/telegram/main.cpp.o: ../telegram/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alexaxnder/Developer/DAS/C++/repo/shad-cpp/bot/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/bot.dir/telegram/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bot.dir/telegram/main.cpp.o -c /home/alexaxnder/Developer/DAS/C++/repo/shad-cpp/bot/telegram/main.cpp

CMakeFiles/bot.dir/telegram/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bot.dir/telegram/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alexaxnder/Developer/DAS/C++/repo/shad-cpp/bot/telegram/main.cpp > CMakeFiles/bot.dir/telegram/main.cpp.i

CMakeFiles/bot.dir/telegram/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bot.dir/telegram/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alexaxnder/Developer/DAS/C++/repo/shad-cpp/bot/telegram/main.cpp -o CMakeFiles/bot.dir/telegram/main.cpp.s

CMakeFiles/bot.dir/telegram/main.cpp.o.requires:

.PHONY : CMakeFiles/bot.dir/telegram/main.cpp.o.requires

CMakeFiles/bot.dir/telegram/main.cpp.o.provides: CMakeFiles/bot.dir/telegram/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/bot.dir/build.make CMakeFiles/bot.dir/telegram/main.cpp.o.provides.build
.PHONY : CMakeFiles/bot.dir/telegram/main.cpp.o.provides

CMakeFiles/bot.dir/telegram/main.cpp.o.provides.build: CMakeFiles/bot.dir/telegram/main.cpp.o


CMakeFiles/bot.dir/telegram/bot.cpp.o: CMakeFiles/bot.dir/flags.make
CMakeFiles/bot.dir/telegram/bot.cpp.o: ../telegram/bot.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alexaxnder/Developer/DAS/C++/repo/shad-cpp/bot/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/bot.dir/telegram/bot.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bot.dir/telegram/bot.cpp.o -c /home/alexaxnder/Developer/DAS/C++/repo/shad-cpp/bot/telegram/bot.cpp

CMakeFiles/bot.dir/telegram/bot.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bot.dir/telegram/bot.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alexaxnder/Developer/DAS/C++/repo/shad-cpp/bot/telegram/bot.cpp > CMakeFiles/bot.dir/telegram/bot.cpp.i

CMakeFiles/bot.dir/telegram/bot.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bot.dir/telegram/bot.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alexaxnder/Developer/DAS/C++/repo/shad-cpp/bot/telegram/bot.cpp -o CMakeFiles/bot.dir/telegram/bot.cpp.s

CMakeFiles/bot.dir/telegram/bot.cpp.o.requires:

.PHONY : CMakeFiles/bot.dir/telegram/bot.cpp.o.requires

CMakeFiles/bot.dir/telegram/bot.cpp.o.provides: CMakeFiles/bot.dir/telegram/bot.cpp.o.requires
	$(MAKE) -f CMakeFiles/bot.dir/build.make CMakeFiles/bot.dir/telegram/bot.cpp.o.provides.build
.PHONY : CMakeFiles/bot.dir/telegram/bot.cpp.o.provides

CMakeFiles/bot.dir/telegram/bot.cpp.o.provides.build: CMakeFiles/bot.dir/telegram/bot.cpp.o


CMakeFiles/bot.dir/telegram/bot_api.cpp.o: CMakeFiles/bot.dir/flags.make
CMakeFiles/bot.dir/telegram/bot_api.cpp.o: ../telegram/bot_api.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alexaxnder/Developer/DAS/C++/repo/shad-cpp/bot/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/bot.dir/telegram/bot_api.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bot.dir/telegram/bot_api.cpp.o -c /home/alexaxnder/Developer/DAS/C++/repo/shad-cpp/bot/telegram/bot_api.cpp

CMakeFiles/bot.dir/telegram/bot_api.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bot.dir/telegram/bot_api.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alexaxnder/Developer/DAS/C++/repo/shad-cpp/bot/telegram/bot_api.cpp > CMakeFiles/bot.dir/telegram/bot_api.cpp.i

CMakeFiles/bot.dir/telegram/bot_api.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bot.dir/telegram/bot_api.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alexaxnder/Developer/DAS/C++/repo/shad-cpp/bot/telegram/bot_api.cpp -o CMakeFiles/bot.dir/telegram/bot_api.cpp.s

CMakeFiles/bot.dir/telegram/bot_api.cpp.o.requires:

.PHONY : CMakeFiles/bot.dir/telegram/bot_api.cpp.o.requires

CMakeFiles/bot.dir/telegram/bot_api.cpp.o.provides: CMakeFiles/bot.dir/telegram/bot_api.cpp.o.requires
	$(MAKE) -f CMakeFiles/bot.dir/build.make CMakeFiles/bot.dir/telegram/bot_api.cpp.o.provides.build
.PHONY : CMakeFiles/bot.dir/telegram/bot_api.cpp.o.provides

CMakeFiles/bot.dir/telegram/bot_api.cpp.o.provides.build: CMakeFiles/bot.dir/telegram/bot_api.cpp.o


# Object files for target bot
bot_OBJECTS = \
"CMakeFiles/bot.dir/telegram/main.cpp.o" \
"CMakeFiles/bot.dir/telegram/bot.cpp.o" \
"CMakeFiles/bot.dir/telegram/bot_api.cpp.o"

# External object files for target bot
bot_EXTERNAL_OBJECTS =

bot: CMakeFiles/bot.dir/telegram/main.cpp.o
bot: CMakeFiles/bot.dir/telegram/bot.cpp.o
bot: CMakeFiles/bot.dir/telegram/bot_api.cpp.o
bot: CMakeFiles/bot.dir/build.make
bot: libtelegram.a
bot: CMakeFiles/bot.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alexaxnder/Developer/DAS/C++/repo/shad-cpp/bot/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable bot"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bot.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bot.dir/build: bot

.PHONY : CMakeFiles/bot.dir/build

CMakeFiles/bot.dir/requires: CMakeFiles/bot.dir/telegram/main.cpp.o.requires
CMakeFiles/bot.dir/requires: CMakeFiles/bot.dir/telegram/bot.cpp.o.requires
CMakeFiles/bot.dir/requires: CMakeFiles/bot.dir/telegram/bot_api.cpp.o.requires

.PHONY : CMakeFiles/bot.dir/requires

CMakeFiles/bot.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bot.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bot.dir/clean

CMakeFiles/bot.dir/depend:
	cd /home/alexaxnder/Developer/DAS/C++/repo/shad-cpp/bot/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alexaxnder/Developer/DAS/C++/repo/shad-cpp/bot /home/alexaxnder/Developer/DAS/C++/repo/shad-cpp/bot /home/alexaxnder/Developer/DAS/C++/repo/shad-cpp/bot/cmake-build-debug /home/alexaxnder/Developer/DAS/C++/repo/shad-cpp/bot/cmake-build-debug /home/alexaxnder/Developer/DAS/C++/repo/shad-cpp/bot/cmake-build-debug/CMakeFiles/bot.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bot.dir/depend

