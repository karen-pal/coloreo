#ifndef ORDENAMIENTO_H
#define ORDENAMIENTO_H

#include "../Tipos/Tipos.h"
#include <string.h>

typedef
    struct Ordenamiento {
        u32 numero_de_vertices;
        orden_natural_t *orden_natural;
        orden_interno_t *orden_interno;
    }
    Ordenamiento;

#endif
