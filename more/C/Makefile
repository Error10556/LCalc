.PHONY: core.c koe2.c runtime.c help clean

help:
	@echo 'Specify "core.c", "koe2.c", or "runtime.c" as a target to parse it.'
	@echo 'Specify "clean" to remove all generated files.'

core.c: C/Test
	C/Test -p $@

koe2.c: C/Test C4/Test
	C/Test -p $@
	C4/Test -p $@

runtime.c: C/Test
	C/Test -p $@

C4/Test: C4.cf
	bnfc --cpp-var -m -o C4 C4.cf
	$(MAKE) -C C4 Test

C/Test: C.cf
	bnfc --cpp-var -m -o C C.cf
	$(MAKE) -C C Test

clean:
	test ! -d C || rm -r C
	test ! -d C4 || rm -r C4
