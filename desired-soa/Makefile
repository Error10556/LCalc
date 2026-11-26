.PHONY: build debug coverage buildprofile devsetup clean-build clean-debug clean-coverage clean-buildprofile clean-devsetup clean

build:
	cmake -S . -B build/ -DCMAKE_BUILD_TYPE=Release $(CMAKE_CONFFLAGS)
	cmake --build build/ -j10 $(CMAKE_BUILDFLAGS)

debug:
	cmake -S . -B debug/ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++ $(CMAKE_CONFFLAGS)
	cmake --build debug/ -j10 $(CMAKE_BUILDFLAGS)

coverage:
	cmake -S . -B coverage/ -DCMAKE_BUILD_TYPE=Coverage -DCMAKE_CXX_COMPILER=g++ $(CMAKE_CONFFLAGS)
	cmake --build coverage/ -j10 $(CMAKE_BUILDFLAGS)

buildprofile:
	cmake -S . -B buildprofile/ -DCMAKE_BUILD_TYPE=BuildProfile $(CMAKE_CONFFLAGS)
	cmake --build buildprofile/ $(CMAKE_BUILDFLAGS)

devsetup:
	cmake -S . -B devsetup/ -DCMAKE_BUILD_TYPE=DevSetup $(CMAKE_CONFFLAGS)

clean-debug:
	rm -rf debug/

clean-build:
	rm -rf build/

clean-coverage:
	rm -rf coverage/

clean-buildprofile:
	rm -rf buildprofile/

clean-devsetup:
	rm -rf devsetup/

clean:
	rm -rf build/ debug/ coverage/ buildprofile/ devsetup/
