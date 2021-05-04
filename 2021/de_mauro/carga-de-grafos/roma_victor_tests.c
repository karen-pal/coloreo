
/** A este archivo lo hago para probar si funciona bien la forma en que compilo y linkeo
 * el programa.
 * No me queda claro de la consigna cómo va a ser, por eso lo intenté hacer de la forma más simple que
 * se pueda: basta compilar el archivo `index.c` en un `.o` y linkearlo al momento de compilar otro archivo.
 */

#include "RomaVictor.h"
#include "requeridas.h"
#include <stdbool.h>
#include <time.h>

double get_time() {
    return (double)clock()/CLOCKS_PER_SEC;
}




int main() {
    printf("\n");
    printf("Cargando grafo desde stdin...\n");
    printf("\n");

    Grafo grafo = ConstruccionDelGrafo();
    bool test_1000_greedys = false;
    bool dar_info_detallada = false;
    bool test_bipartito = false;
    if (test_bipartito) {
	char bip = Bipartito(grafo);
	printf("Es bipartito? %d\n",(bool)bip);
    }
    if (grafo == NULL) {
        fprintf(stderr, "error: No se puede leer el grafo\n");
        return -1;
    }
    printf("Se leyó el grafo\n");
    printf("\n");

    u32 greedy = Greedy(grafo);
    printf("----> greedy %u\n", greedy);

    if (dar_info_detallada) {
	    printf("Número de vértices: %u\n", NumeroDeVertices(grafo));
	    printf("Número de lados: %u\n", NumeroDeLados(grafo));
	    printf("Delta: %u\n", Delta(grafo));
	    printf("\n");
	    if (NumeroDeVertices(grafo)<20){
		    printf("Colores antes \n");
		    printf("\n");
		    for (u32 i=0; i<NumeroDeVertices(grafo);i++){
			printf("Vertice: %i --- Color: %u\n", Nombre(i,grafo), Color(i,grafo));
		    }
	    }
	    printf("\n");
    }

    if (dar_info_detallada) {
        printf("Colores despues \n");
        printf("\n");
        for (u32 i=0; i<NumeroDeVertices(grafo);i++){
            printf("Vertice: %i --- Color: %u\n", Nombre(i,grafo), Color(i,grafo));
        }
    } 
    //AleatorizarVertices(grafo,3);
    u32 greedy2 = Greedy(grafo);
    printf("----> SEGUNDO greedy %u\n", greedy2);
    if (dar_info_detallada) {
        printf("Colores despues segundo greedy \n");
        printf("\n");
        for (int i=0; i<NumeroDeVertices(grafo);i++){
            printf("Vertice: %i --- Color: %u\n", Nombre(i,grafo), Color(i,grafo));
        }
    }
    if (test_1000_greedys){
	    for (int i = 0; i<10; i++){
		Greedy(grafo);
		//printf("----> %d greedy %u\n",i, Greedy(grafo));
	    }
    }
    DestruccionDelGrafo(grafo);

    return 0;
}
