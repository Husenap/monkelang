.PHONY: install-prerequisites gen build repl clean test

install-prerequisites:
	apt-get update
	apt-get install build-essential ninja-build cmake ccache
	apt-get install flex bison

gen: 
	cmake . -Bbuild -GNinja

build: 
	cmake --build build/

repl: 
	./build/src/monke/tools/monke_repl

test:
	cd build && ctest --output-on-failure

clean:
	rm -rf build/
