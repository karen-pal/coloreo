#ifndef __ccg_H
#define __ccg_H

#include <stdio.h>
#include <stdlib.h>

#include "requeridas.h"

int cmpfunc (const void * a, const void * b);

u32* Natural(Grafo G);

u32 MaxColor(Grafo G);

void Bfs (u32 x, u32 *vertices_cc, Grafo G);

#endif