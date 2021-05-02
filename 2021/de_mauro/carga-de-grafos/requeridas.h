#ifndef __req_H
#define __req_H


#include <stdio.h>
#include <stdlib.h>

#include "GrafoSt21.h"

typedef GrafoSt *Grafo;

char WelshPowell(Grafo G);

char RevierteBC(Grafo G);

char ChicoGrandeBC(Grafo G);

char AleatorizarVertices(Grafo G,u32 R);

char SwitchColores(Grafo G,u32 i,u32 j);

char Bipartito(Grafo G);

u32 Greedy(Grafo G);

u32 NumCCs(Grafo G);

void Bfs (u32 x, u32 *vertices_cc, Grafo G);
u32 encontrar_minimo_color(u32 i ,u32 * color_vecinos, u32 grado);

void arreglo_colores_vecino(u32 vertice, Grafo G, u32 * color_vecinos, u32 grado);
int cmpfunc (const void * a, const void * b); 
u32 MaxColor(Grafo G);
#endif
