.PHONY: build debug # coverage

build:
	cmake -S . -B build/ -DCMAKE_BUILD_TYPE=Release
	cmake --build build/ -j10

debug:
	cmake -S . -B debug/ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++
	cmake --build debug/ -j10
