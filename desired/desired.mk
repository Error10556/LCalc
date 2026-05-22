desired/grammar.lex.cpp: desired/grammar.l | desired
	cd desired && flex grammar.l

desired/grammar.tab.hpp desired/grammar.tab.cpp &: desired/grammar.ypp \
	| desired
	cd desired && bison grammar.ypp

desired:
	mkdir desired

desired/Test: desired/Test.cpp desired/grammar.tab.cpp \
	desired/grammar.lex.cpp desired/Absyn.cpp | desired
	cd desired && $(CXX) $(CXXFLAGS) Test.cpp grammar.lex.cpp grammar.tab.cpp \
		Absyn.cpp -o Test

all: \
	desired/Test
