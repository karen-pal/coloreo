#ifndef SAFE_STDLIB
#define SAFE_STDLIB

#include <stdlib.h>
#include <stdio.h>

/** Como el calloc normal pero hace `exit` si
 * no hay memoria.
*/
void *safe_calloc(size_t nmemb, size_t size) {
    void *result = calloc(nmemb, size);

    if (result == NULL) {
        fprintf(stderr, "error: safe_calloc NULL\n");
        exit(-1);
    }

    return result;
}

/** Como el realloc normal pero hace `exit` si
 * no hay memoria.
*/
void *safe_realloc(void *ptr, size_t size) {
    void *result = realloc(ptr, size);

    if (result == NULL) {
        fprintf(stderr, "error: safe_realloc NULL\n");
        exit(-1);
    }

    return result;
}

// "safe" es una forma de decir, no sé si se le pueda llamar "seguro" a hacer un `exit`
// cuando hay un error.

#endif
