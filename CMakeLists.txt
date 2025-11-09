cmake_minimum_required(VERSION 3.15)
project(RayTracer C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB SOURCES "src/*.c")
add_executable(RayTracer ${SOURCES})

target_include_directories(RayTracer PRIVATE include)

# Link libraries
target_link_libraries(RayTracer PRIVATE m)
set(CMAKE_BUILD_TYPE Debug)

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

enable_testing()

add_executable(
  RayTest
  tests/testRay.cpp
)
target_link_libraries(
  RayTest
  GTest::gtest_main
  RayLib
)

include(GoogleTest)
gtest_discover_tests(RayTest)
