#include <stdbool.h>
#include <stdlib.h>
#include "mine.h"

#define VALUE 0x1F
#define MINE 0x40
#define VISIBLE 0x80

typedef struct {
    char val;
    char flag;
} Spot;

struct _Board {
    bool cont;
    bool alloc;
    Dim rows;
    Dim cols;
    Area mines;
    Area flags;
    Area clean;
    Spot **grid;
};

bool cont(Board *b) {
    return b->cont;
}

bool won(Board *b) {
    return !b->clean;
}

Dim rows(Board *b) {
    return b->rows;
}

Dim cols(Board *b) {
    return b->cols;
}

void around(void (*fn)(Board *, Dim, Dim), Board *b, Dim h, Dim v) {
    static char row[] = {-1,  0,  0,  1,  0,  1,  0,  0};
    static char col[] = {-1,  1,  1, -2,  2, -2,  1,  1};
    /* static char row[] = {-1, 1,  1, -1}; */
    /* static char col[] = { 0, 1, -1, -1}; */
    char n;
    for(n = 0; n < sizeof(row)/sizeof(row[0]); ++n) {
        h += row[n];
        v += col[n];
        if(h >= 0 && h < b->rows && v >= 0 && v < b->cols) {
            (*fn)(b, h, v);
        }
    }
}

void fnBoard(void (*fn)(Board *, Dim, Dim), Board *b) {
    Dim rows = b->rows;
    Dim cols;
    while(rows--) {
        cols = b->cols;
        while(cols--) {
            (*fn)(b, rows, cols);
        }
    }
}

char val(Board *b, Dim h, Dim v) {
    if(b->grid[h][v].flag & VISIBLE) {
        return FLAG;
    } else if(b->grid[h][v].val & VISIBLE) {
        return b->grid[h][v].val & VALUE;
    } else {
        return -1;
    }
}

static inline void zero(Board *b, Dim h, Dim v) {
    b->grid[h][v].val = b->grid[h][v].flag = 0;
}

static inline void incVal(Board *b, Dim h, Dim v) {
    ++b->grid[h][v].val;
}

static inline void incFlag(Board *b, Dim h, Dim v) {
    ++b->grid[h][v].flag;
}

static inline void decFlag(Board *b, Dim h, Dim v) {
    --b->grid[h][v].flag;
}

void reveal(Board *b, Dim h, Dim v) {
    if(b->grid[h][v].val & VISIBLE || b->grid[h][v].flag & VISIBLE) {
        return;
    }
    if(b->grid[h][v].val & MINE) {
        b->cont = false;
        return;
    }
    b->grid[h][v].val |= VISIBLE;
    if(!--b->clean) {
        b->cont = false;
        return;
    }
    if(!(b->grid[h][v].val & VALUE)) {
        around(&reveal, b, h, v);
    }
}

void check(Board *b, Dim h, Dim v) {
    if(b->grid[h][v].val & VISIBLE) {
        if(b->grid[h][v].flag && !((b->grid[h][v].val ^ b->grid[h][v].flag) & VALUE)) {
            around(&reveal, b, h, v);
        }
        return;
    }
    reveal(b, h, v);
}

void flag(Board *b, Dim h, Dim v) {
    if(b->grid[h][v].val & VISIBLE) {
        return;
    }
    if(b->grid[h][v].flag & VISIBLE) {
        --b->flags;
        around(&decFlag, b, h, v);
    } else {
        ++b->flags;
        around(&incFlag, b, h, v);
    }
    b->grid[h][v].flag ^= VISIBLE;
}

Board *make() {
    Board *b = malloc(sizeof(Board));
    b->rows = 10;
    b->cols = 10;
    b->mines = 0;
    b->flags = 0;
    b->clean = (b->rows * b->cols);
    b->alloc = false;
    b->cont = true;
    b->grid = NULL;
    return b;
}

void alloc(Board *b) {
    Dim rows = b->rows;
    if(b == NULL) {
        return;
    }
    b->grid = malloc(b->rows * sizeof(Spot *));
    while(rows--) {
        b->grid[rows] = malloc(b->cols * sizeof(Spot));
    }
    fnBoard(zero, b);
    b->alloc = true;
}

bool mine(Board *b, Area mines) {
    Dim h, v;
    if(!b->alloc) {
        return false;
    }
    if(mines > b->clean) {
        return false;
    }
    srand(time(0));
    b->mines += mines;
    b->flags += mines;
    b->clean -= mines;
    while(mines) {
        h = rand() % b->rows;
        v = rand() % b->cols;
        if(!(b->grid[h][v].val & (MINE | VISIBLE))) {
            b->grid[h][v].val |= MINE;
            around(&incVal, b, h, v);
            --mines;
        }
    }
    return true;
}

void destroy(Board *b) {
    while(b->rows--) {
        free(b->grid[b->rows]);
    }
    free(b->grid);
    free(b);
}
