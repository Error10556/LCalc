CXXFLAGS = -std=c++20 -Wall -Wextra
CXXFLAGS_BISON = $(CXXFLAGS) -Wno-unused-but-set-variable
BISONFLAGS =
OLD_BNFCFLAGS = --cpp-var ../grammar.cf
BNFCFLAGS = $(OLD_BNFCFLAGS)

astinst.o: ast.hpp astlib.hpp astinst.cpp
	$(CXX) $(CXXFLAGS) astinst.cpp -c -o astinst.o

astinstdb.o: ast.hpp astlib.hpp astinst.cpp
	clang++ $(CXXFLAGS) astinst.cpp -glldb -fstandalone-debug -c -o astinstdb.o

main: astinst.o astinst.hpp ast.hpp astlib.hpp astlib-debug.hpp astlib-debug.cpp main.cpp
	$(CXX) $(CXXFLAGS) main.cpp astlib-debug.cpp astinst.o -o main

maindb: astinstdb.o astinst.hpp ast.hpp astlib.hpp astlib-debug.hpp astlib-debug.cpp main.cpp
	clang++ $(CXXFLAGS) main.cpp astlib-debug.cpp astinstdb.o -glldb -fstandalone-debug -o maindb
