#include "veinteveinte.h"

/*//getnodebyname se podría hacer ordenando el nodos_array y haciendo busqueda binaria
NodoSt GetNodeByName(Grafo G,u32 name){
   for (u32 i=0; i<G->cant_vert;i++) {
        if (G->nodos_array[i].nombre == name) {
            return G->nodos_array[i]
        }
    }
    return null
}


u32 Greedy(Grafo G){
    GetNodeByName(G,Nombre(G,0)).color = 0; //Nombre() se ocupa de que se mantenga el orden
    u32 actual_color;
    //iterar por todos los verticesq
    for (u32 i = 1; i < G->cant_ver; i++) {
        actual_color = 0;
        //iterar por los vértices ya coloreados
        for (u32 j = i - 1; j = 0; j--) {
            if (GetNodeByName(G,Nombre(G,j)).color == actual_color) {
                for (u32 k = 0; k < G->nodos_array[i].grado; k++) {
                    if (GetNodeByName(G,Nombre(G,j)).nombre == G->nodos_array[i].vecinos[k]) {
                        actual_color++;
                        break;
                    }
                }
            }
        }
        G->nodos_array[i].color = actual_color;
    }
*/

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
    return G->nodos_array[i].nombre;
}

u32 Color(u32 i, Grafo G) {
    return G->nodos_array[i].color;
}

u32 Grado(u32 i, Grafo G) {
    return G->nodos_array[i].grado;
}

char FijarColor(u32 x,u32 i,Grafo G) {
    if (i < NumeroDeVertices(G)) {
        G->nodos_array[i].color = x;
        return 0;
    } else {
        return 1;
    }
}

//revisar
u32 ColorVecino(u32 j,u32 i,Grafo G) { 
    if (i >= NumeroDeVertices(G) || j >= G->nodos_array[i].grado) {
        return 4294967295;
    } else {
        int index = 0;
        while (G->nodos_array[i].vecinos[j] != G->nodos_array[index].nombre) {
            index++;
        }
         return Color(index, G);
    }
}

u32 NombreVecino(u32 j,u32 i,Grafo G) {
    return G->nodos_array[i].vecinos[j];
}

char ChicoGrandeBC(Grafo G) {
    for (int i = 0; i < NumeroDeVertices(G) -1; i++) {
        for (int j = 1; j < NumeroDeVertices(G); j++) {
            if (Color(i,G) > Color(j,G)) {

            }
        }
    }
}


u32 OrdenVecino(u32 j,u32 i,Grafo G) {
    u32 vecino = NombreVecino(j, i , G);
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
                    j = 0;
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

u32 NumCC(Grafo G) {
    //calcular bnfs para un nodo. guardar los nodos de su comp conexa
    //calcular bnfs para el siguiente nodo que no está en alguna comp
    //conexa ya calculada.
    //seguir hasta que no haya más nodos a recorrer
    //la cant de estos conjuntos de comp conexas va a ser la cant de comp conexas de G
    return 0;
};

char WelshPowell(Grafo G) {
    
}
*/
char SwitchColores(Grafo G,u32 i,u32 j) {
    u32 max_color = 0 ;
    for (u32 indice = 0; indice < NumeroDeVertices(G); indice++) {
        if (Color(indice, G) > max_color) {
            max_color = Color(indice, G);
        }
    }
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

