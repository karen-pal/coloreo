#ifndef GRAFO_ST
#define GRAFO_ST

typedef unsigned int u32;

typedef struct color_t { u32 value; } color_t;

typedef struct orden_natural_t { u32 value; } orden_natural_t;

typedef struct orden_interno_t { u32 value; } orden_interno_t;

typedef struct numero_de_vecino_t { u32 value; } numero_de_vecino_t;

typedef struct peso_t { u32 value; } peso_t;

typedef struct nombre_t { u32 value; } nombre_t;

typedef
    struct Ordenamiento {
        u32 numero_de_vertices;
        orden_natural_t *orden_natural;
        orden_interno_t *orden_interno;
    }
    Ordenamiento;


struct vecino_t {
    orden_natural_t orden_natural;
    peso_t peso;
};

typedef struct vecino_t vecino_t;


struct Vertice {
    nombre_t nombre;
    color_t color;
    vecino_t *vecinos;
    u32 cantidad_de_vecinos;
};

typedef struct Vertice Vertice;

struct Vertices {
    Vertice *vertices;
    u32 numero_de_vertices;
};

typedef struct Vertices Vertices;

typedef
    struct GrafoSt {
        u32 numero_de_lados;
        u32 delta;

        Ordenamiento ordenamiento;
        Vertices vertices;
    }
    GrafoSt;

typedef unsigned int u32;

#endif
