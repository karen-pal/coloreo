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
            /*inicializacion del array*/
            for (int index = 0; index < grafo->cant_ver; index++) {
                grafo->nodos_array[index].nombre= -1;
                grafo->nodos_array[index].vecinos= NULL;
                grafo->nodos_array[index].grado= 0;
                grafo->nodos_array[index].color= 4294967295;// ???!!2^32-1;
            }
        } else {
            //printf("una linea de largo %ld:\n", strlen(file_array[i]));
            int first_node_name;
            int second_node_name;
            first_node_name = (int)file_array[i][2]-48;
            second_node_name = (int)file_array[i][4]-48;
            printf("%d %d\n",first_node_name, second_node_name);
            int is_already_there=0;
            int j;
            for (j=0; j<i-1; j++) {
                if (grafo->nodos_array[j].nombre == -1){
                    break;
                }
                if (grafo->nodos_array[j].nombre == first_node_name) {
                    is_already_there=1;
                    break;
                } 
            }
            //printf("HOLA SOY J %d\n", j);
            if (is_already_there == 0) {
                grafo->nodos_array[array_index].nombre = first_node_name;
                grafo->nodos_array[array_index].color = 4294967295;//2^32 -1;???!!
                grafo->nodos_array[array_index].grado = 1;
                grafo->nodos_array[array_index].vecinos = malloc(sizeof(uint32_t));
                grafo->nodos_array[array_index].vecinos[0] = second_node_name;
                
                //printf("el nodo se llama %d y es vecino de %d \n", grafo->nodos_array[array_index].nombre, 
                //   grafo->nodos_array[array_index].vecinos[0]);
                array_index++;
            } else {
                grafo->nodos_array[j].grado++;
                grafo->nodos_array[j].vecinos = realloc(grafo->nodos_array[j].vecinos,
                                                        grafo->nodos_array[j].grado * sizeof(uint32_t));
                grafo->nodos_array[j].vecinos[grafo->nodos_array[j].grado-1] = second_node_name;
            }
        }
        i++;
    }
    //esto es para ver nomas... ALABADO SEAN LOS PRINTS
    for (int index = 0; index<grafo->cant_ver; index++) {
        printf("NOMBRE: %d \nCOLOR: %d \nGRADO: %d \n", 
                grafo->nodos_array[index].nombre, grafo->nodos_array[index].color,grafo->nodos_array[index].grado);
        for (int vecindex= 0; vecindex < grafo->nodos_array[index].grado;vecindex++){
            printf("Vecino %d: %d\n", vecindex,grafo->nodos_array[index].vecinos[vecindex]);
        }
        printf("\n");
    }

    fclose(in_file);

    return(0);
}
