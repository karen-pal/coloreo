struct GrafoSt {
    u32 cant_ver;
    u32 cant_lad;
    u32 delta;
    NodoSt nodos_array[cant_ver]; // y si aca esta el orden?
    u32 orden[cant_ver]; // esto no hace falta
};

struct NodoSt {
    u32 vecinos[grado]; //solo guardamos los nombres
    u32 nombre;
    u32 grado;
    u32 color;
};
