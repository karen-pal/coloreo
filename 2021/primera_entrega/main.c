#include "GrafoSt21.h"
#include "RomaVictor.h"
#include <stdio.h>

int main (void) {
    Grafo grafo = ConstruccionDelGrafo();
    if (grafo == NULL){
	return 1;
    }
    for (int index = 0; index<NumeroDeVertices(grafo); index++) {
        printf("NOMBRE: %u \nCOLOR: %u \nGRADO: %u \n", 
                Nombre(index,grafo), Color(index,grafo),Grado(index,grafo));
        for (int vecindex= 0; vecindex < Grado(index,grafo);vecindex++){
            printf("Vecino %u: %u\n", vecindex,NombreVecino(vecindex, index, grafo));
        }
        printf("\n");
    }
    return 0;
}
