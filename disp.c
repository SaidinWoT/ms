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
void play(Board *b) {
    static char v;
    while(cont(b)) {
        if((v = val(b, z+1, y, x)) != INVALID) {
            mvchgat(4, 6, 3, A_REVERSE, color(v), NULL);
        }
        mvchgat(4, 30, 3, A_REVERSE, color(val(b, z, y, x)), NULL);
        if((v = val(b, z-1, y, x)) != INVALID) {
            mvchgat(4, 54, 3, A_REVERSE, color(v), NULL);
        }
        switch(getch()) {
            case 'h':
                x = (x + xlen(b) - 1) % xlen(b);
                break;
            case 'j':
                y = (y + ylen(b) + 1) % ylen(b);
                break;
            case 'k':
                y = (y + ylen(b) - 1) % ylen(b);
                break;
            case 'l':
                x = (x + xlen(b) + 1) % xlen(b);
                break;
            case 'u':
                z = (z + zlen(b) + 1) % zlen(b);
                break;
            case 'd':
                z = (z + zlen(b) - 1) % zlen(b);
                break;
            case 'f':
                flag(b, z, y, x);
                output(b);
                break;
            case 'q':
                endwin();
                exit(0);
            case 'c':
                check(b, z, y, x);
        }
        output(b);
    }
}

Board *init() {
    char c;
    Board *b = make();
    alloc(b);
    mine(b, 30);
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
            short colors[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
            for(c = 0; c < FLAG; ++c) {
                init_pair(c + 1, (c % 9) + 1, 0);
            }
            init_pair(FLAG, 10, 0);
            init_pair(INVISIBLE, 11, 0);
            init_pair(INVALID, 2, 0);
        } else {
            short colors[] = {COLOR_BLUE, COLOR_GREEN, COLOR_RED, COLOR_MAGENTA, COLOR_RED, COLOR_CYAN, COLOR_WHITE, COLOR_YELLOW};
            for(c = 0; c < INVALID + 1; ++c) {
                init_pair(c + 1, colors[c % 7], 0);
            }
        }
    }
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();
    output(b);
    /* mvchgat(4, 30, 3, A_REVERSE, FLAG + 1, NULL); */
    return b;
}

static char *spotstr(char v) {
    char *ret = malloc(3*sizeof(char));
    switch(v) {
        case 0:
            return "   ";
        case INVISIBLE:
            return "[?]";
        case INVALID:
            return " ~ ";
        case FLAG:
            return "[F]";
        default:
            sprintf(ret, "[%X]", v);
            return ret;
    }
}

static void printSpot(Board *b, Dim z, Dim y, Dim x) {
    char c = val(b, z, y, x);
    attron(COLOR_PAIR(color(c)));
    mvprintw(y, 3*x, spotstr(c));
    attroff(COLOR_PAIR(color(c)));
}

void output(Board *b) {
    static char off[] = {-2, -1, 0, 1, 2};
    static char on[] = {-4, -3, -2, -1, 0, 1, 2, 3, 4};
    char h, v, c;
    for(h = 0; h < sizeof(on)/sizeof(*on); ++h) {
        for(v = 0; v < sizeof(on)/sizeof(*on); ++v) {
            c = val(b, z, y + on[h], x + on[v]);
            attron(COLOR_PAIR(color(c)));
            mvprintw(h, 3*(1+(sizeof(off)/sizeof(*off))+v), spotstr(c));
            attroff(COLOR_PAIR(color(c)));
        }
    }
    for(h = 0; h < sizeof(off)/sizeof(*off); ++h) {
        for(v = 0; v < sizeof(off)/sizeof(*off); ++v) {
            c = val(b, z+1, y + off[h], x + off[v]);
            attron(COLOR_PAIR(color(c)));
            mvprintw(h+2, 3*v, spotstr(c));
            attroff(COLOR_PAIR(color(c)));
            c = val(b, z-1, y + off[h], x + off[v]);
            attron(COLOR_PAIR(color(c)));
            mvprintw(h+2, 3*(2+(sizeof(off)/sizeof(*off))+(sizeof(on)/sizeof(*on))+v), spotstr(c));
            attroff(COLOR_PAIR(color(c)));
        }
    }
}

void end(Board *b) {
    attron(COLOR_PAIR(2));
    if(won(b)) {
        mvprintw(ylen(b), 0, "You have winned.");
    } else {
        mvprintw(ylen(b), 0, "You have losed.");
    }
    getch();
    destroy(b);
    clear();
    endwin();
}
