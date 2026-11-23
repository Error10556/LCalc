set(CACHE{CMAKE_CXX_FLAGS_DEBUG} -glldb -fstandalone-debug -O0)

cmake_minimum_required(VERSION 4.0.0)
project(LCalc_SOA LANGUAGES CXX)

add_executable(main main.cpp printer-common.cpp astinst.cpp)
target_compile_options(main -Wall -Wextra)
