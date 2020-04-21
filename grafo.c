#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//https://www.tutorialspoint.com/c_standard_library/c_function_isdigit.htm
#include "GrafoSt2020.h"
#include "veinteveinte.h"

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

//si el nodo no existe devuelve -1, si existe devuelve su indice
int existe_nodo (Grafo grafo,int i, int first_node_name) {
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
    grafo->nodos_array[array_index].vecinos = malloc(sizeof(u32));
    grafo->nodos_array[array_index].vecinos[0] = nodo2;
    grafo->orden[array_index] = array_index;
}

void agregar_vecino(Grafo grafo, int index, int nodo){
    grafo->nodos_array[index].grado++;
    grafo->nodos_array[index].vecinos = realloc(grafo->nodos_array[index].vecinos,
                                            grafo->nodos_array[index].grado * sizeof(u32));
    grafo->nodos_array[index].vecinos[grafo->nodos_array[index].grado-1] = nodo;
}


int main(void) {

    Grafo grafo = malloc(sizeof(GrafoSt));

    FILE *in_file;
    in_file = fopen("./test1.corr", "r");
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
            grafo->cant_ver = (u32) ((int)file_array[i][2]-48);
            grafo->cant_lad =  (u32) ((int)file_array[i][4]-48);
            grafo->nodos_array = malloc(grafo->cant_ver * sizeof(NodoSt));
            grafo->orden = malloc(grafo->cant_ver * sizeof(u32));
            /*inicializacion del array*/
            for (int index = 0; index < grafo->cant_ver; index++) {
                grafo->nodos_array[index].nombre= -1;
                grafo->nodos_array[index].vecinos= NULL;
                grafo->nodos_array[index].grado= 0;
                grafo->nodos_array[index].color= 4294967295;//2^32-1;
            }
        } else {
            //printf("una linea de largo %ld:\n", strlen(file_array[i]));
            int first_node_name = (int)file_array[i][2]-48;
            int second_node_name = (int)file_array[i][4]-48;
            printf("%d %d\n",first_node_name, second_node_name);
            int indice_nodo1 = existe_nodo(grafo,array_index,first_node_name);
            int indice_nodo2 = existe_nodo(grafo,array_index, second_node_name);
            /*printf("HOLA SOY PRUEBA %d\n", indice_nodo1);
            printf("HOLA SOY PRUEBA2 %d\n", indice_nodo2);
            printf("HOLA SOY INDEX %d\n", array_index);*/
            if (indice_nodo1 == -1 && indice_nodo2 == -1) {
                inicializar_nodo(grafo,array_index,first_node_name,second_node_name);
                array_index++;
                inicializar_nodo(grafo,array_index,second_node_name,first_node_name);
                array_index++;
            } else if (indice_nodo1 == -1 && indice_nodo2 != -1) {
                inicializar_nodo(grafo,array_index,first_node_name,second_node_name);
                agregar_vecino(grafo, indice_nodo2, first_node_name);
                array_index++;
            } else if (indice_nodo1 != -1 && indice_nodo2 == -1) {
                inicializar_nodo(grafo,array_index,second_node_name,first_node_name);
                agregar_vecino(grafo, indice_nodo1, second_node_name);
                array_index++;
            } else {
                agregar_vecino(grafo, indice_nodo2, first_node_name);
                agregar_vecino(grafo, indice_nodo1, second_node_name);
            }
        }
        i++;
    }
    //esto es para ver nomas... ALABADO SEAN LOS PRINTS
    u32 colores = Greedy(grafo);
    //SwitchColores(grafo, 1, 1);
    for (int index = 0; index<grafo->cant_ver; index++) {
        printf("NOMBRE: %u \nCOLOR: %u \nGRADO: %u \n", 
                grafo->nodos_array[grafo->orden[index]].nombre, grafo->nodos_array[grafo->orden[index]].color,grafo->nodos_array[grafo->orden[index]].grado);
        for (int vecindex= 0; vecindex < grafo->nodos_array[grafo->orden[index]].grado;vecindex++){
            printf("Vecino %u: %u\n", vecindex,grafo->nodos_array[grafo->orden[index]].vecinos[vecindex]);
        }
        printf("\n");
    }
    /*printf("###############################FIJARORDEN 1 3##################\n");
    FijarOrden(1,grafo,3);
    for (int index = 0; index<grafo->cant_ver; index++) {
        printf("NOMBRE: %u \n",
                grafo->nodos_array[grafo->orden[index]].nombre);
        printf("\n");
    }
    */
    printf("###############################################\n");
    //ChicoGrandeBC(grafo);
    //RevierteBC(grafo);
    //WelshPowell(grafo);
    for (int index = 0; index<grafo->cant_ver; index++) {
        printf("NOMBRE: %u \nCOLOR: %u \nGRADO: %u \n", 
                grafo->nodos_array[grafo->orden[index]].nombre, grafo->nodos_array[grafo->orden[index]].color,grafo->nodos_array[grafo->orden[index]].grado);
        for (int vecindex= 0; vecindex < grafo->nodos_array[grafo->orden[index]].grado;vecindex++){
            printf("Vecino %u: %u\n", vecindex,grafo->nodos_array[grafo->orden[index]].vecinos[vecindex]);
        }
        printf("\n");
    }
    //NumCCs(grafo);
    char bip = Bipartito(grafo);
    if (bip == '1') {
        printf("SOY BIPARTITO\n");
    }
    fclose(in_file);
    /*FijarOrden(2, grafo, 2);
    for (int indice = 0; indice < NumeroDeVertices(grafo); indice++) {
        printf("FIJARORDEN: %u\n",Nombre(indice, grafo));
    }*/
    printf("COLORES: %u\n", colores);
    /*
    printf("LADOS: %u\n", NumeroDeLados(grafo));
    //printf("NOMBRE: %u\nCOLOR: %u\nGRADO: %u\n", Nombre(2,grafo), Color(2,grafo), Grado(2,grafo));
    printf("COLOR VECINO: %u\n", ColorVecino(15, 5, grafo));
    printf("NOMBRE VECINO: %u\n", NombreVecino(2, 3, grafo));
    printf("ORDEN VECINO: %u\n", OrdenVecino(2, 3, grafo));*/
    return(0);
}
