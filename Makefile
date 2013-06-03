CC=gcc

all:
	$(CC) -lncurses main.c disp.c mine.c -o ms

clean:
	rm ms
