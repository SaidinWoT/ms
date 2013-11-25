CC=gcc

all:
	$(CC) -lncurses main.c ndisp.c mine.c -o ms

clean:
	rm ms
