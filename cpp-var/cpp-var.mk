VAR_CXX = clang++
VAR_CXXFLAGS = -std=c++17 -glldb -fsanitize=address -fsanitize=leak -Wall -Werror -Wextra
VAR_LDFLAGS = -fsanitize=address -fsanitize=leak

all: cpp-var/Testgrammar

cpp-var/Testgrammar: cpp-var/Makefile
	$(MAKE) -C cpp-var CXX=$(VAR_CXX) CXXFLAGS='$(VAR_CXXFLAGS)' LDFLAGS='$(VAR_LDFLAGS)' all

cpp-var/Makefile: grammar.cf
	cd cpp-var && bnfc --cpp-var -p LC -m ../grammar.cf

clean-var:
	$(MAKE) -C cpp-var distclean
