CXXFLAGS = -std=c++20 -Wall -Wextra
CXXFLAGS_BISON = $(CXXFLAGS) -Wno-unused-but-set-variable
BISONFLAGS =
OLD_BNFCFLAGS = --cpp-var ../grammar.cf
BNFCFLAGS = $(OLD_BNFCFLAGS)
DEBUGFLAGS = -glldb -fstandalone-debug

all: main maindb

astinst.o: ast.hpp astlib.hpp astinst.cpp
	$(CXX) $(CXXFLAGS) astinst.cpp -c -o astinst.o

astinstdb.o: ast.hpp astlib.hpp astinst.cpp
	clang++ $(CXXFLAGS) astinst.cpp $(DEBUGFLAGS) -c -o astinstdb.o

printer-common.o: printer-common.hpp printer-common.cpp
	$(CXX) $(CXXFLAGS) printer-common.cpp -c -o printer-common.o

printer-commondb.o: printer-common.hpp printer-common.cpp
	clang++ $(CXXFLAGS) printer-common.cpp $(DEBUGFLAGS) -c -o printer-common.o

main: astinst.o printer-common.o astinst.hpp ast.hpp astlib.hpp astlib-debug.hpp astlib-debug.cpp main.cpp
	$(CXX) $(CXXFLAGS) main.cpp astlib-debug.cpp astinst.o printer-common.o -o main

maindb: astinstdb.o printer-commondb.o astinst.hpp ast.hpp astlib.hpp astlib-debug.hpp astlib-debug.cpp main.cpp
	clang++ $(CXXFLAGS) main.cpp astlib-debug.cpp astinstdb.o printer-commondb.o $(DEBUGFLAGS) -o maindb
