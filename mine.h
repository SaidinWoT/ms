#ifndef _MINES_H
#define _MINES_H

#include <stdbool.h>

#define FLAG 27
#define INVISIBLE 28
#define INVALID 29

typedef struct _Board Board;
typedef unsigned char Dim;
typedef unsigned int Vol;

extern Board *make();
extern void alloc(Board *);
extern bool mine(Board *, Vol);
extern void destroy(Board *);

extern bool cont(Board *);
extern Dim zlen(Board *);
extern Dim ylen(Board *);
extern Dim xlen(Board *);

extern char val(Board *, Dim, Dim, Dim);
extern void flag(Board *, Dim, Dim, Dim);
extern void check(Board *, Dim, Dim, Dim);
extern void fnBoard(void (*)(Board *, Dim, Dim, Dim), Board *);

extern Board *init();
extern void play(Board *);
extern void output(Board *);
extern void end(Board *);

#endif
