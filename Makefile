CXXFLAGS = -std=c++17

# include cpp-classic/cpp-classic.mk
include desired/desired.mk
include desired-swl/desired-swl.mk
include cpp-var/cpp-var.mk
# include cpp-var-swl/cpp-var-swl.mk
include cpp-var-ranges/cpp-var-ranges.mk
include cpp-var-starts/cpp-var-starts.mk

.PHONY: all
.DEFAULT_GOAL := all

