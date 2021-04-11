#include "GrafoSt21.h"
#include "RomaVictor.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
void imprimir_todo(Grafo grafo){

        for (int index = 0; index<NumeroDeVertices(grafo); index++) {
            printf("* NOMBRE: %u \n* COLOR: %u \n* GRADO: %u \n", 
                    Nombre(index,grafo), Color(index,grafo),Grado(index,grafo));
            for (int vecindex= 0; vecindex < Grado(index,grafo);vecindex++){
                printf("Vecino %u: %u\n", vecindex,NombreVecino(vecindex, index, grafo));
            }
            printf("\n");
        }
}
/*void imprimir_todo(Grafo grafo){

        for (int index = 0; index<NumeroDeVertices(grafo); index++) {
            printf("* NOMBRE: %u \n* COLOR: %u \n* GRADO: %u \n", 
                    grafo->nodos_array[index].nombre, grafo->nodos_array[index].color,grafo->nodos_array[index].grado);
            for (int vecindex= 0; vecindex < grafo->nodos_array[index].grado;vecindex++){
                printf("Vecino %u: %u\n", vecindex,grafo->nodos_array[index].vecinos[vecindex].vecino);
            }
            printf("\n");
        }

}*/
void test_caso_3(Grafo grafo){
    int sum = 0;
    for (int j=0; j<NumeroDeVertices(grafo);j++){
	sum+=Grado(j,grafo);
    }
    printf("\nCaso 3\n");
    printf("La suma %d deberia ser igual a %d\n", sum, 2*NumeroDeLados(grafo));
} 

void test_caso_7(Grafo grafo, int grado_primero, int grado_ultimo){
    printf(">>>>>>>>>>ORDEN ANTES\n\n");
    for (int i=0; i<NumeroDeVertices(grafo);i++){
	printf("*lugar %d: %u\n", i, Nombre(i,grafo));

    }
    printf("%u tendrìa que ser %d\n",Grado(0,grafo), grado_primero);
    if (NumeroDeLados(grafo)<100 && NumeroDeVertices(grafo)<100) {
        printf(">>>>>>>>>>Detallado ANTES\n");
        imprimir_todo(grafo);
    }

    printf("\nCaso 7\n");
    printf("[*] swap primero (%u) y ultimo(%u)\n",Nombre(0,grafo),Nombre(NumeroDeVertices(grafo)-1,grafo));
    FijarOrden(0,grafo, 7);
    FijarOrden(NumeroDeVertices(grafo)-1,grafo,0);
    printf(">>>>>>>>>>ORDEN DESPUES\n\n");
    for (int i=0; i<NumeroDeVertices(grafo);i++){
	printf("*lugar %d: %u\n", i, Nombre(i,grafo));

    }
    printf("%u tendrìa que ser %d\n",Grado(0,grafo),grado_ultimo);
    if (NumeroDeLados(grafo)<100 && NumeroDeVertices(grafo)<100) {
        printf(">>>>>>>>>>Detallado DESPUES\n");
        imprimir_todo(grafo);
    }

}
void test_caso_6(Grafo grafo){
	printf("\n\nCASO 6\n");
	printf("ANTES DE COLOREAR\n");
	for (int j=0; j<NumeroDeVertices(grafo);j++){
		printf("* %u:%u\n",Nombre(j,grafo),Color(j,grafo));
	}
	for (int i=0; i<NumeroDeVertices(grafo);i++){
		FijarColor(i,i, grafo);
	}
	printf("DESPUES DE COLOREAR\n");
	for (int j=0; j<NumeroDeVertices(grafo);j++){
		printf("* %u:%u\n",Nombre(j,grafo),Color(j,grafo));
	}

}

void test_caso_8(Grafo grafo, Grafo copia){
	test_caso_7(grafo,6,1);
	FijarColor(11,0,grafo);
	printf(">>>>Original despues de reorden y color\n");
	imprimir_todo(grafo);
	test_caso_7(copia,6,1);
	FijarColor(4,0,copia);
	printf(">>>>copia despues de reorden y color\n");
	imprimir_todo(copia);
}

void test_pesos(Grafo grafo){
	printf("\n\nEmpiezo a testear pesos\n");
	int ctr = 0;
	for(int i=0; i<NumeroDeVertices(grafo);i++){
		for (int j=0; j<Grado(i,grafo);j++){
			FijarPesoLadoConVecino(j,i,ctr,grafo);
			ctr++;
		}
	}
	for(int i=0; i<NumeroDeVertices(grafo);i++){
		for (int j=0; j<Grado(i,grafo);j++){

				printf("Vert %u --- Vert %u : %u\n",Nombre(i,grafo),NombreVecino(j,i,grafo),PesoLadoConVecino(j,i,grafo));

		}
	}

}

int main (void) {
    bool test_copia = false;

    Grafo grafo = ConstruccionDelGrafo();
    if (grafo == NULL){
	return 1;
    }
    imprimir_todo(grafo);
    /*
    test_caso_6(grafo);
    test_pesos(grafo);

    if (test_copia){
	printf(">>>>Original\n");
        imprimir_todo(grafo);
	Grafo copia = CopiarGrafo(grafo);
    	printf("\n\n");
	test_caso_8(grafo,copia);
	return 0;
    }
    //testear numero de lados y de vertices
    printf("\nCaso 2\n");
    printf("Segun NumeroDeVertices, nodes=%u \nSegun NumeroDeLados, edges=%u\n", NumeroDeVertices(grafo), NumeroDeLados(grafo));
    //testear orden y fijar Orden 
    printf("\n\n");
    test_caso_3(grafo);
    test_caso_6(grafo);
    printf("\n\n");
    test_caso_7(grafo,6,1);
    */
    return 0;
}
