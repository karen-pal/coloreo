#include <stdio.h>
#include <stdlib.h>

int lines(FILE *file){
    int lines = 0;
    in_file = fopen(file, "r");
    while (scanf(file, "%s %d %d", &buff)==1) {
        if (buff=="\n"){
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
    char buff[100];
    fscanf(in_file, "%s", buff);
    printf("%s\n", buff);
    int i;
    fscanf(in_file, "%d", &i);
    printf("%d\n",i);
    fscanf(in_file, "%d", &i);
    printf("%d\n",i);
    return 0;
}
