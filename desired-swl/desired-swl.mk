DESIRED_SWL_CXX = clang++
DESIRED_SWL_CXXFLAGS = -std=c++20 -Wall -Wextra -Wno-unused-but-set-variable -fsanitize=address -fsanitize=leak -fstandalone-debug -glldb
DESIRED_SWL_LDFLAGS = -fsanitize=address -fsanitize=leak -fstandalone-debug -glldb

all: desired-swl/Testgrammar desired-swl/Eval

desired-swl/Testgrammar desired-swl/libgrammarParser.a desired-swl/libgrammarPrinter.a desired-swl/grammar.tab.hpp &:
	cd desired-swl && $(MAKE) CXX=$(DESIRED_SWL_CXX) CXXFLAGS='$(DESIRED_SWL_CXXFLAGS)' LDFLAGS='$(DESIRED_SWL_LDFLAGS)' all

desired-swl/Eval: desired-swl/Eval.o desired-swl/libgrammarParser.a desired-swl/libgrammarPrinter.a
	cd desired-swl && $(DESIRED_SWL_CXX) $(DESIRED_SWL_LDFLAGS) Eval.o libgrammarParser.a libgrammarPrinter.a -o Eval

desired-swl/Eval.o: desired-swl/Eval.cpp \
	desired-swl/Absyn.hpp \
	desired-swl/grammar.tab.hpp \
	desired-swl/ContextFreePrettyPrinter.hpp \
	desired-swl/PatternMatching.hpp \
	desired-swl/variant.hpp \
	desired-swl/variant_detail.hpp \
	desired-swl/variant_visit.hpp
	cd desired-swl && $(DESIRED_SWL_CXX) $(DESIRED_SWL_CXXFLAGS) -c -o Eval.o Eval.cpp
