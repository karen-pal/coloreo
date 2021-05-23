#ifndef __req_H
#define __req_H


#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>
#include "GrafoSt21.h"

typedef GrafoSt *Grafo;

typedef
    struct Bookkeeping {
        u32 nombre_nodo;
        u32 indice_interno;

    }
    Bookkeeping;

typedef 
    struct Elem {
        u32 indice_orig; //orden interno
        u32 color;
    } 
    Elem;
char AleatorizarVertices(Grafo G,u32 R);

char Bipartito(Grafo G);

u32 Greedy(Grafo G);

u32 NumCCs(Grafo G);

void Bfs (u32 x, u32 *vertices_cc, Grafo G);
u32 EncontrarMinimoColor(u32 * color_vecinos, u32 grado);

void ArregloColoresVecino(u32 vertice, Grafo G, u32 * color_vecinos, u32 grado);
int _natural_compare(const void *_a, const void *_b);
int _struct_compare(const void *_a, const void *_b);
u32 MaxColor(Grafo G);

u32* CopiarArreglo(void *orig, size_t element_size, u32 largo);

int _mayor_menor_comp(const void *_a, const void *_b);
bool ChequearPermutacion ( u32* perm, u32 color);

void CalcularNaturalArray(Grafo G, u32 * result);
char OrdenPorBloqueDeColores(Grafo G, u32 * perm);
#endif
