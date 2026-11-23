cmake_minimum_required(VERSION 4.0.0)

set(CACHE{CMAKE_CXX_FLAGS_DEBUG} TYPE STRING VALUE "-glldb -fstandalone-debug -O0")
set(CACHE{CMAKE_CXX_FLAGS_COVERAGE} TYPE STRING VALUE "-O0 --coverage")

project(LCalc_SOA LANGUAGES CXX)

add_executable(main main.cpp printer-common.cpp astinst.cpp astlib-debug.cpp)
target_compile_options(main PRIVATE -Wall -Wextra)
target_compile_features(main PRIVATE cxx_std_20)
