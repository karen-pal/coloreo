#include "GrafoSt21.h"
#include "RomaVictor.h"
#include <stdio.h>
#include <assert.h>
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
int main (void) {

    Grafo grafo = ConstruccionDelGrafo();
    if (grafo == NULL){
	return 1;
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
    return 0;
}
