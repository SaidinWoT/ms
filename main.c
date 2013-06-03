#include "mine.h"

int main(int argc, char **argv) {
    Board *b = init();
<<<<<<< HEAD
    play(b);
=======
    do {
        turn(b);
        output(b);
    } while(cont(b));
>>>>>>> 0fe3865a7330f28910c369f20d376f850a88a9db
    end(b);
    return 0;
}

