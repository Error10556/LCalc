VARSWL_CXX = clang++
VARSWL_CXXFLAGS = -std=c++20 -glldb -fsanitize=address -fsanitize=leak -Wall -Werror -Wextra
VARSWL_LDFLAGS = -fsanitize=address -fsanitize=leak

all: cpp-var-swl/Testgrammar

cpp-var-swl/Testgrammar: cpp-var-swl/Makefile
	$(MAKE) -C cpp-var-swl CXX=$(VARSWL_CXX) CXXFLAGS='$(VARSWL_CXXFLAGS)' LDFLAGS='$(VARSWL_LDFLAGS)' all

cpp-var-swl/Makefile: grammar.cf
	bnfc --cpp-var --variants-from=swl -p LC -o cpp-var-swl -m grammar.cf

clean-var-swl:
	$(MAKE) -C cpp-var-swl distclean
