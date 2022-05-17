fmt:
	cd src && (ls *.cpp *.h *.hpp *.cc | xargs clang-format -i -style=file)

clean:
	xmake clean

build:
	xmake

run:
	xmake run

compile_commands:
	xmake project -k compile_commands
	mv compile_commands.json ./build

.PHONY: build run clean fmt
