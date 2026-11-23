.PHONY: build debug coverage clean-build clean-debug clean-coverage clean

build:
	cmake -S . -B build/ -DCMAKE_BUILD_TYPE=Release $(CONFFLAGS)
	cmake --build build/ -j10 $(MAKEFLAGS)

debug:
	cmake -S . -B debug/ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++ $(CONFFLAGS)
	cmake --build debug/ -j10 $(MAKEFLAGS)

coverage:
	cmake -S . -B coverage/ -DCMAKE_BUILD_TYPE=Coverage -DCMAKE_CXX_COMPILER=g++ $(CONFFLAGS)
	cmake --build coverage/ -j10 $(MAKEFLAGS)

clean-debug:
	rm -rf debug/

clean-build:
	rm -rf build/

clean-coverage:
	rm -rf coverage/

clean:
	rm -rf build/ debug/ coverage/
