DESIRED_CXX = clang++
DESIRED_CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-unused-but-set-variable -fsanitize=address -fsanitize=leak -fstandalone-debug -glldb
DESIRED_LDFLAGS = -fsanitize=address -fsanitize=leak -fstandalone-debug -glldb

all: desired/Test desired/Eval

desired/Test desired/libgrammarParser.a desired/libgrammarPrinter.a desired/grammar.tab.hpp &:
	cd desired && $(MAKE) CXX=$(DESIRED_CXX) CXXFLAGS='$(DESIRED_CXXFLAGS)' LDFLAGS='$(DESIRED_LDFLAGS)' all

desired/Eval: desired/Eval.o desired/libgrammarParser.a desired/libgrammarPrinter.a
	cd desired && $(DESIRED_CXX) $(DESIRED_LDFLAGS) Eval.o libgrammarParser.a libgrammarPrinter.a -o Eval

desired/Eval.o: desired/Eval.cpp \
	desired/Absyn.hpp \
	desired/grammar.tab.hpp \
	desired/PrettyPrinter.hpp \
	desired/PatternMatching.hpp
	cd desired && $(DESIRED_CXX) $(DESIRED_CXXFLAGS) -c -o Eval.o Eval.cpp
