#include "calculoscongrafos.h"

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

u32* Natural(Grafo G) {
    u32 *array_natural = malloc(sizeof(u32)*NumeroDeVertices(G));
    for (u32 indice = 0; indice < NumeroDeVertices(G); indice++) {
            array_natural[indice] = Nombre(indice, G);
    }
    qsort(array_natural, NumeroDeVertices(G), sizeof(u32), cmpfunc);
    return array_natural;
}

u32 MaxColor(Grafo G) {
    u32 max_color = 0 ;
    for (u32 indice = 0; indice < NumeroDeVertices(G); indice++) {
        if (Color(indice, G) > max_color) {
            max_color = Color(indice, G);
        }
    }
    return max_color;
}

void Bfs (u32 x, u32 *vertices_cc, Grafo G) {
    if (vertices_cc[x] == 0) {
        vertices_cc[x] = 1;
    }
    for (u32 j = 0; j < Grado(x,G); j++) {
        int indice = OrdenVecino(j,x,G);
        if (vertices_cc[indice] == 0) {
            vertices_cc[indice] = 2;
        }
    }
    for (u32 j = 0; j < Grado(x,G); j++) {
        u32 indice = OrdenVecino(j,x,G);
        for (u32 i = 0; i < Grado(indice,G); i++) {
            if (vertices_cc[OrdenVecino(i,indice,G)] == 0) {
                Bfs(OrdenVecino(i,indice,G), vertices_cc, G);
            }
        }
    }
}