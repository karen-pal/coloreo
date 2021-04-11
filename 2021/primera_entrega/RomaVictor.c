#include "RomaVictor.h"
#include <stdlib.h>
#include <stdio.h>
#include "grafo.h"


u32 NumeroDeVertices(Grafo G) {
    return G->cant_ver;
}

u32 NumeroDeLados(Grafo G) {
    return G->cant_lad;
}

u32 Delta(Grafo G) {
    u32 delta = 0;
    for (u32 index = 0; index<NumeroDeVertices(G); index++) {
        if (G->nodos_array[index].grado > delta) {
            delta = G->nodos_array[index].grado;
        }
    }
    return delta;
}

u32 Nombre(u32 i, Grafo G) {
    return G->nodos_array[G->orden[i]].nombre;
}

u32 Color(u32 i, Grafo G) {
    return G->nodos_array[G->orden[i]].color;
}

u32 Grado(u32 i, Grafo G) {
    return G->nodos_array[G->orden[i]].grado;
}

char FijarColor(u32 x,u32 i,Grafo G) {
    if (i < NumeroDeVertices(G)) {
        G->nodos_array[G->orden[i]].color = x;
        return 0;
    } else {
        return 1;
    }
}

char FijarOrden(u32 i,Grafo G,u32 N) {
    if (i < NumeroDeVertices(G) && N < NumeroDeVertices(G)) {
        G->orden[i] = G->array_nat[N];
        return 0;
    } else {
        return 1;
    }
}

u32 ColorVecino(u32 j,u32 i,Grafo G) {
    if (i >= NumeroDeVertices(G) || j >= G->nodos_array[G->orden[i]].grado) {
        return (2^32)-1;
    } else {
        u32 index = 0;
        while (G->nodos_array[G->orden[i]].vecinos[j].vecino != G->nodos_array[G->orden[index]].nombre) {
            index++;
        }
         return Color(index, G);
    }
}

u32 NombreVecino(u32 j,u32 i,Grafo G) {
    return G->nodos_array[G->orden[i]].vecinos[j].vecino;
}




u32 OrdenVecino(u32 j,u32 i,Grafo G) {
    u32 vecino = NombreVecino(j, i, G);
    for (u32 indice = 0; indice < NumeroDeVertices(G); indice++) {
        if (vecino == Nombre(indice, G)) {
            return indice;
        }
    }
}

void DestruccionDelGrafo(Grafo grafo){
   free(grafo->orden);
   for (u32 i = 0; i < NumeroDeVertices(grafo); i++) {
   	free(grafo->nodos_array[i].vecinos);
   }
   free(grafo->nodos_array);
   free(grafo);
}

Grafo ConstruccionDelGrafo(void) {
    Grafo grafo = malloc(sizeof(GrafoSt));
    bool ok = GraphParse(grafo, stdin);
    printf("esta todo %s\n", ok ? "true" : "false");
    if (!ok) {
	printf("Mal grafo\n");
        DestruccionDelGrafo(grafo);
        return NULL;
    }

    grafo->array_nat = malloc(sizeof(u32)*NumeroDeVertices(grafo));
    u32 *array_natural = malloc(sizeof(u32)*NumeroDeVertices(grafo));
    for (u32 indice = 0; indice < NumeroDeVertices(grafo); indice++) {
            array_natural[indice] = grafo->nodos_array[indice].nombre;

        }
    qsort(array_natural, NumeroDeVertices(grafo), sizeof(u32), cmpfunc);
    for (u32 i = 0; i < NumeroDeVertices(grafo); i++) {
        for (u32 j=0; j<NumeroDeVertices(grafo);j++){
            if (grafo->nodos_array[j].nombre == array_natural[i]){
                grafo->array_nat[i] = j;
                break;
            }
        }
    }

    return grafo;
}

Grafo CopiarGrafo(Grafo G){
    return G;
}

u32 PesoLadoConVecino(u32 j,u32 i,Grafo G){
    return 1;
}
u32 FijarPesoLadoConVecino(u32 j,u32 i,u32 p,Grafo G){
    return 1;
}
