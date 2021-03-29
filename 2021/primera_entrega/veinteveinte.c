#include "veinteveinte.h"



u32* Natural(Grafo G) {
    u32 *array_natural = malloc(sizeof(u32)*NumeroDeVertices(G));
    for (u32 indice = 0; indice < NumeroDeVertices(G); indice++) {
            array_natural[indice] = Nombre(indice, G);
    }
    for (u32 indice = 0; indice < NumeroDeVertices(G)-1; indice++) {
        u32 indice_minimo = indice;
        for (u32 indice2 = indice + 1; indice2 < NumeroDeVertices(G); indice2++) {
            if (array_natural[indice2] < array_natural[indice_minimo]) {
                indice_minimo = indice2;
            }
        }
        u32 temp = array_natural[indice];
        array_natural[indice] = array_natural[indice_minimo];
        array_natural[indice_minimo] = temp;
    }
    return array_natural;
}

u32 MaxColor(Grafo G) {
    u32 max_color = 0 ;
    for (u32 indice = 0; indice < NumeroDeVertices(G); indice++) {
        if (Color(indice, G) > max_color) {
            max_color = Color(indice, G);
        }
    }
    return max_color;
}
u32 NumeroDeVertices(Grafo G) {
    return G->cant_ver;
}

u32 NumeroDeLados(Grafo G) {
    return G->cant_lad;
}

char NaturalOr(Grafo G) {
    //array_natural tiene los nombres de nodos ordenados
    u32 *array_natural = Natural(G);
    u32 *array_natural2 = Natural(G);
    for (u32 i = 0; i < NumeroDeVertices(G); i++) {
        for (u32 j = 0; j < NumeroDeVertices(G); j++) {
            if (array_natural2[i] == array_natural[j]) {
                FijarOrden(i, G, j);
                break;
            }
        }
    }
    free(array_natural);
    free(array_natural2);
    return 0;

}

u32 Delta(Grafo G) {
    u32 delta = 0;
    for (u32 index = 0; index<NumeroDeVertices(G); index++) {
        if (G->nodos_array[index].grado > delta) {
            delta = G->nodos_array[index].grado;
        }
    }
    return delta;
}

u32 Nombre(u32 i, Grafo G) {
    return G->nodos_array[G->orden[i]].nombre;
}

u32 Color(u32 i, Grafo G) {
    return G->nodos_array[G->orden[i]].color;
}

u32 Grado(u32 i, Grafo G) {
    return G->nodos_array[G->orden[i]].grado;
}

char FijarColor(u32 x,u32 i,Grafo G) {
    if (i < NumeroDeVertices(G)) {
        G->nodos_array[G->orden[i]].color = x;
        return 0;
    } else {
        return 1;
    }
}

char FijarOrden(u32 i,Grafo G,u32 N) {
    if (i < NumeroDeVertices(G) && N < NumeroDeVertices(G)) {
        G->orden[i] = G->array_nat[N];
        return 0;
    } else {
        return 1;
    }
}

u32 ColorVecino(u32 j,u32 i,Grafo G) {
    if (i >= NumeroDeVertices(G) || j >= G->nodos_array[G->orden[i]].grado) {
        return (2^32)-1;
    } else {
        u32 index = 0;
        while (G->nodos_array[G->orden[i]].vecinos[j] != G->nodos_array[G->orden[index]].nombre) {
            index++;
        }
         return Color(index, G);
    }
}

u32 NombreVecino(u32 j,u32 i,Grafo G) {
    return G->nodos_array[G->orden[i]].vecinos[j];
}




u32 OrdenVecino(u32 j,u32 i,Grafo G) {
    u32 vecino = NombreVecino(j, i, G);
    for (u32 indice = 0; indice < NumeroDeVertices(G); indice++) {
        if (vecino == Nombre(indice, G)) {
            return indice;
        }
    }
}



char SwitchColores(Grafo G,u32 i,u32 j) {
    u32 max_color = MaxColor(G);
    if (i <= max_color && j <= max_color) {
        if (i != j) {
            for (u32 indice = 0; indice < NumeroDeVertices(G); indice++) {
                if (Color(indice, G) == i) {
                    FijarColor(j, indice, G);
                } else if (Color(indice, G) == j) {
                    FijarColor(i, indice, G);
                }
            }
        }
        return 0;
    }
    return 1;
}

