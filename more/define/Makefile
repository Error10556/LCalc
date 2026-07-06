.PHONY: prettyparse

prettyparse: test/Test
	test/Test -p good01.in

test/Test: test.cf
	bnfc --cpp-var -o test -m test.cf
	$(MAKE) -C test Test
