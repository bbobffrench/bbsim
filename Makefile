CC      := gcc
CFLAGS  := -std=c99 -Wall -pedantic -Iinclude -O3
LDFLAGS := -lSDL2 -lcairo

.PHONY: clean

bin/bbsim:build/bbsim.o build/bb.o build/graphics.o
	@mkdir -p bin
	$(CC) $^ $(LDFLAGS) -o $@

build/bbsim.o: src/bbsim.c include/bb.h include/graphics.h
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/bb.o: src/bb.c include/bb.h
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/graphics.o: src/graphics.c include/graphics.h
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/* bin/*
