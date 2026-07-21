VAR_RANGES_CXX = clang++
VAR_RANGES_CXXFLAGS = -std=c++17 -glldb -fsanitize=address -fsanitize=leak -Wall -Werror -Wextra
VAR_RANGES_LDFLAGS = -fsanitize=address -fsanitize=leak

all: cpp-var-ranges/Testgrammar

cpp-var-ranges/Testgrammar: cpp-var-ranges/Makefile
	$(MAKE) -C cpp-var-ranges CXX=$(VAR_RANGES_CXX) CXXFLAGS='$(VAR_RANGES_CXXFLAGS)' LDFLAGS='$(VAR_RANGES_LDFLAGS)' all

cpp-var-ranges/Makefile: grammar.cf
	cd cpp-var-ranges && bnfc --cpp-var -p LC -m ../grammar.cf --positions=range

clean-var-ranges:
	$(MAKE) -C cpp-var-ranges distclean
