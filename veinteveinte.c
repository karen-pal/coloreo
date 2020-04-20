#include "veinteveinte.h"

u32 NumeroDeVertices(Grafo G) {
    return G->cant_ver;
}

u32 NumeroDeLados(Grafo G) {
    return G->cant_lad;
}

u32 Delta(Grafo G) {
    u32 delta = 0;
    for ( int index = 0; index<NumeroDeVertices(G); index++) {
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
        u32 array_natural[NumeroDeVertices(G)];
        for (u32 indice = 0; indice < NumeroDeVertices(G); indice++) {
            array_natural[indice] = G->orden[indice];
        }
        for (u32 indice = 0; indice < NumeroDeVertices(G)-1; indice++) {
            u32 indice_minimo = indice;
            for (u32 indice2 = indice + 1; indice2 < NumeroDeVertices(G); indice2++) {
                if (Nombre(array_natural[indice2], G) < Nombre(array_natural[indice_minimo], G)) {
                    indice_minimo = indice2;
                }
            }
            u32 temp = array_natural[indice];
            array_natural[indice] = array_natural[indice_minimo];
            array_natural[indice_minimo] = temp;
        }
        G->orden[i] = array_natural[N];
        return 0;
    } else {
        return 1;
    }
} 

u32 ColorVecino(u32 j,u32 i,Grafo G) { 
    if (i >= NumeroDeVertices(G) || j >= G->nodos_array[G->orden[i]].grado) {
        return -1;
    } else {
        int index = 0;
        while (G->nodos_array[G->orden[i]].vecinos[j] != G->nodos_array[index].nombre) {
            index++;
        }
         return Color(index, G);
    }
}

u32 NombreVecino(u32 j,u32 i,Grafo G) {
    return G->nodos_array[G->orden[i]].vecinos[j];
}

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

char ChicoGrandeBC(Grafo G) {
    //array_natural tiene los nombres de nodos ordenados
    u32 *array_natural = Natural(G);
    u32 *array_cg = malloc(sizeof(u32)*NumeroDeVertices(G));
    u32 max_color = MaxColor(G);
    u32 indice_cg = 0;
    for (u32 i = 0; i <= max_color; i++) {
        for (u32 j = 0; j < NumeroDeVertices(G); j++) {
            if (Color(j, G) == i) {
                array_cg[indice_cg] = Nombre(j, G);
                indice_cg++;
            }
        }
    }
    indice_cg = 0;
    for (u32 i = 0; i < NumeroDeVertices(G); i++) {
        for (u32 j = 0; j < NumeroDeVertices(G); j++) {
            if (array_cg[i] == array_natural[j]) {
                FijarOrden(indice_cg, G, j);
                indice_cg++;
                break;
            }
        }
    }
    for (u32 j = 0; j < NumeroDeVertices(G); j++) {
        printf("ARRAYCG: %u\n", array_cg[j]);
    }

    return 0;
}



u32 OrdenVecino(u32 j,u32 i,Grafo G) {
    u32 vecino = NombreVecino(j, i, G);
    for (u32 indice = 0; indice < NumeroDeVertices(G); indice++) {
        if (vecino == Nombre(indice, G)) {
            return indice;
        }
    }
}
u32 Greedy(Grafo G) {
    FijarColor(0,0,G);
    u32 max_color = 0;
    for (u32 i = 1; i < NumeroDeVertices(G); i++) {
        u32 color = 0;
        for (int j = 0; j < Grado(i,G); j++) {
            if (OrdenVecino(j, i, G) < i) {
                if (ColorVecino(j, i, G) == color) {
                    color++;
                    j = -1;
                }
            }
        }
        if (color > max_color) {
            max_color = color;
        }
        FijarColor(color, i, G);
    }
    return max_color + 1;
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


 /*   csa
    //en el orden pasar por cada nodo
    //si es el primero ponerle color 0
    //al siguiente, fijarse si ponerle el color min interfiere
    //con el coloreo que ya está en el grafo
    //darle el color min tal que no joda

    return actual_color;
};

//preguntar si greedy y el resto de las cosas modifican al grafo que se le pasa 
//OJO
//PREGUNTAR TIPO DEL RETURN
//usar el del apunte >:(
u32 Bipartito(Grafo G) {
    //asumiendo que el reordenamiento disminuye numcrom (y no lo aumenta)
    for i do n times:
        numCrom[i] = greedy(G);
        AleatorizarOrden(G, R);
    //calcular greedy de G en varios órdenes
    colorMin = min(numCrom)
    if colorMin == 2 return 1;
    else return 0;
};
*/
u32 NumCCs(Grafo G) {
    u32 vertices_cc[NumeroDeVertices(G)];
    vertices_cc[0] = Nombre(0, G); 
    u32 indice = 1;
    for (u32 i = 0; i < Grado(0, G); i++) {
        vertices_cc[indice] = NombreVecino(i, 0 ,G);
        indice++;
    }
    for(u32 i = 0; i < NumeroDeVertices(G); i++) {
        for (u32 j = 0; j < indice; j++) {
            
    }
    for (u32 i = 0; i < NumeroDeVertices(G); i++) {
        printf("NUMCC: %u\n", vertices_cc[i]);
    }
    //calcular bnfs para un nodo. guardar los nodos de su comp conexa
    //calcular bnfs para el siguiente nodo que no está en alguna comp
    //conexa ya calculada.
    //seguir hasta que no haya más nodos a recorrer
    //la cant de estos conjuntos de comp conexas va a ser la cant de comp conexas de G
    return 0;
};
/*
char WelshPowell(Grafo G) {
    
}


/*u32 chicogrande = malloc(sizeof(u32)*NumeroDeVertices(G));
    chicogrande tiene los nombres de nodos ordenados con c
    int min = 0;
    int en_su_lugar = -1;
    for (int j= 0; j<NumeroDeVertices(G)-1; j++){
        for (int k=1; k<NumeroDeVertices(G); k++){
            if (Color(j,G)<=Color(k,G)) {
                en_su_lugar = 1;
            
            } else {
                faltan[j] = j;
                en_su_lugar = 0;
                break; //fijarse que se pongan todos
            }

    
    }
    //para fijar orden
    for (i=0; i<NumVertices(G);i++){
    //agregar chicogrande[i] viendo posicion en donde  está en nat    ural
        for (j=0; j<NumVertices(G);j++){
            if (natural[j] == chicogrande[i]){
                posicion=j;
                break;
            }
        }
        FijarOrden(i,G,posicion);
    }*/

    /*
    i=0, chicogande[i] = 1,
      j=0, natural[j] = 1

    i=1, chicogrande[i]=3
        j=0, natural[j]=1
        j=1, natural[j]=2
        j=2, natural[j]=3
    */
