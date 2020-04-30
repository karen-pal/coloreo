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

char ChicoGrandeBC(Grafo G) {
    //array_natural tiene los nombres de nodos ordenados
    u32 *array_natural = Natural(G);
    u32 *array_cg = malloc(sizeof(u32)*NumeroDeVertices(G));
    if (array_natural == NULL || array_cg == NULL) {
        return 1;
    }
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
    for (u32 i = 0; i < NumeroDeVertices(G); i++) {
        for (u32 j = 0; j < NumeroDeVertices(G); j++) {
            if (array_cg[i] == array_natural[j]) {
                FijarOrden(i, G, j);
                break;
            }
        }
    }
    free(array_natural);
    free(array_cg);
    return 0;
}

char WelshPowell(Grafo G) {
    //array_natural tiene los nombres de nodos ordenados
    u32 *array_natural = Natural(G);
    u32 *array_wp = malloc(sizeof(u32)*NumeroDeVertices(G));
    if (array_natural == NULL || array_wp == NULL) {
        return 1;
    }
    u32 delta = Delta(G);
    u32 indice_wp = 0;
    
    for (u32 i = delta; i != 4294967295; i--) {
        for (u32 j = 0; j < NumeroDeVertices(G); j++) {
            if (Grado(j, G) == i) {
                array_wp[indice_wp] = Nombre(j, G);
                indice_wp++;
            }
        }
    }
    for (u32 i = 0; i < NumeroDeVertices(G); i++) {
        for (u32 j = 0; j < NumeroDeVertices(G); j++) {
            if (array_wp[i] == array_natural[j]) {
                FijarOrden(i, G, j);
                break;
            }
        }
    }
    free(array_natural);
    free(array_wp);
    return 0;

}

char RevierteBC(Grafo G) {
    //array_natural tiene los nombres de nodos ordenados
    u32 *array_natural = Natural(G);
    u32 *array_gc = malloc(sizeof(u32)*NumeroDeVertices(G));
    if (array_natural == NULL || array_gc == NULL) {
        return 1;
    }
    u32 max_color = MaxColor(G);
    u32 indice_gc = 0;
    for (u32 i = max_color; i != 4294967295; i--) {
        for (u32 j = 0; j < NumeroDeVertices(G); j++) {
            if (Color(j, G) == i) {
                array_gc[indice_gc] = Nombre(j, G);
                indice_gc++;
            }
        }
    }
    for (u32 i = 0; i < NumeroDeVertices(G); i++) {
        for (u32 j = 0; j < NumeroDeVertices(G); j++) {
            if (array_gc[i] == array_natural[j]) {
                FijarOrden(i, G, j);
                indice_gc++;
                break;
            }
        }
    }
    free(array_natural);
    free(array_gc);
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

void Bfs (u32 x, u32 *vertices_cc, Grafo G) {
    if (vertices_cc[x] == 0) {
        vertices_cc[x] = 1;
    }
    for (u32 j = 0; j < Grado(x,G); j++) {
        int indice = OrdenVecino(j,x,G);
        //printf("vertices_Cc[%d] = %d\n", indice, vertices_cc[indice]);
        if (vertices_cc[indice] == 0) {
            vertices_cc[indice] = 2;
            //printf("HOLAvertices_Cc[%d] = %d\n", indice, vertices_cc[indice]);
        }
    }
    for (u32 j = 0; j < Grado(x,G); j++) {
        u32 indice = OrdenVecino(j,x,G);
        for (u32 i = 0; i < Grado(indice,G); i++) {
            if (vertices_cc[OrdenVecino(i,indice,G)] == 0) {
                Bfs(OrdenVecino(i,indice,G), vertices_cc, G);
            }
        }
    }
}
/*Devuelve 1 si W es bipartito, 0 si no.
Ademas, si devuelve 1, colorea W con un coloreo propio de dos colores. Si devuelve 0, debe dejar a G coloreado con algún
coloreo propio. (no necesariamente el mismo que tenı́a al principio).*/

char Bipartito(Grafo G) {
    u32 *vertices_cc = calloc(NumeroDeVertices(G),sizeof(u32));
    if (vertices_cc == NULL){
        return '-1';
    }
    for(u32 i = 0; i < NumeroDeVertices(G); i++) {
        if (vertices_cc[i] == 0) {
            Bfs(i, vertices_cc, G);
        }
    }
    for(u32 i = 0; i < NumeroDeVertices(G); i++) {
        FijarColor(vertices_cc[i]%2,i,G);
    }
    int color = 2;
    for(u32 i = 0; i < NumeroDeVertices(G); i++) {
        for(u32 j = 0; j < Grado(i,G); j++) {
            if (Color(i,G) == ColorVecino(j, i, G)) {
                FijarColor(color,OrdenVecino(j, i, G),G);
                color++;
            }
        }
    }
    free(vertices_cc);
    if (MaxColor(G) == 1) {
        printf("SOY BIPARTITO\n");
        return '1';
    } else {
        return '0';
    }


}

u32 NumCCs(Grafo G) {
    u32 *vertices_cc = calloc(NumeroDeVertices(G),sizeof(u32));
    if (vertices_cc == NULL){
        return 0;
    }
    u32 NumCC = 0;
    for(u32 i = 0; i < NumeroDeVertices(G); i++) {
        if (vertices_cc[i] == 0) {
            Bfs(i, vertices_cc, G);
            NumCC++;
        }
    }
    printf("AMOUNT CC: %u\n", NumCC);
    for(u32 i = 0; i < NumeroDeVertices(G); i++) {
        printf("VERTICES_CC[%d] = %u\n", i, vertices_cc[i]);
    }
    free(vertices_cc);
    vertices_cc = NULL;
    return NumCC;
}


char AleatorizarVertices(Grafo G, u32 R){
    srand(R);
    u32 *array_random = calloc(NumeroDeVertices(G),sizeof(u32));
    if (array_random == NULL){
        return 1;
    }
    u32 i = 0;
    while (i != NumeroDeVertices(G)) {
        int indice = ((int)rand()) % NumeroDeVertices(G);
        int j;
        for (j=0; j<i; j++){
            if (indice == array_random[j]) {
                j=-1;
                break;
            }
        }
        if (j!=-1) {
            array_random[i]= indice;
            FijarOrden(i,G,indice);
            i++;
        }
    }
    free(array_random);
    array_random=NULL;
    return 0;
}
