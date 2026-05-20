include cpp-classic/cpp-classic.mk

.PHONY: all
.DEFAULT_GOAL := all

all: \
	cpp-classic/Absyn.C \
	cpp-classic/Absyn.H \
	cpp-classic/Bison.H \
	cpp-classic/Buffer.C \
	cpp-classic/Buffer.H \
	cpp-classic/grammar.l \
	cpp-classic/grammar.tab.c \
	cpp-classic/grammar.y \
	cpp-classic/lex.LC.c \
	cpp-classic/ParserError.H \
	cpp-classic/Parser.H \
	cpp-classic/Printer.C \
	cpp-classic/Printer.H \
	cpp-classic/Skeleton.C \
	cpp-classic/Skeleton.H \
	cpp-classic/Test.C \
	cpp-classic/LCTest \
	| cpp-classic
