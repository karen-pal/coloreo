
#include "RomaVictor.h"
#include "requeridas.h"
#include <stdbool.h>
#include <time.h>




int main() {
    printf("\n");
    printf("Cargando grafo desde stdin...\n");
    printf("\n");

    Grafo grafo = ConstruccionDelGrafo();
    bool test_1000_greedys = false;
    bool dar_info_detallada = false;
    bool test_bipartito = false;
    bool test_entrega3= true;
    if (test_entrega3){
        u32 * perm_que_sea = malloc(4*sizeof(u32));
        int j=0;
        for (int i = 3; i>-1;i--){
            perm_que_sea[j] = i;
            j++;
        }
        for (int i=0; i<4; i++){
            printf("%d:%d\n",i,perm_que_sea[i]);
        }
        printf("Resultados del que SI es perm:\n");
    	//AleatorizarVertices(grafo,1);
        printf("Imprimiendo grafo original...\n");
        for (int j=0; j<NumeroDeVertices(grafo); j++){
            printf("%d: %u\n", j, Nombre(j,grafo));
        }
        char hol = OrdenPorBloqueDeColores2(grafo,perm_que_sea);
        free(perm_que_sea);

        //printf("Resultados del que NO es perm:\n");

        //u32 * perm_que_no_sea[4];
        //perm_que_no_sea[0]=10;
        //for (int k=1; k<4; k++){
        //    perm_que_no_sea[k] = k;
        //}
        //for (int i=0; i<4; i++){
        //    printf("%d:%d\n",i,perm_que_no_sea[i]);
        //}
        //char hol2 = OrdenPorBloqueDeColores(grafo, perm_que_no_sea);
    }
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

    if (test_bipartito) {
        char bip = Bipartito(grafo);
        printf("Es bipartito? %d\n",(bool)bip);
    }
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
    AleatorizarVertices(grafo,1);
    u32 greedy2 = Greedy(grafo);
    printf("----> SEGUNDO greedy %u\n", greedy2);
    if (dar_info_detallada) {
        printf("Colores despues segundo greedy \n");
        printf("\n");
        for (int i=0; i<NumeroDeVertices(grafo);i++){
            printf("Vertice: %i --- Color: %u\n", Nombre(i,grafo), Color(i,grafo));
        }
    }
    if (test_bipartito) {
        char bip = Bipartito(grafo);
        printf("Es bipartito? %d\n",(bool)bip);
    }
    if (test_1000_greedys){
	    for (int i = 0; i<1000; i++){
    		AleatorizarVertices(grafo,i);
		printf("seed %i greedy da %d\n",i, Greedy(grafo));
		//printf("----> %d greedy %u\n",i, Greedy(grafo));
	    }
    }
    DestruccionDelGrafo(grafo);

    return 0;
}
