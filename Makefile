include cpp-classic/cpp-classic.mk

.PHONY: all
.DEFAULT_GOAL := all

all: \
	cpp-classic/LCTest \
	cpp-classic/Eval \
	| cpp-classic
