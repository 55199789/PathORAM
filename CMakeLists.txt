cmake_minimum_required(VERSION 3.16)
project(PathORAM
  VERSION 0.1
  DESCRIPTION "PathORAM Implementation"
  LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O1 -coverage -fprofile-arcs")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -mavx -mavx512f -mavx512cd -mavx512vl -mavx512bw -mavx512dq -mbmi2 -march=native -DPARALLEL -s -fopenmp")
add_compile_options(-Wno-deprecated-declarations)
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

SET(GCC_COVERAGE_LINK_FLAGS "-coverage -lgcov")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

find_package(TBB REQUIRED)
if(TBB_FOUND)
  message("TBB found")
else()
  message(FATAL_ERROR "TBB not found")
endif()

find_package(OpenMP)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(NLopt REQUIRED)

include_directories(include)

file(GLOB LIB_SOURCES "src/*.cpp")
add_library(PathORAMLib ${LIB_SOURCES})
target_link_libraries(PathORAMLib
  TBB::tbb
  Threads::Threads
  OpenMP::OpenMP_CXX
  crypto
  nlopt)

file(GLOB EXE_SOURCES "app/*.cpp")
add_executable(PathORAM ${EXE_SOURCES})

target_link_libraries(PathORAM PathORAMLib)
