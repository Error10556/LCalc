CXXFLAGS = -std=c++20 -Wall -Wextra
CXXFLAGS_BISON = $(CXXFLAGS) -Wno-unused-but-set-variable
BISONFLAGS =
OLD_BNFCFLAGS = --cpp-var ../grammar.cf
BNFCFLAGS = $(OLD_BNFCFLAGS)

astinst.o: ast.hpp astlib.hpp astinst.cpp
	$(CXX) $(CXXFLAGS) astinst.cpp -c -o astinst.o

main: astinst.o astinst.hpp ast.hpp astlib.hpp main.cpp
	$(CXX) $(CXXFLAGS) main.cpp astinst.o -o main
