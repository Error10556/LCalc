VAR_CXX = clang++
VAR_CXXFLAGS = -glldb -fsanitize=address -fsanitize=leak
VAR_LDFLAGS = -fsanitize=address -fsanitize=leak
VAR_COMPILE = $(VAR_CXX) $(VAR_CXXFLAGS) -c -o

cpp-var/Absyn.hpp cpp-var/Absyn.cpp cpp-var/grammar.l cpp-var/grammar.ypp &: grammar.cf | cpp-var
	cd cpp-var && bnfc --cpp-var -p LC ../grammar.cf

cpp-var/Absyn.o: cpp-var/Absyn.cpp cpp-var/Absyn.hpp | cpp-var
	$(VAR_COMPILE) $@ $<

cpp-var:
	mkdir cpp-var

all: cpp-var/Absyn.o

.PHONY: clean-var
clean-var:
	test ! -f cpp-var/Absyn.o || rm cpp-var/Absyn.o
	test ! -f cpp-var/Absyn.cpp || rm cpp-var/Absyn.cpp
	test ! -f cpp-var/Absyn.hpp || rm cpp-var/Absyn.hpp
	test ! -f cpp-var/grammar.l || rm cpp-var/grammar.l
	test ! -f cpp-var/grammar.ypp || rm cpp-var/grammar.ypp
