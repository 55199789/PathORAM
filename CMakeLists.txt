cmake_minimum_required(VERSION 3.16)
project(PathORAM
  VERSION 0.1
  DESCRIPTION "PathORAM Implementation"
  LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O1 -coverage -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -mavx -mavx512f -mavx512cd -mavx512vl -mavx512bw -mavx512dq -mbmi2 -march=native -s")
add_compile_options(-Wno-deprecated-declarations -std=c++20)
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "C++ Standard Required: ${CMAKE_CXX_STANDARD_REQUIRED}")
message(STATUS "C++ Extensions: ${CMAKE_CXX_EXTENSIONS}")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

message("Build type: " ${CMAKE_BUILD_TYPE})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message("Adding Debug flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  message("Adding Release flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
else()
  message(FATAL_ERROR "Unknown build type: " ${CMAKE_BUILD_TYPE})
endif()

include_directories(include)

add_library(common include/external_memory/server/serverBackend.cpp)

file(GLOB LIB_SOURCES "src/*.cpp")
add_library(PathORAMLib ${LIB_SOURCES})
target_link_libraries(PathORAMLib
  crypto
  common)

file(GLOB EXE_SOURCES "app/*.cpp")
add_executable(PathORAM ${EXE_SOURCES})

target_link_libraries(PathORAM PathORAMLib)

# Add benchmarks
find_package(benchmark REQUIRED)
file(GLOB BENCHMARK_FILES "benchmarks/*.cpp")
add_executable(PathORAMBenchmark ${BENCHMARK_FILES})
target_link_libraries(PathORAMBenchmark
  benchmark::benchmark
  PathORAMLib)