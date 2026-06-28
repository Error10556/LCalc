VAR_CXX = clang++
VAR_CXXFLAGS = -glldb -fsanitize=address -fsanitize=leak -Wall -Werror -Wno-unused-but-set-variable
VAR_LDFLAGS = -fsanitize=address -fsanitize=leak

all: cpp-var/Test

cpp-var/Test: cpp-var/Makefile
	$(MAKE) -C cpp-var $(MAKE) CXX=$(VAR_CXX) CXXFLAGS='$(VAR_CXXFLAGS)' LDFLAGS='$(VAR_LDFLAGS)' all

cpp-var/Makefile:
	bnfc --cpp-var -p LC -o cpp-var -m grammar.cf

clean-var:
	@ while read line; do \
		for file in cpp-var/$${line#/}; do \
			test ! -f $$file || rm $$file; \
		done \
	done <cpp-var/.gitignore
