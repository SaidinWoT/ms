#include <stdio.h>
#include <unistd.h>
#include "mine.h"

Opts opts = {.pattern = SQUARE, .z = 1, .y = 10, .x = 10, .mines = 15};

void usage() {
    printf("ms by Scott Frazier\n");
    printf("\n");
    printf("Usage: ms [options]\n");
    printf("\n");
    printf("-d X,Y,Z    Set dimensions with comma-delimited list\n");
    printf("-m N        Set mine count to N\n");
    printf("-d P        Set sweep pattern number\n");
    printf("  1 - 2 Dimensional Square\n");
    printf("  2 - 3 Dimensional Cube\n");
    printf("  3 - 2 Dimensional Cross (Immediately adjacent cells only)\n");
    printf("  4 - 3 Dimensional Cross (Immediately adjacent cells only)\n");
    printf("  5 - 3 Dimensional Rings (Cube without corners)\n");
    printf("\n");
    printf("-h  Displays this information.\n");
    printf("-v  Displays brief version information.\n");
}

int main(int argc, char **argv) {
    Board *b;
    char c, pat;
    while((c = getopt(argc, argv, "hVd:m:p:")) != -1) {
        switch(c) {
            case '?':
            case 'h':
                usage();
                return 0;
            case 'V':
                printf("ms by Scott Frazier\n");
                return 0;
            case 'd':
                sscanf(optarg, "%d,%d,%d", &(opts.x), &(opts.y), &(opts.z));
                break;
            case 'm':
                sscanf(optarg, "%d", &(opts.mines));
                break;
            case 'p':
                sscanf(optarg, "%d", &pat);
                switch(pat) {
                    case 5:
                        opts.pattern = RINGS;
                        break;
                    case 4:
                        opts.pattern = CROSS3D;
                        break;
                    case 3:
                        opts.pattern = CROSS;
                        break;
                    case 2:
                        opts.pattern = CUBE;
                        break;
                    case 1:
                    default:
                        opts.pattern = SQUARE;
                        break;
                }
        }
    }
    b = make(&opts);
    init(b, &opts);
    play(b);
    end(b);
    return 0;
}
