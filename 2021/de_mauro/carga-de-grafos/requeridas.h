#ifndef __req_H
#define __req_H


#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>
#include "GrafoSt21.h"

typedef GrafoSt *Grafo;

typedef
    struct Result {
        u32 nombre_nodo;
        u32 indice_interno;

    }
    Result;

typedef 
    struct Elem {
        u32 indice_orig;
        u32 color;
    } 
    Elem;
char AleatorizarVertices(Grafo G,u32 R);

char Bipartito(Grafo G);

u32 Greedy(Grafo G);

u32 NumCCs(Grafo G);

void Bfs (u32 x, u32 *vertices_cc, Grafo G);
u32 encontrar_minimo_color(u32 i ,u32 * color_vecinos, u32 grado, Grafo G);

void arreglo_colores_vecino(u32 vertice, Grafo G, u32 * color_vecinos, u32 grado);
int _natural_compare(const void *_a, const void *_b);
int _struct_compare(const void *_a, const void *_b);
u32 MaxColor(Grafo G);

u32* copiar_arreglo(void *orig, size_t element_size, u32 length);

int _mayor_menor_comp(const void *_a, const void *_b);
bool check_permutation ( u32* perm, u32 color);

void calcular_natural_array(Grafo G, u32 * result);
char OrdenPorBloqueDeColores(Grafo G, u32 * perm);
u32** OrdenPorBloqueDeColores2(Grafo G, u32 * perm);
#endif
