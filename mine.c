#include <stdbool.h>
#include <stdlib.h>
#include "mine.h"

#define VALUE 0x1F
#define MINE 0x40
#define VISIBLE 0x80

#define CONT (0x1 << 31)

struct _Spot {
    char val;
    char flag;
};

bool cont(Board *b) {
    return b->pattern & CONT;
}

bool won(Board *b) {
    return !b->clean;
}

void around(void (*fn)(Board *, Dim, Dim, Dim), Board *b, Dim z, Dim y, Dim x) {
    static char dz[] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    static char dy[] = {-1,-1,-1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,-1,-1,-1,-1,-1,-1, 0, 0, 0, 1, 1, 1};
    static char dx[] = {-1, 0, 1,-1, 1,-1, 0, 1, 1, 0,-1, 1, 0,-1, 1, 0,-1,-1, 0, 1,-1, 0, 1,-1, 0, 1};
    char u, v, w, n;
    int pattern = b->pattern & 0x07FFFFFF;
    for(n = 0; pattern; ++n, pattern >>= 1) {
        if(pattern & 1) {
            w = z + dz[n];
            v = y + dy[n];
            u = x + dx[n];
            if(w >= 0 && w < b->zlen && v >= 0 && v < b->ylen && u >= 0 && u < b->xlen) {
                (*fn)(b, w, v, u);
            }
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
        if(b->grid[z][y][x].val & MINE) {
            return MINED;
        }
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

static inline void show(Board *b, Dim z, Dim y, Dim x) {
    b->grid[z][y][x].val |= VISIBLE;
}

void reveal(Board *b, Dim z, Dim y, Dim x) {
    if(b->grid[z][y][x].val & VISIBLE || b->grid[z][y][x].flag & VISIBLE) {
        return;
    }
    if(b->grid[z][y][x].val & MINE) {
        b->pattern &= ~CONT;
        return;
    }
    b->grid[z][y][x].val |= VISIBLE;
    if(!--b->clean) {
        b->pattern &= ~CONT;
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
        ++b->flags;
        around(&decFlag, b, z, y, x);
    } else {
        --b->flags;
        around(&incFlag, b, z, y, x);
    }
    b->grid[z][y][x].flag ^= VISIBLE;
}

Board *make() {
    Board *b = malloc(sizeof(Board));
    b->pattern = 0x800000FF;
    b->zlen = 1;
    b->ylen = 10;
    b->xlen = 10;
    b->mines = 0;
    b->flags = 0;
    b->clean = (b->zlen * b->ylen * b->xlen);
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
        for(y = b->ylen; y--;) {
            b->grid[z][y] = malloc(b->xlen * sizeof(Spot));
        }
    }
    fnBoard(zero, b);
}

bool mine(Board *b, Dim z, Dim y, Dim x, Vol mines) {
    Dim w, v, u;
    if(mines > b->clean) {
        return false;
    }
    srand(time(0));
    b->mines += mines;
    b->flags += mines;
    b->clean -= mines;
    b->grid[z][y][x].val |= MINE;
    while(mines) {
        w = rand() % b->zlen;
        v = rand() % b->ylen;
        u = rand() % b->xlen;
        if(!(b->grid[w][v][u].val & (MINE | VISIBLE))) {
            b->grid[w][v][u].val |= MINE;
            around(&incVal, b, w, v, u);
            --mines;
        }
    }
    b->grid[z][y][x].val &= ~MINE;
    return true;
}

void showAll(Board *b) {
    fnBoard(show, b);
}

void destroy(Board *b) {
    Dim y;
    while(b->zlen--) {
        for(y = b->ylen; y--;) {
            free(b->grid[b->zlen][y]);
        }
        free(b->grid[b->zlen]);
    }
    free(b->grid);
    free(b);
}
