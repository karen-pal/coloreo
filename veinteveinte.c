#include "veinteveinte.h"

u32 Greedy(Grafo G){
    //inicializacion
    nodos_array[0].color = 0;
    color_0.append(nodos[0]);
    for i in (1, length(nodos_array[0].vecinos)) {
        nodos_array[0].vecino[i].color = 1;
        color_i.append(nodos_array[0].vecino[i])
    }
    /*
    arreglo dinamico con True..el grado del grafo sería
    el largo del arreglo??
    */
    for i in cant_vert,i=1, i++ {
        siguiente_nodo = Nodo(orden[i])
        for j in (0,i-1), j++ {
        if siguiente_nodo in Nodo(orden[i-1]).vecinos {
            if siguiente_nodo.color = Nodo(orden[i-1]).color
            siguiente_nodo.color = Nodo(orden[i-1]).color + 1;
            break;
        }

    }
    }
}
    

u32 Greedy(Grafo G){

    G->nodos_array[0].color = 0;
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
}









    for i in length(nodos_array of G) {
        for (j in 0 i-1, j++) {
            if nodos_array[i].nombre not in nodos_array[j].vecinos and nodos_array[i].color==nodos_array[j].color {
                break;
            }
        }
        nodos_array[i].color = j
        
o bien no es vecino del mas chico, entonces tiene el color mas chico
o es vecino

    }
    //en el orden pasar por cada nodo
    //si es el primero ponerle color 0
    //al siguiente, fijarse si ponerle el color min interfiere
    //con el coloreo que ya está en el grafo
    //darle el color min tal que no joda

    return color;
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
