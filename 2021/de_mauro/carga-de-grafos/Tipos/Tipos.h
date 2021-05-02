#ifndef TIPOS_H
#define TIPOS_H

#include "../common/common.h"

/* Me ayudan a descubrir errores tontos. */

typedef struct color_t { u32 value; } color_t;

typedef struct orden_natural_t { u32 value; } orden_natural_t;

typedef struct orden_interno_t { u32 value; } orden_interno_t;

typedef struct numero_de_vecino_t { u32 value; } numero_de_vecino_t;

typedef struct peso_t { u32 value; } peso_t;

typedef struct nombre_t { u32 value; } nombre_t;

enum result_t {
    SUCCESS,
    FAILURE
};

typedef enum result_t result_t;

#endif
