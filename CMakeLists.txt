cmake_minimum_required(VERSION 3.15)
project(RayTracer C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*.c")
add_executable(RayTracer ${SOURCES})

target_include_directories(RayTracer PRIVATE include)

# Link libraries
target_link_libraries(RayTracer PRIVATE m)

# Add warnings
target_compile_options(RayTracer PRIVATE -Wall -Wextra -pedantic -O0 -g)

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Install
install(TARGETS RayTracer DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
add_library(RayLib SHARED ${SOURCES})