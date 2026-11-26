add_library(libastinst OBJECT astinst.cpp astlib-debug-inst.cpp)
target_link_libraries(libastinst PRIVATE lang_options)
target_include_directories(libastinst INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}" PUBLIC ..)
target_precompile_headers(libastinst PRIVATE ../astlib.hpp ../ast.hpp)
