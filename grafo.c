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

    FILE *in_file;
    in_file = fopen("./huckSm.corr", "r");
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

        for (int j = 1; j<strlen(file_array[i]); j++) {
            if (isdigit(file_array[i][j]) && isdigit(file_array[i][j-1])) {
                int digit1 = (int) file_array[i][j-1] - 48;
                int digit2 = (int) file_array[i][j] - 48;
                char full_number[25];
                char digit2_s[12];
                sprintf(full_number,"%d",digit1);
                sprintf(digit2_s,"%d",digit2);
                strcat(full_number,digit2_s);
                printf("el numero completo es %d\n", atoi(full_number));
            }
        }
        
        int first_node_name = (int)file_array[i][2]-48;
        int second_node_name = (int)file_array[i][4]-48;
        printf("%d %d\n",first_node_name, second_node_name);
        i++;
    }

    fclose(in_file);

    return(0);
}
