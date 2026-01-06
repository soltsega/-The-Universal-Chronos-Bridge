cmake_minimum_required(VERSION 3.10)
project(calendar_converter)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create executable
add_executable(calendar_converter ${SOURCES})

# Platform-specific settings
if(WIN32)
    target_compile_definitions(calendar_converter PRIVATE _WIN32)
    target_link_libraries(calendar_converter)
endif()