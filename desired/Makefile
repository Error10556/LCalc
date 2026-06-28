CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-unused-but-set-variable

OBJECTS = Absyn.o \
	PrettyPrinter.o \
	PrinterCommon.o \
	SyntaxPrinter.o \
	Test.o \
	grammar.lex.o \
	grammar.tab.o

ARCHIVES = libgrammarParser.a libgrammarPrinter.a

BNFC_GENERATED = Absyn.cpp \
	Absyn.hpp \
	PatternMatching.hpp \
	PrettyPrinter.cpp \
	PrettyPrinter.hpp \
	PrinterCommon.cpp \
	PrinterCommon.hpp \
	SyntaxPrinter.cpp \
	SyntaxPrinter.hpp \
	Test.cpp \
	grammar.l \
	grammar.ypp

define HELPMESSAGE
Phony targets:

TARGET      | DESCRIPTION
------------|------------------------------------------------------------------
help        | Print this message.
default     | Build static libraries for the parser and the printers.
all         | Build the static libraries and the "Test" program.
mostlyclean | Delete all object/executable files.
clean       | Delete all files created by this Makefile and BNFC backups.

File targets:

TARGET               | DESCRIPTION
---------------------|---------------------------------------------------------
Test                 | A program that performs parsing and prints the AST.
libgrammarParser.a   | Implements the syntax tree, lexing, and parsing.
libgrammarPrinter.a  | Implements pretty-, syntax-tree-, and linear-printing.
grammar.lex.cpp      | Contains a lexer implementation.
grammar.tab.{h,c}pp  | Contain a syntax parser implementation.

For any *.cpp file, a corresponding *.o file can be built.
endef

.PHONY: all clean mostlyclean default help

default: libgrammarParser.a libgrammarPrinter.a

help:
	$(file >/dev/stdout,$(HELPMESSAGE))
	@:

all: $(ARCHIVES) Test

mostlyclean:
	@for f in $(OBJECTS) $(ARCHIVES) Test; do test ! -f $$f || rm $$f; done

clean: mostlyclean
	@ for f in grammar.lex.cpp grammar.tab.cpp grammar.tab.hpp; do \
		test ! -f $$f || rm $$f; \
	done
	@ for f in $(BNFC_GENERATED); do \
		test ! -f $$f.bak || rm $$f.bak; \
	done

grammar.lex.cpp: grammar.l
	flex grammar.l

grammar.tab.hpp grammar.tab.cpp &: grammar.ypp
	bison grammar.ypp

CXX_COMPILE = $(CXX) $(CXXFLAGS) -c -o $@ $<
Absyn.o: Absyn.cpp Absyn.hpp
	$(CXX_COMPILE)

grammar.tab.o: grammar.tab.cpp grammar.tab.hpp Absyn.hpp PatternMatching.hpp
	$(CXX_COMPILE)

grammar.lex.o: grammar.lex.cpp grammar.tab.hpp
	$(CXX_COMPILE)

PrinterCommon.o: PrinterCommon.cpp PrinterCommon.hpp
	$(CXX_COMPILE)

PrettyPrinter.o: PrettyPrinter.cpp PrettyPrinter.hpp \
	Absyn.hpp PrinterCommon.hpp
	$(CXX_COMPILE)

SyntaxPrinter.o: SyntaxPrinter.cpp SyntaxPrinter.hpp \
	Absyn.hpp PrinterCommon.hpp
	$(CXX_COMPILE)

AR_COMPILE = $(AR) $(ARFLAGS) $@ $^
libgrammarParser.a: Absyn.o grammar.lex.o grammar.tab.o
	$(AR_COMPILE)

libgrammarPrinter.a: PrinterCommon.o PrettyPrinter.o SyntaxPrinter.o
	$(AR_COMPILE)

Test.o: Test.cpp Absyn.hpp grammar.tab.hpp \
	PrettyPrinter.hpp SyntaxPrinter.hpp PatternMatching.hpp
	$(CXX_COMPILE)

Test: Test.o libgrammarParser.a libgrammarPrinter.a
	$(CXX) $(LDFLAGS) $^ -o Test
