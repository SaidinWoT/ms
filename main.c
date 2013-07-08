#include <stdio.h>
#include <unistd.h>
#include "mine.h"

Opts opts = {.pattern = 0x000000FF, .z = 1, .y = 10, .x = 10, .mines = 15};

void usage() {
    printf("Usage:\n");
}

int main(int argc, char **argv) {
    Board *b;
    char c;
    while((c = getopt(argc, argv, "x:y:z:m:")) != -1) {
        switch(c) {
            case 'x':
                sscanf(optarg, "%d", &(opts.x));
                break;
            case 'y':
                sscanf(optarg, "%d", &(opts.y));
                break;
            case 'z':
                sscanf(optarg, "%d", &(opts.z));
                break;
            case 'm':
                sscanf(optarg, "%d", &(opts.mines));
                break;
            case '?':
                usage();
                return 1;
        }
    }
    printf("%dx %dy %dz %dm\n", opts.x, opts.y, opts.z, opts.mines);
    b = make(&opts);
    init(b, &opts);
    play(b);
    end(b);
    return 0;
}
