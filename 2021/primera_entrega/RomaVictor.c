#include "RomaVictor.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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

int cmpfuncs (const void * a, const void * b) {
    Tupla *A = (Tupla *)a;
    Tupla *B = (Tupla *)b;

  return ( A->nodo1 - B->nodo1 );
}

Grafo ConstruccionDelGrafo(void) {
    u32 *temp_nodos_1=NULL;
    u32 *temp_nodos_2=NULL;
    Grafo grafo = malloc(sizeof(GrafoSt));
    Result res = GraphParse(grafo, stdin, temp_nodos_1,temp_nodos_2);
    for (int i=0; i<2*NumeroDeLados(grafo);i++){
	printf("temp_1[%d]: %u || temp_2[%d]: %u\n",i,res.nodos_1[i],i,res.nodos_2[i]);
    }	
    printf("esta todo %s\n", res.result ? "bien" : "mal");
    if (! res.result) {
	printf("Mal grafo\n");
        DestruccionDelGrafo(grafo);
        return NULL;
    }

     Tupla array_n[NumeroDeLados(grafo)*2];

   for (int i = 0; i < NumeroDeLados(grafo)*2; i++){
       array_n[i].nodo1 = res.nodos_1[i];
       array_n[i].nodo2 = res.nodos_2[i];
   }
   qsort(array_n, NumeroDeLados(grafo)*2, sizeof(Tupla), cmpfuncs);

     for (int i=0; i<2*NumeroDeLados(grafo);i++){
	printf("temp_1[%d]: %u || temp_2[%d]: %u\n",i,array_n[i].nodo1,i,array_n[i].nodo2);
    }	

    //grafo->array_nat = malloc(sizeof(u32)*NumeroDeVertices(grafo));
    //u32 *array_natural = malloc(sizeof(u32)*NumeroDeVertices(grafo));
    //for (u32 indice = 0; indice < NumeroDeVertices(grafo); indice++) {
    //        array_natural[indice] = grafo->nodos_array[indice].nombre;

    //    }
    //qsort(array_natural, NumeroDeVertices(grafo), sizeof(u32), cmpfunc);
    //for (u32 i = 0; i < NumeroDeVertices(grafo); i++) {
    //    for (u32 j=0; j<NumeroDeVertices(grafo);j++){
    //        if (grafo->nodos_array[j].nombre == array_natural[i]){
    //            grafo->array_nat[i] = j;
    //            break;
    //        }
    //    }
    //}

    return grafo;
}

Grafo CopiarGrafo(Grafo G){
    //allocate memory
    Grafo new_grafo = malloc(sizeof(GrafoSt));
    new_grafo->cant_ver = G->cant_ver;
    new_grafo->cant_lad = G->cant_lad;

    new_grafo->orden = malloc(sizeof(u32)*NumeroDeVertices(G));
    //memcpy(new_grafo->orden, G->orden, NumeroDeVertices(G));

    new_grafo->array_nat = malloc(sizeof(u32)*NumeroDeVertices(G));
    //memcpy(new_grafo->array_nat, G->array_nat, NumeroDeVertices(G));

    new_grafo->nodos_array = malloc(sizeof(NodoSt)*NumeroDeVertices(G));

    //copy content

    for (int j=0; j<NumeroDeVertices(G); j++){
        new_grafo->orden[j] =  G->orden[j];
        new_grafo->array_nat[j] =  G->array_nat[j];
        new_grafo->nodos_array[j].vecinos = malloc(sizeof(LadoConPeso) * Grado(j,G)); 
	    new_grafo->nodos_array[j].nombre = G->nodos_array[j].nombre;
	    new_grafo->nodos_array[j].grado = G->nodos_array[j].grado;
	    new_grafo->nodos_array[j].color = G->nodos_array[j].color;
	    for (int k=0; k<new_grafo->nodos_array[j].grado;k++){
		    new_grafo->nodos_array[j].vecinos[k].vecino = G->nodos_array[j].vecinos[k].vecino; 
		    new_grafo->nodos_array[j].vecinos[k].peso = G->nodos_array[j].vecinos[k].peso;
	    }
    }
    return new_grafo;
}

u32 PesoLadoConVecino(u32 j,u32 i,Grafo G){
    return G->nodos_array[G->orden[i]].vecinos[j].peso;
}
u32 FijarPesoLadoConVecino(u32 j,u32 i,u32 p,Grafo G){
    G->nodos_array[G->orden[i]].vecinos[j].peso = p;
    return 0;
}
