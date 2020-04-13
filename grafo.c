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

    NodoSt nodo_1;
    nodo_1.nombre = (uint32_t) 1;
    nodo_1.grado = (uint32_t) 1;
    nodo_1.color = (uint32_t) 0;

    NodoSt nodo_2;
    nodo_2.nombre = (uint32_t) 1;
    nodo_2.grado = (uint32_t) 1;
    nodo_2.color = (uint32_t) 0;

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

    while (fgets(line,9,in_file) != NULL) {
        file_array[i] = line;
        printf("una linea de largo %ld:\n", strlen(file_array[i]));

        int full_number;
        int full_number1;
        int full_number2;
        int flip=0;
        int first_node_name;
        int second_node_name;
        first_node_name = (int)file_array[i][2]-48;
        second_node_name = (int)file_array[i][4]-48;
        printf("%d %d\n",first_node_name, second_node_name);
        i++;
    }

    fclose(in_file);

    return(0);
}
