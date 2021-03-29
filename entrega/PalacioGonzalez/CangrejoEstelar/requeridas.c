#include "calculoscongrafos.h"
#include "requeridas.h"

char ChicoGrandeBC(Grafo G) {
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
    u32 *array_natural = Natural(G);
    u32 *array_rev = malloc(sizeof(u32)*NumeroDeVertices(G));
    if (array_natural == NULL || array_rev == NULL) {
        return 1;
    }
    u32 max_color = MaxColor(G);
    u32 indice_rev = 0;
    for (u32 i = max_color; i != 4294967295; i--) {
        for (u32 j = 0; j < NumeroDeVertices(G); j++) {
            if (Color(j, G) == i) {
                array_rev[indice_rev] = Nombre(j, G);
                indice_rev++;
            }
        }
    }
    for (u32 i = 0; i < NumeroDeVertices(G); i++) {
        for (u32 j = 0; j < NumeroDeVertices(G); j++) {
            if (array_rev[i] == array_natural[j]) {
                FijarOrden(i, G, j);
                break;
            }
        }
    }
    free(array_natural);
    free(array_rev);
    return 0;
}

u32 Greedy(Grafo G) {
    FijarColor(0,0,G);
    u32 max_color = 0;
    for (u32 i = 1; i < NumeroDeVertices(G); i++) {
        u32 color = 0;
        for (u32 j = 0; j < Grado(i,G); j++) {
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

char Bipartito(Grafo G) {
    u32 *vertices_cc = calloc(NumeroDeVertices(G),sizeof(u32));
    if (vertices_cc == NULL){
        return '2';
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
