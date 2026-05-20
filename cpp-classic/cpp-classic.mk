cpp-classic/Absyn.C cpp-classic/Absyn.H cpp-classic/Buffer.C cpp-classic/Buffer.H cpp-classic/grammar.l cpp-classic/grammar.y cpp-classic/ParserError.H cpp-classic/Parser.H cpp-classic/Printer.C cpp-classic/Printer.H cpp-classic/Skeleton.C cpp-classic/Skeleton.H cpp-classic/Test.C &: grammar.cf | cpp-classic
	cd cpp-classic && bnfc -p LC --cpp ../grammar.cf

cpp-classic:
	mkdir cpp-classic
