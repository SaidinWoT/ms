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
    Dim zlen;
    Dim ylen;
    Dim xlen;
    Vol mines;
    Vol flags;
    Vol clean;
    Spot ***grid;
};

bool cont(Board *b) {
    return b->cont;
}

bool won(Board *b) {
    return !b->clean;
}

Dim zlen(Board *b) {
    return b->zlen;
}

Dim ylen(Board *b) {
    return b->ylen;
}

Dim xlen(Board *b) {
    return b->xlen;
}

void around(void (*fn)(Board *, Dim, Dim, Dim), Board *b, Dim z, Dim y, Dim x) {
    static char dz[] = {0, 0, 0, 0, 0, 0, 0, 0, \
        1, 0, 0, 0, 0, 0, 0, 0, 0, \
        -2, 0, 0, 0, 0, 0, 0, 0, 0};
    static char dy[] = {-1, 0, 0, 1, 0, 1, 0, 0, \
        0, 0, 0, -1, 0, 0, -1, 0, 0, \
        0, 0, 0, 1, 0, 0, 1, 0, 0};
    static char dx[] = {-1, 1, 1, -2, 2, -2, 1, 1, \
        0, -1, -1, 2, -1, -1, 2, -1, -1, \
        0, 1, 1, -2, 1, 1, -2, 1, 1};
    /* static char dz[] = { 0,  0,  0,  0,  0,  0,  0,  0}; */
    /* static char dy[] = {-1,  0,  0,  1,  0,  1,  0,  0}; */
    /* static char dx[] = {-1,  1,  1, -2,  2, -2,  1,  1}; */
    /* static char dz[] = { 0, 0,  0,  0}; */
    /* static char dy[] = {-1, 1,  1, -1}; */
    /* static char dx[] = { 0, 1, -1, -1}; */
    char n;
    for(n = 0; n < sizeof(dy)/sizeof(dy[0]); ++n) {
        z += dz[n];
        y += dy[n];
        x += dx[n];
        if(z >= 0 && z < b->zlen && y >= 0 && y < b->ylen && x >= 0 && x < b->xlen) {
            (*fn)(b, z, y, x);
        }
    }
}

void fnBoard(void (*fn)(Board *, Dim, Dim, Dim), Board *b) {
    Dim z = b->zlen;
    Dim y, x;
    while(z--) {
        y = b->ylen;
        while(y--) {
            x = b->xlen;
            while(x--) {
                (*fn)(b, z, y, x);
            }
        }
    }
}

char val(Board *b, Dim z, Dim y, Dim x) {
    if(z < 0 || z >= b->zlen || y < 0 || y >= b->ylen || x < 0 || x >= b->xlen) {
        return INVALID;
    } else if(b->grid[z][y][x].flag & VISIBLE) {
        return FLAG;
    } else if(b->grid[z][y][x].val & VISIBLE) {
        return b->grid[z][y][x].val & VALUE;
    } else {
        return INVISIBLE;
    }
}

static inline void zero(Board *b, Dim z, Dim y, Dim x) {
    b->grid[z][y][x].val = b->grid[z][y][x].flag = 0;
}

static inline void incVal(Board *b, Dim z, Dim y, Dim x) {
    ++b->grid[z][y][x].val;
}

static inline void incFlag(Board *b, Dim z, Dim y, Dim x) {
    ++b->grid[z][y][x].flag;
}

static inline void decFlag(Board *b, Dim z, Dim y, Dim x) {
    --b->grid[z][y][x].flag;
}

void reveal(Board *b, Dim z, Dim y, Dim x) {
    if(b->grid[z][y][x].val & VISIBLE || b->grid[z][y][x].flag & VISIBLE) {
        return;
    }
    if(b->grid[z][y][x].val & MINE) {
        b->cont = false;
        return;
    }
    b->grid[z][y][x].val |= VISIBLE;
    if(!--b->clean) {
        b->cont = false;
        return;
    }
    if(!(b->grid[z][y][x].val & VALUE)) {
        around(&reveal, b, z, y, x);
    }
}

void check(Board *b, Dim z, Dim y, Dim x) {
    if(b->grid[z][y][x].val & VISIBLE) {
        if(b->grid[z][y][x].flag && !((b->grid[z][y][x].val ^ b->grid[z][y][x].flag) & VALUE)) {
            around(&reveal, b, z, y, x);
        }
        return;
    }
    reveal(b, z, y, x);
}

void flag(Board *b, Dim z, Dim y, Dim x) {
    if(b->grid[z][y][x].val & VISIBLE) {
        return;
    }
    if(b->grid[z][y][x].flag & VISIBLE) {
        --b->flags;
        around(&decFlag, b, z, y, x);
    } else {
        ++b->flags;
        around(&incFlag, b, z, y, x);
    }
    b->grid[z][y][x].flag ^= VISIBLE;
}

Board *make() {
    Board *b = malloc(sizeof(Board));
    b->zlen = 3;
    b->ylen = 10;
    b->xlen = 10;
    b->mines = 0;
    b->flags = 0;
    b->clean = (b->zlen * b->ylen * b->xlen);
    b->alloc = false;
    b->cont = true;
    b->grid = NULL;
    return b;
}

void alloc(Board *b) {
    Dim z = b->zlen;
    Dim y = b->ylen;
    if(b == NULL) {
        return;
    }
    b->grid = malloc(b->zlen * sizeof(Spot **));
    while(z--) {
        b->grid[z] = malloc(b->ylen * sizeof(Spot *));
        while(y--) {
            b->grid[z][y] = malloc(b->xlen * sizeof(Spot));
        }
    }
    fnBoard(zero, b);
    b->alloc = true;
}

bool mine(Board *b, Vol mines) {
    Dim z, y, x;
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
        z = rand() % b->zlen;
        y = rand() % b->ylen;
        x = rand() % b->xlen;
        if(!(b->grid[z][y][x].val & (MINE | VISIBLE))) {
            b->grid[z][y][x].val |= MINE;
            around(&incVal, b, z, y, x);
            --mines;
        }
    }
    return true;
}

void destroy(Board *b) {
    while(b->zlen--) {
        while(b->ylen--) {
            free(b->grid[b->zlen][b->ylen]);
        }
        free(b->grid[b->zlen]);
    }
    free(b->grid);
    free(b);
}
