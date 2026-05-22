desired/grammar.lex.cpp: desired/grammar.l | desired
	cd desired && flex grammar.l

desired/grammar.tab.hpp desired/grammar.tab.cpp &: desired/grammar.ypp \
	| desired
	cd desired && bison grammar.ypp

desired:
	mkdir desired

desired/Test: desired/Test.o desired/grammar.tab.o \
	desired/grammar.lex.o desired/Absyn.o | desired
	cd desired && $(CXX) $(CXXFLAGS) Test.o grammar.lex.o grammar.tab.o \
		Absyn.o -o Test

all: \
	desired/Test
