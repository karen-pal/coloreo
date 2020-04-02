#include <stdio.h>
#include <stdlib.h>

int lines(FILE *file){
    int lines = 0;
    //in_file = fopen(file, "r");
    char buff_lett[100];
    char buff_num[200];
    char buff_num2[200];
    while (fscanf(file, "%s %s %s", buff_lett, buff_num, buff_num2) != EOF) {
        printf("%s %s %s\n", buff_lett,buff_num,buff_num2);
        if (buff_lett=="\n"){
            lines++;
        }
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
    fscanf(in_file, "%s %s %s", buff_lett, buff_num ,buff_num2);
    printf("%s %s %s\n", buff_lett,buff_num,buff_num2);
    int line_num = lines(in_file);
    printf("%d\n", line_num);
    /*
    printf("%s\n", buff);
    int i;
    fscanf(in_file, "%d", &i);
    printf("%d\n",i);
    fscanf(in_file, "%d", &i);
    printf("%d\n",i);*/
    return 0;
}
