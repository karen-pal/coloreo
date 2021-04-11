
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GrafoSt21.h"
#include "RomaVictor.h"

enum LineStart {
    LineComment = 'c',
    LineGraphConfig = 'p',
    LineGraphEdge = 'e',
    LineNewLine = '\n',
    LineCarriageReturn = '\r'
};

int existe_nodo (Grafo grafo,int i, u32 first_node_name) {
    int res = -1;
    for (int j=0; j<i; j++) {
        if (grafo->nodos_array[j].nombre == -1){
            break;
        } else if (grafo->nodos_array[j].nombre == first_node_name) {
            res = j;
            break;
        }
    }
    return res;
}

void inicializar_nodo(Grafo grafo, int array_index, int nodo1, int nodo2) {
    grafo->nodos_array[array_index].nombre = nodo1;
    grafo->nodos_array[array_index].grado = 1;
    grafo->nodos_array[array_index].vecinos = malloc(sizeof(LadoConPeso));
    grafo->nodos_array[array_index].vecinos[0].vecino = nodo2;
    grafo->nodos_array[array_index].vecinos[0].peso = 0;
    grafo->orden[array_index] = array_index;
    grafo->nodos_array[array_index].color= 4294967295;//2^32-1;
}

//p edge 3 3
//e 1 2
//e 1 3
//1 --> [(2,0),(3,0)]

void agregar_vecino(Grafo grafo, int index, int nodo){
    grafo->nodos_array[index].grado++;
    grafo->nodos_array[index].vecinos = realloc(grafo->nodos_array[index].vecinos,
                                            grafo->nodos_array[index].grado * sizeof(LadoConPeso));
    grafo->nodos_array[index].vecinos[grafo->nodos_array[index].grado-1].vecino = nodo;
    grafo->nodos_array[index].vecinos[grafo->nodos_array[index].grado-1].peso = 0;
}


bool GraphParse(Grafo grafo, FILE *stream) {
    char line;
    u32 nodes = 0;
    u32 edges = 4294967295;
    u32 node1, node2 = 0;
    int readchars = 0;
    u32 curredge = 0;
    int array_index = 0;
    char edge_string[4];

    // Error handling
    int matched_params = 0;
    while ((readchars = fscanf(stream, "%c", &line)) != 0) {
        if (readchars == EOF || curredge >= edges) {
	    printf("curredge: %d\n", curredge);
	    if (curredge < edges) return false;
            // if it reached the end without an error, the
            // graph was parsed successfully
	    printf(">>>>>>>SUCCESS\n");
            return true;
        }

        switch (line) {
        case LineComment:
            //printf("%s: found a comment, skipping...", __func__);

            // A bit to unwind here, but simplifies things a lot,
            // as it helps us completely skip comments:
            //   '*' means ignore this value, don't try to assign it
            //   '[]' matches a regular expression within
            //   '^\n' means everything from the beginning ('^') til
            //   the newline ('\n')
            fscanf(stream, "%*[^\n]\n");

            break;

        case LineGraphConfig:
            matched_params = fscanf(stream, "%4s%u%u",&edge_string, &nodes, &edges);
	    printf("entro al if?: %s, porque matched params: %d, y comparacion %d\n", matched_params != 3 || strcmp(edge_string, "edge")!=0 ? "true" : "false", matched_params!=3, strcmp(edge_string,"edge"));
            if (matched_params != 3 || strcmp(edge_string, "edge")!=0) {
                printf("couldn't parse config: nodes='%u', edges='%u'\n",nodes,edges);
                return false;
            }

            printf("Found in graph config: nodes=%u, edges=%u\n",
                      nodes, edges);
            grafo->cant_ver = nodes;
            grafo->cant_lad = edges;
            grafo->nodos_array = malloc(grafo->cant_ver * sizeof(NodoSt));
            grafo->orden = malloc(grafo->cant_ver * sizeof(u32));
            break;

        case LineGraphEdge:
            matched_params = fscanf(stream, "%u%u", &node1, &node2);
            if (matched_params != 2) {
                printf("couldn't parse edge: (%u, %u)", node1,
                          node2);
                return false;
            }

            int indice_nodo1 = existe_nodo(grafo,array_index,node1);
            int indice_nodo2 = existe_nodo(grafo,array_index, node2);
            //printf("HOLA SOY PRUEBA %d\n", indice_nodo1);
            //printf("HOLA SOY PRUEBA2 %d\n", indice_nodo2);
            //printf("HOLA SOY INDEX %d\n", array_index);
            if (indice_nodo1 == -1 && indice_nodo2 == -1) {
                inicializar_nodo(grafo,array_index,node1,node2);
                array_index++;
                inicializar_nodo(grafo,array_index,node2,node1);
                array_index++;
            } else if (indice_nodo1 == -1 && indice_nodo2 != -1) {
                inicializar_nodo(grafo,array_index,node1,node2);
                agregar_vecino(grafo, indice_nodo2, node1);
                array_index++;
            } else if (indice_nodo1 != -1 && indice_nodo2 == -1) {
                inicializar_nodo(grafo,array_index,node2,node1);
                agregar_vecino(grafo, indice_nodo1, node2);
                array_index++;
            } else {
                agregar_vecino(grafo, indice_nodo2, node1);
                agregar_vecino(grafo, indice_nodo1, node2);
            }
            curredge++;
            break;

        case LineNewLine:
        case LineCarriageReturn:  // some graphs come from Windows
            break;

        default:
            printf("couldn't parse graph: found '%c'", line);
            return false;
        }
    }
}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}



