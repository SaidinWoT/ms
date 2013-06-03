#ifndef _MINES_H
#define _MINES_H

#include <stdbool.h>

#define FLAG 9

typedef struct _Board Board;
typedef unsigned char Dim;
typedef unsigned short Area;

extern Board *make();
extern void alloc(Board *);
extern bool mine(Board *, Area);

extern bool cont(Board *);
extern Dim rows(Board *);
extern Dim cols(Board *);

extern char val(Board *, Dim, Dim);
extern void flag(Board *, Dim, Dim);
extern void check(Board *, Dim, Dim);
extern void fnBoard(void (*)(Board *, Dim, Dim), Board *);

extern Board *init();
extern void turn(Board *);
extern void output(Board *);
extern void end(Board *);

#endif
