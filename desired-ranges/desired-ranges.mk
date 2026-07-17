DESIRED_CXX = clang++
DESIRED_CXXFLAGS = -std=c++17 -Wall -Wextra -fsanitize=address -fsanitize=leak -fstandalone-debug -glldb
DESIRED_LDFLAGS = -fsanitize=address -fsanitize=leak -fstandalone-debug -glldb

all: desired-ranges/Testgrammar desired-ranges/Eval

desired-ranges/Testgrammar desired-ranges/libgrammarParser.a desired-ranges/libgrammarPrinter.a desired-ranges/grammar.tab.hpp &:
	cd desired-ranges && $(MAKE) CXX=$(DESIRED_CXX) CXXFLAGS='$(DESIRED_CXXFLAGS)' LDFLAGS='$(DESIRED_LDFLAGS)' all

desired-ranges/Eval: desired-ranges/Eval.o desired-ranges/libgrammarParser.a desired-ranges/libgrammarPrinter.a
	cd desired-ranges && $(DESIRED_CXX) $(DESIRED_LDFLAGS) Eval.o libgrammarParser.a libgrammarPrinter.a -o Eval

desired-ranges/Eval.o: desired-ranges/Eval.cpp \
	desired-ranges/Absyn.hpp \
	desired-ranges/grammar.tab.hpp \
	desired-ranges/ContextFreePrettyPrinter.hpp \
	desired-ranges/PatternMatching.hpp
	cd desired-ranges && $(DESIRED_CXX) $(DESIRED_CXXFLAGS) -c -o Eval.o Eval.cpp
