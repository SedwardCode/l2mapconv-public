CC=clang
CXX=clang++
BUILD_TYPE=Release

.PHONY: configure
configure:
	CC=$(CC) CXX=$(CXX) cmake -S . -B build -G Ninja -D CMAKE_BUILD_TYPE=$(BUILD_TYPE) -D CMAKE_EXPORT_COMPILE_COMMANDS=1

.PHONY: build
build: configure
	cmake --build build --parallel

.PHONY: clean
clean:
	git submodule foreach --recursive git reset --hard
	rm -rf build

.PHONY: submodules
submodules:
	git submodule update --init --recursive
	git pull --recurse-submodules --jobs=8
