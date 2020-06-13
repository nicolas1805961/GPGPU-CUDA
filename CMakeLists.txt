cmake_minimum_required (VERSION 3.14)
project (GPGPU CUDA CXX)

option(PROF "Reference build" NO)
include(FetchContent)

FetchContent_Declare(
  GoogleBenchmark
  URL https://github.com/google/benchmark/archive/v1.4.1.tar.gz
  )

FetchContent_Declare(
  CLI11
  URL https://github.com/CLIUtils/CLI11/archive/v1.8.0.tar.gz
  )

FetchContent_Declare(
  spdlog
  URL https://github.com/gabime/spdlog/archive/v1.4.2.tar.gz
  )


if (NOT GoogleBenchmark_POPULATED)
  FetchContent_Populate(GoogleBenchmark)
  set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "From Gtest")
  set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "From Gtest")
  add_subdirectory(${googlebenchmark_SOURCE_DIR} ${googlebenchmark_BINARY_DIR})
endif()

FetchContent_MakeAvailable(spdlog)
FetchContent_MakeAvailable(CLI11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR} )
find_package(PNG REQUIRED)
find_package(CUDA REQUIRED)
find_package(TBB REQUIRED)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(SOURCES
        benchmark/bench.cu
        cpu/src/Graph.cpp
        cpu/src/Image.cpp
        cpu/src/graphcut_cpu.cpp
        gpu/src/Graph_gpu.cu
        gpu/src/graphcut_gpu.cu
)

add_executable(bench ${SOURCES})
target_include_directories(bench PRIVATE cpu/include gpu/include)
target_compile_features(bench PUBLIC cxx_std_14)
target_link_libraries(bench PRIVATE spdlog::spdlog benchmark)
