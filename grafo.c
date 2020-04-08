
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//https://www.tutorialspoint.com/c_standard_library/c_function_isdigit.htm

int line_count(FILE *file){
    int lines = 0;
    rewind(file);

    char buff_lett[100];
    char buff_num[200];
    char buff_num2[200];
    while (fscanf(file, "%s %s %s", buff_lett, buff_num, buff_num2) != EOF) {
        lines=lines+1;
    }

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
    //printf("%d \n",file_length);
    int i = 0;

    while (fgets(line,8,in_file) != NULL) {
        puts(line);
    }

    fclose(in_file);
    /*
    for (int j=0;j<file_length;j++) {
        for (int k=0; k<strlen(file_array[j]); k++) {
            if (isdigit(file_array[j][k])) {
                printf("%d",file_array[j][k]);
            }
        }
    }
    */
    return(0);
}
