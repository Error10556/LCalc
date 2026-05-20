cpp-classic/Absyn.C cpp-classic/Absyn.H cpp-classic/Buffer.C cpp-classic/Buffer.H cpp-classic/grammar.l cpp-classic/grammar.y cpp-classic/ParserError.H cpp-classic/Parser.H cpp-classic/Printer.C cpp-classic/Printer.H cpp-classic/Skeleton.C cpp-classic/Skeleton.H cpp-classic/Test.C &: grammar.cf | cpp-classic
	cd cpp-classic && bnfc -p LC --cpp ../grammar.cf

cpp-classic/lex.LC.c: cpp-classic/grammar.l | cpp-classic
	cd cpp-classic && flex grammar.l

cpp-classic/Bison.H cpp-classic/grammar.tab.c &: cpp-classic/grammar.y \
	| cpp-classic
	cd cpp-classic && bison grammar.y

cpp-classic/grammar.tab.o: cpp-classic/grammar.tab.c | cpp-classic
	$(CXX) -c $< -o $@

cpp-classic/lex.LC.o: cpp-classic/lex.LC.c | cpp-classic
	$(CXX) -c $< -o $@

cpp-classic/LCTest: \
	cpp-classic/Absyn.o \
	cpp-classic/Absyn.H \
	cpp-classic/Bison.H \
	cpp-classic/Buffer.o \
	cpp-classic/Buffer.H \
	cpp-classic/grammar.l \
	cpp-classic/grammar.tab.o \
	cpp-classic/lex.LC.o \
	cpp-classic/ParserError.H \
	cpp-classic/Parser.H \
	cpp-classic/Printer.o \
	cpp-classic/Printer.H \
	cpp-classic/Skeleton.o \
	cpp-classic/Skeleton.H \
	cpp-classic/Test.o \
	| cpp-classic
	cd cpp-classic && $(CXX) Absyn.o Buffer.o grammar.tab.o lex.LC.o \
		Printer.o Skeleton.o Test.o -o LCTest

cpp-classic:
	mkdir cpp-classic
