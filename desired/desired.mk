DESIRED_CXX = clang++
DESIRED_CXXFLAGS = -glldb -fsanitize=address -fsanitize=leak
DESIRED_LDFLAGS = -fsanitize=address -fsanitize=leak

desired/grammar.lex.cpp: desired/grammar.l | desired
	cd desired && flex grammar.l

desired/grammar.tab.hpp desired/grammar.tab.cpp &: desired/grammar.ypp \
	| desired
	cd desired && bison grammar.ypp

desired/Absyn.o: desired/Absyn.cpp desired/Absyn.hpp | desired
	$(DESIRED_CXX) $(CXXFLAGS) $(DESIRED_CXXFLAGS) -c -o $@ $<

desired/grammar.tab.o: desired/grammar.tab.cpp desired/Absyn.hpp | desired
	$(DESIRED_CXX) $(CXXFLAGS) $(DESIRED_CXXFLAGS) -c -o $@ $<

desired/grammar.lex.o: desired/grammar.lex.cpp | desired
	$(DESIRED_CXX) $(CXXFLAGS) $(DESIRED_CXXFLAGS) -c -o $@ $<

desired/PrettyPrinter.o: desired/PrettyPrinter.cpp desired/PrettyPrinter.hpp \
	desired/Absyn.hpp
	$(DESIRED_CXX) $(CXXFLAGS) $(DESIRED_CXXFLAGS) -c -o $@ $<

desired/SyntaxPrinter.o: desired/SyntaxPrinter.cpp desired/SyntaxPrinter.hpp \
	desired/Absyn.hpp
	$(DESIRED_CXX) $(CXXFLAGS) $(DESIRED_CXXFLAGS) -c -o $@ $<

desired/Test.o: desired/Test.cpp desired/Absyn.hpp desired/grammar.tab.hpp \
	desired/PrettyPrinter.hpp desired/SyntaxPrinter.hpp | desired
	$(DESIRED_CXX) $(CXXFLAGS) $(DESIRED_CXXFLAGS) -c -o $@ $<

desired/Test: desired/Test.o desired/grammar.tab.o desired/PrettyPrinter.o \
	desired/SyntaxPrinter.o desired/grammar.lex.o desired/Absyn.o | desired
	cd desired && $(DESIRED_CXX) $(LDFLAGS) $(DESIRED_LDFLAGS) Test.o \
		grammar.lex.o grammar.tab.o PrettyPrinter.o SyntaxPrinter.o \
		Absyn.o -o Test

desired/Eval.o: desired/Eval.cpp desired/PrettyPrinter.hpp \
	desired/grammar.tab.hpp desired/Absyn.hpp | desired
	$(DESIRED_CXX) $(CXXFLAGS) $(DESIRED_CXXFLAGS) -c -o $@ $<

desired/Eval: desired/Eval.o desired/grammar.tab.o desired/PrettyPrinter.o \
	desired/grammar.lex.o desired/Absyn.o | desired
	cd desired && $(DESIRED_CXX) $(LDFLAGS) $(DESIRED_LDFLAGS) Eval.o \
		grammar.lex.o grammar.tab.o PrettyPrinter.o Absyn.o -o Eval

desired:
	mkdir desired

all: \
	desired/Test \
	desired/Eval
