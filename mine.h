#ifndef _MINES_H
#define _MINES_H

#include <stdbool.h>

#define FLAG 27
#define INVISIBLE 28
#define INVALID 29
#define MINED 30

typedef enum { \
    CROSS = 0x00000000, \
    SQUARE = 0x00000000, \
    CROSS3D = 0x00000000, \
    RINGS = 0x00000000, \
    CUBE = 0x07FFDFFF} Pattern;

typedef unsigned char Dim;
typedef unsigned int Vol;
typedef struct _Spot Spot;
typedef struct {
    int pattern;
    Dim zlen, ylen, xlen;
    Vol mines, flags, clean;
    Spot ***grid;
} Board;

typedef struct {
    int pattern;
    int z, y, x;
    Vol mines;
} Opts;

extern Board *make(Opts *opts);
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

#endif /* !_MINES_H */
