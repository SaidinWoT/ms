#include "mine.h"

int main(int argc, char **argv) {
    init();
    Board *b = make();
    alloc(b);
    mine(b, 10);
    do {
        turn(b);
        output(b);
    } while(cont(b));
    end(b);
    return 0;
}

