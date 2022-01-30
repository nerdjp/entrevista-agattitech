CXX = g++
CFLAGS = -g -Wall
LIBS = -I/usr/include/SDL2 -lSDL2 -lSDL2main
NAME = main
BINDIR = bin

all: main

main:
	$(CXX) $(CFLAGS) $(LIBS) main.cpp -o $(BINDIR)/main

run: main
	./bin/main
