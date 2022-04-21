fmt:
	cd src && (ls *.cpp *.h *.hpp *.cc | xargs clang-format -i -style=file)

clean:
	xmake clean

build:
	xmake

run:
	xmake run

.PHONY: build run clean fmt
