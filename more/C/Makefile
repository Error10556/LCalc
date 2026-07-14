.PHONY: core.c koe2.c runtime.c help clean

help:
	@echo 'Specify "core.c", "koe2.c", or "runtime.c" as a target to parse it.'
	@echo 'Specify "clean" to remove all generated files.'

core.c: C/TestC
	C/TestC -p $@

koe2.c: C/TestC C4/TestC4
	C/TestC -p $@
	C4/TestC4 -p $@

runtime.c: C/TestC
	C/TestC -p $@

C4/TestC4: C4.cf
	bnfc --cpp-var -m -o C4 C4.cf
	$(MAKE) -C C4 TestC4

C/TestC: C.cf
	bnfc --cpp-var -m -o C C.cf
	$(MAKE) -C C TestC

clean:
	test ! -d C || rm -r C
	test ! -d C4 || rm -r C4
