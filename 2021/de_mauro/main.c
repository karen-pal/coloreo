#include "GrafoSt21.h"
#include "RomaVictor.h"
#include "requeridas.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

int main (void) {
    bool test_copia = false;

    Grafo grafo = ConstruccionDelGrafo();
    if (grafo == NULL){
	return 1;
    }
    printf("Vert %u Lados %u\n",NumeroDeVertices(grafo),NumeroDeLados(grafo));
    //imprimir_todo(grafo);
    
    //test_caso_6(grafo);
    //test_pesos(grafo);

    if (test_copia){
	printf(">>>>Copiando\n");
        //imprimir_todo(grafo);
	Grafo copia = CopiarGrafo(grafo);
    	//printf("\nCopiado grafo. Test 8 \n\n");
	//test_caso_8(grafo,copia);
	DestruccionDelGrafo(copia);
        DestruccionDelGrafo(grafo);
	return 0;
    }
    //char bipartito = Bipartito(grafo);
    //printf("----> Es bipartito? %d\n", bipartito);
    u32 greedy = Greedy(grafo);
    printf("----> greedy %u\n", greedy);

    //testear numero de lados y de vertices
    /*
    printf("\nCaso 2\n");
    printf("Segun NumeroDeVertices, nodes=%u \nSegun NumeroDeLados, edges=%u\n", NumeroDeVertices(grafo), NumeroDeLados(grafo));
    //testear orden y fijar Orden 
    printf("\n\n");
    test_caso_3(grafo);
    test_caso_6(grafo);
    printf("\n\n");
    test_caso_7(grafo,6,1);
    */
    DestruccionDelGrafo(grafo);
    return 0;
}
