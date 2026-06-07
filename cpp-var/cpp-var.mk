VAR_CXX = clang++
VAR_CXXFLAGS = -glldb -fsanitize=address -fsanitize=leak
VAR_LDFLAGS = -fsanitize=address -fsanitize=leak
VAR_COMPILE = $(VAR_CXX) $(VAR_CXXFLAGS) -c -o

cpp-var/Absyn.hpp cpp-var/Absyn.cpp cpp-var/grammar.l cpp-var/grammar.ypp \
	cpp-var/PatternMatching.hpp cpp-var/SyntaxPrinter.cpp \
	cpp-var/SyntaxPrinter.hpp &: grammar.cf | cpp-var
	cd cpp-var && bnfc --cpp-var -p LC ../grammar.cf

cpp-var/Absyn.o: cpp-var/Absyn.cpp cpp-var/Absyn.hpp | cpp-var
	$(VAR_COMPILE) $@ $<

cpp-var/grammar.lex.cpp: cpp-var/grammar.l | cpp-var
	cd cpp-var && flex grammar.l

cpp-var/grammar.tab.cpp cpp-var/grammar.tab.hpp &: cpp-var/grammar.ypp | cpp-var
	cd cpp-var && bison grammar.ypp

cpp-var/grammar.lex.o: cpp-var/grammar.lex.cpp cpp-var/grammar.tab.hpp | cpp-var
	$(VAR_COMPILE) $@ -Icpp-var $<

cpp-var/grammar.tab.o: cpp-var/grammar.tab.cpp cpp-var/PatternMatching.hpp cpp-var/Absyn.hpp | cpp-var
	$(VAR_COMPILE) $@ -Icpp-var $<

cpp-var/SyntaxPrinter.o: cpp-var/SyntaxPrinter.cpp cpp-var/SyntaxPrinter.hpp | cpp-var
	$(VAR_COMPILE) $@ -Icpp-var $<

cpp-var:
	mkdir cpp-var

all: cpp-var/Absyn.o cpp-var/grammar.lex.o cpp-var/grammar.tab.o \
	cpp-var/SyntaxPrinter.o

.PHONY: clean-var
clean-var:
	@ test ! -f cpp-var/Absyn.o || rm cpp-var/Absyn.o
	@ test ! -f cpp-var/Absyn.cpp || rm cpp-var/Absyn.cpp
	@ test ! -f cpp-var/Absyn.hpp || rm cpp-var/Absyn.hpp
	@ test ! -f cpp-var/grammar.l || rm cpp-var/grammar.l
	@ test ! -f cpp-var/grammar.ypp || rm cpp-var/grammar.ypp
	@ test ! -f cpp-var/PatternMatching.hpp || rm cpp-var/PatternMatching.hpp
	@ test ! -f cpp-var/grammar.lex.cpp || rm cpp-var/grammar.lex.cpp
	@ test ! -f cpp-var/grammar.lex.o || rm cpp-var/grammar.lex.o
	@ test ! -f cpp-var/grammar.tab.cpp || rm cpp-var/grammar.tab.cpp
	@ test ! -f cpp-var/grammar.tab.hpp || rm cpp-var/grammar.tab.hpp
	@ test ! -f cpp-var/grammar.tab.o || rm cpp-var/grammar.tab.o
	@ test ! -f cpp-var/SyntaxPrinter.cpp || rm cpp-var/SyntaxPrinter.cpp
	@ test ! -f cpp-var/SyntaxPrinter.hpp || rm cpp-var/SyntaxPrinter.hpp
	@ test ! -f cpp-var/SyntaxPrinter.o || rm cpp-var/SyntaxPrinter.o
