all: test

STX_DIR ?= "deps/libstx"

build:
	mkdir -p build/devel
	cd build/devel && \
	cmake -DSTX_DIR=$(STX_DIR) -DCMAKE_BUILD_TYPE=Debug ../.. && \
	make

test: build
	/bin/bash -c 'cd build/devel && (ls -1 test-cstable* | while read l; do ./$$l || exit 1; done)'

clean:
	rm -rf build

.PHONY: build test clean
