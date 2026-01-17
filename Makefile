CC = gcc
SRC = code/main.c code/drawing.c code/loadfiles.c
OUT = ./compiled/gra

INCLUDES = -I./headers `sdl2-config --cflags`
LIBS = `sdl2-config --libs`

all: setup compile

setup:
	mkdir -p ./compiled

compile:
	mkdir -p ./compiled
	gcc -O2 -I./headers `sdl2-config --cflags` -o ./compiled/gra code/main.c code/drawing.c code/loadfiles.c code/player.c code/init.c code/combos.c code/physics.c code/enemies.c `sdl2-config --libs`

clean:
	rm -rf ./compiled