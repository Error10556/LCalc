CXXFLAGS = -std=c++17

include cpp-classic/cpp-classic.mk
include desired/desired.mk
include cpp-var/cpp-var.mk

.PHONY: all
.DEFAULT_GOAL := all

