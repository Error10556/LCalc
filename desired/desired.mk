desired/grammar.lex.cpp: desired/grammar.l | desired
	cd desired && flex grammar.l

desired/grammar.tab.hpp desired/grammar.tab.cpp &: desired/grammar.ypp \
	| desired
	cd desired && bison grammar.ypp

desired/Absyn.o: desired/Absyn.cpp desired/Absyn.hpp | desired
	$(CXX) $(CXXFLAGS) -c -o $@ $<

desired/grammar.tab.o: desired/grammar.tab.cpp desired/Absyn.hpp | desired
	$(CXX) $(CXXFLAGS) -c -o $@ $<

desired/grammar.lex.o: desired/grammar.lex.cpp | desired
	$(CXX) $(CXXFLAGS) -c -o $@ $<

desired/Test.o: desired/Test.cpp desired/Absyn.hpp desired/grammar.tab.hpp \
	| desired
	$(CXX) $(CXXFLAGS) -c -o $@ $<

desired/Test: desired/Test.o desired/grammar.tab.o \
	desired/grammar.lex.o desired/Absyn.o | desired
	cd desired && $(CXX) $(LDFLAGS) Test.o grammar.lex.o grammar.tab.o \
		Absyn.o -o Test

desired:
	mkdir desired

all: \
	desired/Test
