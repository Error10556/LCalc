# Building

Run `make`.

# What is interesting

The `cpp-classic` directory contains an implementation of Lambda Calculus
using the current BNFC executable (see `cpp-classic/example.lc` for an example
of a Lambda Calculus program). The grammar can be found in `grammar.cf`.

The `cpp-classic` implementation uses the C++ backend of BNFC, which we
consider outdated.

`cpp-classic/LCTest` can be invoked to parse `cpp-classic/example.lc`,
and `cpp-classic/Eval` will evaluate the expressions of the program (using
call-by-value).

The `desired` directory contains a hand-crafted implementation of Lambda
Calculus using Flex and Bison. The grammar files `desired/grammar.l` and
`desired/grammar.ypp` are written by hand, but I aim to automate their
generation through a custom backend for BNFC.

`desired/Test` can be invoked to parse `desired/samples/*.lc`,
and `desired/Eval` will evaluate the expressions of the programs (using
call-by-value).
