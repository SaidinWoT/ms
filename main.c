#include "mine.h"

int main(int argc, char **argv) {
    Board *b = init();
    do {
        turn(b);
        output(b);
    } while(cont(b));
    end(b);
    return 0;
}

