#ifndef TESTS
#define TESTS

// Utilidades para los tests

#define STRINGIFY(x) #x

#define Assert(testExpression) \
    if (!(testExpression)) {\
        fprintf(stderr, "❌ `%s` failed in %s\n", STRINGIFY(testExpression), __func__);\
        return false;\
    }\

#define EndTest \
    fprintf(stdout, "✔️ %s\n", __func__);\
    return true;\


#include <time.h>

/// Devuelve el tiempo en segundos.
/// Funciona solo en unix.
double get_time() {
    return (double)clock()/CLOCKS_PER_SEC;
}

#endif
