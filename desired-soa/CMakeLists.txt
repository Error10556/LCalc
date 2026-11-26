cmake_minimum_required(VERSION 4.0.0)

set(CACHE{CMAKE_CXX_FLAGS_DEBUG} TYPE STRING VALUE "-glldb -fstandalone-debug -O0")
set(CACHE{CMAKE_CXX_FLAGS_COVERAGE} TYPE STRING VALUE "-O0 --coverage")
set(CACHE{CMAKE_CXX_FLAGS_BUILDPROFILE} TYPE STRING VALUE "-O3  -ftime-report -ftime-report-details")

set(CACHE{CMAKE_EXPORT_COMPILE_COMMANDS} TYPE BOOL FORCE VALUE ON)

project(LCalc_SOA LANGUAGES CXX)

add_library(lang_options INTERFACE)
target_compile_options(lang_options INTERFACE -Wall -Wextra)
target_compile_features(lang_options INTERFACE cxx_std_20)

add_subdirectory(astinst)

add_executable(main main.cpp printer-common.cpp astlib-debug.cpp)
target_precompile_headers(main REUSE_FROM libastinst)
target_link_libraries(main PRIVATE libastinst lang_options)
