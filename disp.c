#include <ncurses.h>
#include <stdlib.h>
#include "mine.h"

char color(char val) {
    switch(val) {
        case FLAG:
            return 1;
        case -1:
            return 2;
        default:
            return 3;
    }
}

void turn(Board *b) {
    static int ch;
    static int x = 0;
    static int y = 0;
    for(ch = getch(); ch != 'c'; ch = getch()) {
        mvchgat(y, x*3, 3, A_NORMAL, color(val(b, y, x)), NULL);
        switch(ch) {
            case 'h':
                x = (x + cols(b) - 1) % cols(b);
                break;
            case 'j':
                y = (y + rows(b) + 1) % rows(b);
                break;
            case 'k':
                y = (y + rows(b) - 1) % rows(b);
                break;
            case 'l':
                x = (x + cols(b) + 1) % cols(b);
                break;
            case 'f':
                flag(b, y, x);
                output(b);
                break;
            case 'q':
                endwin();
                exit(0);
        }
        mvchgat(y, x*3, 3, A_REVERSE, color(val(b, y, x)), NULL);
    }
    check(b, y, x);
}

Board *init() {
    Board *b = make();
    alloc(b);
    mine(b, 10);
    initscr();
    if(has_colors() == TRUE) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_YELLOW);
        init_pair(2, COLOR_WHITE, COLOR_BLUE);
        init_pair(3, COLOR_BLACK, COLOR_CYAN);
        init_pair(4, COLOR_RED, COLOR_CYAN);
        init_pair(5, COLOR_RED, COLOR_YELLOW);
    }
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();
    output(b);
    mvchgat(0, 0, 3, A_REVERSE, 2, NULL);
}

void printSpot(Board *b, Dim h, Dim v) {
    char value = val(b, h, v);
    attron(COLOR_PAIR(color(val(b, h, v))));
    if(value == -1) {
        mvprintw(h, 3*v, "[?]");
    } else if(value == FLAG) {
        mvprintw(h, 3*v, "[F]");
    } else if(value == 0) {
        mvprintw(h, 3*v, "   ");
    } else {
        mvprintw(h, 3*v, "[%X]", value);
    }
    attroff(COLOR_PAIR(color(val(b, h, v))));
}

void output(Board *b) {
    fnBoard(printSpot, b);
}

void end(Board *b) {
    if(won(b)) {
        mvprintw(rows(b), 0, "You have winned.");
    } else {
        mvprintw(rows(b), 0, "You have losed.");
    }
    getch();
    endwin();
}
