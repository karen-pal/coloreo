#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//https://www.tutorialspoint.com/c_standard_library/c_function_isdigit.htm
#include "grafoSmall.h"

int line_count(FILE *file){
    int lines = 0;

    char buff_lett[100];
    char buff_num[200];
    char buff_num2[200];
    rewind(file);
    while (fscanf(file, "%s %s %s", buff_lett, buff_num, buff_num2) != EOF) {
        lines=lines+1;
    }
    rewind(file);
    return lines;
}


int main(void) {

    GrafoSt *grafo = malloc(sizeof(GrafoSt));
    NodoSt *nodo_1 = malloc(sizeof(NodoSt));
    nodo_1->nombre = (uint32_t) 1;
    nodo_1->grado = (uint32_t) 1;
    nodo_1->color = (uint32_t) 0;

    FILE *in_file;
    in_file = fopen("./grafoUno.g", "r");
    if (!in_file) {
        printf("ERROR READING FILE\n");
        return(-1);
    }
    char line[9];
    int file_length = line_count(in_file);
    char *file_array[file_length];
    int i = 0;
    int array_index = 0;
    while (fgets(line,9,in_file) != NULL) {
        file_array[i] = line;
        if (i == 0) {
            grafo->cant_ver = (uint32_t) ((int)file_array[i][2]-48);
            grafo->cant_lad =  (uint32_t) ((int)file_array[i][4]-48);
            grafo->nodos_array = malloc(grafo->cant_ver * sizeof(NodoSt));

        } else {
            printf("una linea de largo %ld:\n", strlen(file_array[i]));

            int first_node_name;
            int second_node_name;
            first_node_name = (int)file_array[i][2]-48;
            second_node_name = (int)file_array[i][4]-48;
            printf("%d %d\n",first_node_name, second_node_name);
            int is_already_there=0;
            for (int j=0; j<i; j++) {
                if (grafo->nodos_array[j].nombre == first_node_name) {
                    is_already_there=1;
                    break;
                }
            }
            if (is_already_there == 0) {
                grafo->nodos_array[array_index].nombre = first_node_name;
                printf("el nodo se llama %d\n", grafo->nodos_array[array_index].nombre);
                array_index++;
            }
        }
        i++;
    }

    fclose(in_file);

    return(0);
}
