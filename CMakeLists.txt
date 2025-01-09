cmake_minimum_required(VERSION 3.22)

project(msweep)

include(CMakeDependentOption)
# Let user decide if they need a compile_commands in the root directory.
cmake_dependent_option(SYMLINK_COMPILE_COMMANDS
  "After generating compile_commands.json, create a symlink in the
project root directory. Only has an effect on debug builds."
  OFF
  "CMAKE_EXPORT_COMPILE_COMMANDS"
  OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(msweep)
set_target_properties(msweep PROPERTIES
        # Make sure to cleanup this file
        ADDITIONAL_CLEAN_FILES compile_commands.json)
# Enable compiler warnings portably
target_compile_options(msweep PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic-errors>
)
# Add source files
add_subdirectory(src)
# Add header file in src to search path for subdirectories
include_directories(src)
# Find and link dependencies
set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)
target_link_libraries(msweep ${CURSES_LIBRARIES})

# Only link compile commands to root dir when debug
add_custom_command(TARGET msweep PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E $<IF:$<BOOL:SYMLINK_COMPILE_COMMANDS>,create_symlink,true>
  ${CMAKE_BINARY_DIR}/compile_commands.json
  ${PROJECT_SOURCE_DIR}/compile_commands.json
  COMMAND_EXPAND_LISTS)

# Use ccache if possible
message(CHECK_START "Looking for ccache")
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  message(CHECK_PASS "found")
  set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
elseif()
  message(CHECK_FAIL "not found")
endif()

