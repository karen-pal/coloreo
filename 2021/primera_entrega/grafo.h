
#ifndef __g_H
#define __g_H
#include "GrafoSt21.h"
#include <stdbool.h>
typedef struct{
	u32 nodo1;
	u32 nodo2;
} Tupla;
typedef struct{
	Tupla * array_nodos;
	bool result;
} Result;


int existe_nodo (Grafo grafo,int i, u32 first_node_name);
void inicializar_nodo(Grafo grafo, int array_index, int nodo1, int nodo2);
void agregar_vecino(Grafo grafo, int index, int nodo);

Result GraphParse(Grafo grafo, FILE *stream, Tupla * array_nodos);
int cmpfunc (const void * a, const void * b);
u32* ContarGrados(Tupla * array_nodos, int largo, int lados);


#endif
