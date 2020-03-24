#include "veinteveinte.h"

//getnodebyname se podría hacer ordenando el nodos_array y haciendo busqueda binaria
NodoSt GetNodeByName(Grafo G,u32 name){
   for (u32 i=0; i<G->cant_vert;i++) {
        if (G->nodos_array[i].nombre == name) {
            return G->nodos_array[i]
        }
    }
    return null
}


u32 Greedy(Grafo G){

    GetNodeByName(G,Nombre(G,0)).color = 0;
    u32 actual_color;
    for (u32 i = 1; i < G->cant_ver; i++) {
        actual_color = 0;
        for (u32 j = i - 1; j = 0; j--) {
            if (G->nodos_array[j].color == actual_color) {
                for (u32 k = 0; k < G->nodos_array[i].grado; k++) {
                    if (G->nodos_array[j].nombre == G->nodos_array[i].vecinos[k]) {
                        actual_color++;
                        break;
                    }
                }
            }
        }
        G->nodos_array[i].color = actual_color;
    }
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

//(si i == j)???
char SwitchColores(Grafo G,u32 i,u32 j) {
    u32 max_color = 0 ;
    for (u32 index = 0; G->cant_ver; index++) {
        if (G->nodos_array[index].color > max_color) {
            max_color = G->nodos_array[index].color;
        }
    }
    if (i <= max_color && j >= max_color) {
        for (u32 index = 0; index < G-> cant_ver; index++) {
            if (G->nodos_array[index].color == i) {
                G->nodos_array[index].color = j;
            } else if (G->nodos_array[index].color == j) {
                G->nodos_array[index].color = i;
            }
        }
        return 0;
    }
    return 1;
}
