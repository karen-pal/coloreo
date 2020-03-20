struct GrafoSt {
    u32 cant_ver;
    u32 cant_lad;
    u32 delta;
    NodoSt nodos_array[cant_ver];
    u32 orden[cant_ver];
};

struct NodoSt {
    u32 vecinos[grado]; //solo guardamos los nombres
    u32 nombre;
    u32 grado;
    u32 color;
};
