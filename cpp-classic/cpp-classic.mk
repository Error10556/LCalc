CLASSIC_GRAMMARHEADERS := \
	cpp-classic/Absyn.H \
	cpp-classic/Buffer.H \
	cpp-classic/ParserError.H \
	cpp-classic/Parser.H \
	cpp-classic/Printer.H \
	cpp-classic/Skeleton.H

CLASSIC_GRAMMARCODE := \
	cpp-classic/Absyn.C \
	cpp-classic/Buffer.C \
	cpp-classic/Printer.C \
	cpp-classic/Skeleton.C \
	cpp-classic/grammar.l \
	cpp-classic/grammar.y

CLASSIC_GRAMMARFILES := $(CLASSIC_GRAMMARHEADERS) $(CLASSIC_GRAMMARCODE)

CLASSIC_GRAMMAROBJ := \
	cpp-classic/Absyn.o \
	cpp-classic/Buffer.o \
	cpp-classic/Printer.o \
	cpp-classic/Skeleton.o \
	cpp-classic/grammar.tab.o \
	cpp-classic/lex.LC.o

$(CLASSIC_GRAMMARFILES) cpp-classic/Test.C &: grammar.cf | cpp-classic
	cd cpp-classic && bnfc -p LC --cpp ../grammar.cf

cpp-classic/lex.LC.c: cpp-classic/grammar.l | cpp-classic
	cd cpp-classic && flex grammar.l

cpp-classic/Bison.H cpp-classic/grammar.tab.c &: cpp-classic/grammar.y \
	| cpp-classic
	cd cpp-classic && bison grammar.y

cpp-classic/grammar.tab.o: cpp-classic/grammar.tab.c | cpp-classic
	$(CXX) $(CXXFLAGS) -c $< -o $@

cpp-classic/lex.LC.o: cpp-classic/lex.LC.c | cpp-classic
	$(CXX) $(CXXFLAGS) -c $< -o $@

cpp-classic/LCTest: $(CLASSIC_GRAMMARHEADERS) $(CLASSIC_GRAMMAROBJ) \
	cpp-classic/Bison.H cpp-classic/Test.o | cpp-classic
	$(CXX) $(CLASSIC_GRAMMAROBJ) cpp-classic/Test.o $(CXXFLAGS) \
		-o cpp-classic/LCTest

cpp-classic/Eval: cpp-classic/Eval.o $(CLASSIC_GRAMMARHEADERS) \
	$(CLASSIC_GRAMMAROBJ) cpp-classic/Bison.H | cpp-classic
	$(CXX) $(CXXFLAGS) $< $(CLASSIC_GRAMMAROBJ) -o cpp-classic/Eval

cpp-classic:
	mkdir cpp-classic

all: \
	cpp-classic/LCTest \
	cpp-classic/Eval \
	| cpp-classic
