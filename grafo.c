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
    char line[8];
    int file_length = line_count(in_file);
    char *file_array[file_length];
    int i = 0;

    while (fgets(line,8,in_file) != NULL) {
        file_array[i] = line;
        printf("una linea de largo %ld:\n", strlen(file_array[i]));

        int full_number;
        int full_number1;
        int full_number2;
        int flip=0;
        for (int j = 1; j<strlen(file_array[i]); j++) {
            if (isdigit(file_array[i][j]) && isdigit(file_array[i][j-1])) {
                int digit1 = (int) file_array[i][j-1] - 48;
                int digit2 = (int) file_array[i][j] - 48;
                char digit1_s[25];
                char digit2_s[12];
                sprintf(digit1_s,"%d",digit1);
                sprintf(digit2_s,"%d",digit2);
                strcat(digit1_s,digit2_s);
                if (i%2 == 0 && flip == 0) {
                    full_number1 = atoi(digit1_s);
                    printf("el numero completo 1 es %d\n", full_number1);
                    flip++;
                }
                else {
                    full_number2 = atoi(digit1_s);
                    printf("el numero completo 2 es %d\n", full_number2);
                    flip--;
                }
            }
        }
        int first_node_name;
        int second_node_name;
        if (strlen(file_array[i])>6 && i %2 == 0 ) {
            full_number = full_number1;
        } else {
            full_number = full_number2;
        }
        if (isdigit(file_array[i][3])) {
            printf("wTF s%d\n", full_number);
            first_node_name = full_number;
            if (strlen(file_array[i])>7) {
                second_node_name = full_number;
            } else {
                printf("wTF s%d\n", full_number);
                second_node_name = (int)file_array[i][5]-48;
            }
        } else if (strlen(file_array[i]) > 6) {
            first_node_name = (int)file_array[i][2]-48;
            second_node_name = full_number;
        } else {
            first_node_name = (int)file_array[i][2]-48;
            second_node_name = (int)file_array[i][4]-48;
        }
        printf("%d %d\n",first_node_name, second_node_name);
        i++;
    }

    fclose(in_file);

    return(0);
}
