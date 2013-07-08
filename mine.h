#ifndef _MINES_H
#define _MINES_H

#include <stdbool.h>

#define FLAG 27
#define INVISIBLE 28
#define INVALID 29
#define MINED 30

typedef unsigned char Dim;
typedef unsigned int Vol;
typedef struct _Spot Spot;
typedef struct {
    int pattern;
    Dim zlen, ylen, xlen;
    Vol mines, flags, clean;
    Spot ***grid;
} Board;

extern Board *make();
extern void alloc(Board *);
extern bool mine(Board *, Dim, Dim, Dim, Vol);
extern void destroy(Board *);

extern bool cont(Board *);

extern char val(Board *, Dim, Dim, Dim);
extern void flag(Board *, Dim, Dim, Dim);
extern void check(Board *, Dim, Dim, Dim);
extern void fnBoard(void (*)(Board *, Dim, Dim, Dim), Board *);
extern void showAll(Board *);

extern Board *init();
extern void play(Board *);
extern void output(Board *);
extern void end(Board *);

#endif
