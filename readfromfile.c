#include <stdio.h>
#include <stdlib.h>

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
    char letter;

    FILE *in_file;
    in_file = fopen("./huck.corr", "r");
    if (!in_file) {
        printf("ERROR READING FILE\n");
        return 0;
    }

    char buff_lett[100];
    char buff_num[200];
    char buff_num2[200];
    int a;
    int lines = 0;
    int nodos[500];
    while (( a = fscanf(in_file, "%s %s %s", buff_lett, buff_num, buff_num2)) != EOF) {
        nodos[lines]=(int)*buff_num;
        printf("%s %s %s\n", buff_lett,buff_num,buff_num2);
        printf("HOLA %d %d %d\n\n", lines, *buff_num, nodos[lines]);
        /*
        if (buff_lett[0]=='e'){
            printf("wtf\n");
        }
        */
        lines++;
    }
    //printf("%d\n", lines);
    lines = line_count(in_file);
    //printf("CON LINECOUNT %d\n", lines);
    fclose(in_file);
    return 0;
}
