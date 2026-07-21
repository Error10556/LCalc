VAR_STARTS_CXX = clang++
VAR_STARTS_CXXFLAGS = -std=c++17 -glldb -fsanitize=address -fsanitize=leak -Wall -Werror -Wextra
VAR_STARTS_LDFLAGS = -fsanitize=address -fsanitize=leak

all: cpp-var-starts/Testgrammar

cpp-var-starts/Testgrammar: cpp-var-starts/Makefile
	$(MAKE) -C cpp-var-starts CXX=$(VAR_STARTS_CXX) CXXFLAGS='$(VAR_STARTS_CXXFLAGS)' LDFLAGS='$(VAR_STARTS_LDFLAGS)' all

cpp-var-starts/Makefile: grammar.cf
	cd cpp-var-starts && bnfc --cpp-var -p LC -m ../grammar.cf --positions=start

clean-var-starts:
	$(MAKE) -C cpp-var-starts distclean
