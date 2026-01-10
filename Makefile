CC = gcc
SRC = code/main.c
HEADERS = -I./headers
OUT = gra

mac:
	$(CC) $(SRC) -o $(OUT) $(HEADERS) `sdl2-config --cflags --libs`

lab:
	$(CC) -O2 -I./SDL2-2.0.10/include -L. -o main $(SRC) $(HEADERS) -lm -lSDL2-64 -lpthread -ldl -Irt

clean:
	rm -f $(OUT) main