#include <ncurses.h>
#include <stdlib.h>
#include "mine.h"

int x = 0;
int y = 0;
int z = 0;

short color(char val) {
    switch(val) {
        case INVISIBLE:
            return FLAG + 1;
        case 0:
            return FLAG + 1;
        default:
            return val;
    }
}

void cursor(Board *b) {
    static char v;
    if((v = val(b, z+1, y, x)) != INVALID) {
        mvchgat(4, 6, 3, A_REVERSE, color(v), NULL);
    }
    mvchgat(4, 30, 3, A_REVERSE, color(val(b, z, y, x)), NULL);
    if((v = val(b, z-1, y, x)) != INVALID) {
        mvchgat(4, 54, 3, A_REVERSE, color(v), NULL);
    }
}

void turn(Board *b) {
    static char c;
    for(;;) {
        output(b);
        switch((c = getch())) {
            case 'h':
                x = (x + b->xlen - 1) % b->xlen;
                break;
            case 'j':
                y = (y + b->ylen + 1) % b->ylen;
                break;
            case 'k':
                y = (y + b->ylen - 1) % b->ylen;
                break;
            case 'l':
                x = (x + b->xlen + 1) % b->xlen;
                break;
            case 'u':
                z = (z + b->zlen + 1) % b->zlen;
                break;
            case 'd':
                z = (z + b->zlen - 1) % b->zlen;
                break;
            case 'f':
                flag(b, z, y, x);
                break;
            case 'q':
                end(b);
                exit(0);
            case 'c':
                return;
        }
    }
}

void play(Board *b) {
    while(cont(b)) {
        turn(b);
        check(b, z, y, x);
    }
}

void initDisp() {
    char c;
    initscr();
    if(has_colors() == TRUE) {
        start_color();
        if(can_change_color() == TRUE) {
            init_color(1, 0, 0, 1000);
            init_color(2, 0, 1000, 0);
            init_color(3, 1000, 0, 0);
            init_color(4, 1000, 0, 1000);
            init_color(5, 500, 0, 0);
            init_color(6, 250, 875, 813);
            init_color(7, 0, 0, 0);
            init_color(8, 250, 250, 250);
            init_color(9, 700, 700, 0);
            init_color(10, 700, 50, 50);
            init_color(11, 0, 800, 800);
            for(c = 0; c < FLAG; ++c) {
                init_pair(c + 1, (c % 9) + 1, COLOR_BLACK);
            }
            init_pair(FLAG, 10, COLOR_BLACK);
            init_pair(INVISIBLE, 11, COLOR_BLACK);
            init_pair(INVALID, 2, COLOR_BLACK);
        } else {
            short colors[] = {COLOR_BLUE, COLOR_GREEN, COLOR_RED, COLOR_MAGENTA, COLOR_RED, COLOR_BLUE, COLOR_WHITE, COLOR_YELLOW};
            for(c = 0; c < FLAG + 1; ++c) {
                init_pair(c + 1, colors[c % 8], COLOR_BLACK);
            }
            init_pair(INVISIBLE, COLOR_CYAN, COLOR_BLACK);
            init_pair(INVALID, COLOR_GREEN, COLOR_BLACK);
        }
        init_pair(MINED, COLOR_BLACK, COLOR_RED);
    }
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();
}

Board *init() {
    Board *b = make();
    initDisp();
    alloc(b);
    turn(b);
    mine(b, z, y, x, 15);
    check(b, z, y, x);
    return b;
}

static void spotrep(char c, char *str) {
    switch(c) {
        case 0:
            sprintf(str, "   "); return;
        case INVALID:
            sprintf(str, " ~ "); return;
        case MINED:
            sprintf(str, " * "); return;
        case INVISIBLE:
            sprintf(str, "[?]"); return;
        case FLAG:
            sprintf(str, "[F]"); return;
        default:
            sprintf(str, "[%1X]", c); return;
    }
}

void print(char c, char h, char v) {
    char *str = malloc(4*sizeof(char));
    spotrep(c, str);
    attron(COLOR_PAIR(color(c)));
    mvprintw(h, v, str);
    attroff(COLOR_PAIR(color(c)));
    free(str);
}

void output(Board *b) {
    static char off[] = {-2, -1, 0, 1, 2};
    static char on[] = {-4, -3, -2, -1, 0, 1, 2, 3, 4};
    char c, h, v;
    for(h = -4; h <= 4; ++h) {
        for(v = -4; v <= 4; ++v) {
            print(val(b, z, y + h, x + v), h+4, 3*(v+10));
        }
    }
    for(h = -2; h <= 2; ++h) {
        for(v = -2; v <= 2; ++v) {
            print(val(b, z+1, y + h, x + v), h+4, 3*(v+2));
            print(val(b, z-1, y + h, x + v), h+4, 3*(v+18));
        }
    }
    attron(COLOR_PAIR(2));
    mvprintw(8, 3, "Flags: %2d", b->flags);
    attroff(COLOR_PAIR(2));
    cursor(b);
}

void end(Board *b) {
    attron(COLOR_PAIR(2));
    mvprintw(b->ylen, 0, won(b) ? "You have winned." : "You have losed.");
    showAll(b);
    output(b);
    getch();
    destroy(b);
    clear();
    endwin();
}
