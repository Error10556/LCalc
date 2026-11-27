cmake_minimum_required(VERSION 4.0.0)

set(CACHE{CMAKE_CXX_FLAGS_DEBUG} TYPE STRING VALUE "-glldb -fstandalone-debug -O0")
set(CACHE{CMAKE_CXX_FLAGS_COVERAGE} TYPE STRING VALUE "-O0 --coverage")
set(CACHE{CMAKE_CXX_FLAGS_BUILDPROFILE} TYPE STRING VALUE "-O3 -ftime-report -ftime-report-details")
set(CACHE{CMAKE_CXX_FLAGS_DEVSETUP} TYPE STRING VALUE "-Werror")

project(LCalc_SOA LANGUAGES CXX)

if (${CMAKE_BUILD_TYPE} STREQUAL "DevSetup")
    set(CACHE{CMAKE_EXPORT_COMPILE_COMMANDS} TYPE BOOL FORCE VALUE ON)
endif()


add_library(lang_options INTERFACE)
target_compile_options(lang_options INTERFACE -Wall -Wextra)
target_compile_features(lang_options INTERFACE cxx_std_20)

add_executable(main main.cpp printer-common.cpp classic-pretty-printer.cpp astlib-debug.cpp)
add_subdirectory(astinst)
if (NOT ${CMAKE_BUILD_TYPE} STREQUAL "DevSetup")
    target_precompile_headers(main PRIVATE astlib.hpp ast.hpp)
endif()
target_link_libraries(main PRIVATE lang_options)
