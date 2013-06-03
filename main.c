#include "mine.h"

int main(int argc, char **argv) {
    Board *b = init();
    play(b);
    end(b);
    return 0;
}

