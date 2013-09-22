#include <stdio.h>
#include <unistd.h>
#include "mine.h"

Opts opts = {.pattern = SQUARE, .z = 1, .y = 10, .x = 10, .mines = 15};

void usage() {
    printf("ms by Scott Frazier\n");
    printf("Usage: ms [options]\n");
    printf("\t-d X,Y,Z\tSet dimensions with comma-delimited list\n");
    printf("\t-m N\t\tSet mine count to N\n");
    printf("\t-d P\t\tSet sweep pattern number\n");
    printf("\t\t1 - 2 Dimensional Square\n");
    printf("\t\t2 - 3 Dimensional Cube\n");
    printf("\t\t3 - 2 Dimensional Cross (Immediately adjacent cells only)\n");
    printf("\t\t4 - 3 Dimensional Cross (Immediately adjacent cells only)\n");
    printf("\t\t5 - 3 Dimensional Rings (Cube without corners)\n");
    printf("\t-h\t\tDisplays this information.\n");
    printf("\t-v\t\tDisplays brief version information.\n");
}

Pattern patterns[] = {SQUARE, CUBE, CROSS, CROSS3D, RINGS};

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
                opts.pattern = patterns[pat - 1];
        }
    }
    b = make(&opts);
    init(b, &opts);
    play(b);
    end(b);
    return 0;
}
