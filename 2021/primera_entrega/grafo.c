#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GrafoSt21.h"
#include "RomaVictor.h"
#include "grafo.h"



Result ParsearGrafo(Grafo grafo, FILE *stream, Tupla * array_nodos) {

    char line;
    u32 nodes = 0;
    u32 edges = 4294967295;
    u32 node1, node2 = 0;
    int readchars = 0;
    u32 curredge = 0;
    char edge_string[4];

    Result res;
    res.result = false;
    res.array_nodos = NULL;

    // Error handling
    int matched_params = 0;
    while ((readchars = fscanf(stream, "%c", &line)) != 0) {
        if (readchars == EOF || curredge >= edges) {
	    if (curredge < edges) return res;
            // if it reached the end without an error, the
            // graph was parsed successfully
	    for (u32 i =edges; i<2*edges;i++){
		array_nodos[i].nodo1 = array_nodos[i-edges].nodo2;
		array_nodos[i].nodo2 = array_nodos[i-edges].nodo1;
	    }
	    res.result = true;
	    res.array_nodos = array_nodos;
            return res;
        }

        switch (line) {
        case 'c':
            //   '*' means ignore this value, don't try to assign it
            //   '[]' matches a regular expression within
            //   '^\n' means everything from the beginning ('^') til
            //   the newline ('\n')
            fscanf(stream, "%*[^\n]\n");

            break;

        case 'p':
            matched_params = fscanf(stream, "%4s%u%u",&edge_string, &nodes, &edges);
            if (matched_params != 3 || strcmp(edge_string, "edge")!=0) {
                printf("couldn't parse config: nodes='%u', edges='%u'\n",nodes,edges);
                return res;
            }

            printf("Found in graph config: nodes=%u, edges=%u\n",
                      nodes, edges);
            grafo->cant_ver = nodes;
            grafo->cant_lad = edges;
            grafo->nodos_array = malloc(grafo->cant_ver * sizeof(NodoSt));
            grafo->orden = malloc(grafo->cant_ver * sizeof(u32));
	    array_nodos= malloc(sizeof(Tupla)*2*edges);
            break;

        case 'e':
            matched_params = fscanf(stream, "%u%u", &node1, &node2);
            if (matched_params != 2) {
                printf("couldn't parse edge: (%u, %u)", node1,
                          node2);
                return res;
            }
	    array_nodos[curredge].nodo1 = node1;
	    array_nodos[curredge].nodo2 = node2;
	    curredge++;
            break;

	case '\n':
	case '\r':  // some graphs come from Windows
            break;

        default:
            printf("couldn't parse graph: found '%c'", line);
            return res;
        }
    }
}

int cmpfunc (const void * a, const void * b) {
    Tupla *A = (Tupla *)a;
    Tupla *B = (Tupla *)b;

  return ( A->nodo1 - B->nodo1 );
}


u32* ContarGrados(Tupla * array_nodos, u32 vert, u32 lados){
    u32 count = 0;
    u32 first = 0;
    u32 index = 0;
    u32* res = calloc(lados,sizeof(u32));
    for (u32 i = 0; i<vert;i++) {
	while((index<lados*2) &&(first < lados*2) && array_nodos[first].nodo1 == array_nodos[index].nodo1){
		count++;
		index++;
	}
	res[i] = count;
	count = 0;
	first = index;
    }
    return res;
}
