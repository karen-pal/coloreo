#ifndef TIPOS
#define TIPOS

#include "Tipos.h"
#include "../common/common.c"

color_t color_of(u32 value) {
    color_t t = { value };
    return t;
}

orden_natural_t orden_natural_of(u32 value) {
    orden_natural_t t = { value };
    return t;
}

struct maybe_orden_natural {
    bool has_value;
    orden_natural_t value;
};

typedef struct maybe_orden_natural maybe_orden_natural;

maybe_orden_natural maybe_orden_natural_of(orden_natural_t orden_natural) {
    maybe_orden_natural maybe = { true, orden_natural };
    return maybe;
}

maybe_orden_natural maybe_orden_natural_none() {
    maybe_orden_natural maybe = { false };
    return maybe;
}

orden_interno_t orden_interno_of(u32 value) {
    orden_interno_t t = { value };
    return t;
}

struct maybe_orden_interno {
    bool has_value;
    orden_interno_t value;
};

typedef struct maybe_orden_interno maybe_orden_interno;

maybe_orden_interno maybe_orden_interno_of(orden_interno_t orden_interno) {
    maybe_orden_interno maybe = { true, orden_interno };
    return maybe;
}

maybe_orden_interno maybe_orden_interno_none() {
    maybe_orden_interno maybe = { false };
    return maybe;
}

numero_de_vecino_t numero_de_vecino_of(u32 value) {
    numero_de_vecino_t t = { value };
    return t;
}

peso_t peso_of(u32 value) {
    peso_t t = { value };
    return t;
}

nombre_t nombre_of(u32 value) {
    nombre_t t = { value };
    return t;
}

struct maybe_nombre {
    bool has_value;
    nombre_t value;
};

typedef struct maybe_nombre maybe_nombre;

maybe_nombre maybe_nombre_of(nombre_t nombre) {
    maybe_nombre maybe;
    maybe.has_value = true;
    maybe.value = nombre;
    return maybe;
}

maybe_nombre maybe_nombre_none() {
    maybe_nombre maybe;
    maybe.has_value = false;
    return maybe;
}

// --- result

/** Convierte un result a char para responder ejercicios 8.1, 8.2 y 8.3.
*/
char result_to_char(result_t result) {
    if (result == SUCCESS) {
        return 0;
    } else {
        return 1;
    }
}

#endif
