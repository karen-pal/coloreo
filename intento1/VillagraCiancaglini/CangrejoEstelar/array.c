#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "log.h"

u32 findGap(Array *neighbors) {
    if (neighbors->length == 0) return 0;
    if (ArrayGet(neighbors, 0) > 0) return 0;
    u32 col = 0;
    for (u32 i = 0; i < neighbors->length; i++) {
        u32 v_col = ArrayGet(neighbors, i);
        if (col == v_col)
            col++;
	else if (col > v_col)
	  continue;
        else
            return col;
    }
    return col;
}

bool arrayEqual(Array *old, Array *new) {
    for (u32 i = 0; i < old->length; ++i) {
        u32 expect = ArrayGet(old, i);
        u32 got = ArrayGet(new, i);
        if (expect != got) {
            log_error("%s: expected '%u', got '%u' at pos %u", __func__, expect,
                      got, i);
            return false;
        }
    }

    return true;
}

int arraycmp(const void *a, const void *b) {
    u32 arg1 = *(const u32 *)a;
    u32 arg2 = *(const u32 *)b;

    if (arg1 < arg2) {
        return -1;
    }
    if (arg1 > arg2) {
        return 1;
    }

    return 0;
}

void arraySort(Array *a) {
    qsort(a->contents, a->length, sizeof(u32), arraycmp);
}

void arrayExpand(Array *a) {
    u32 growth = a->capacity < megabyte ? a->capacity * 2
                                        : a->capacity + megabyte;
    u32 newSize = a->capacity == 0 ? 1 : growth;
    a->capacity = newSize;
    a->contents = realloc(a->contents, newSize * sizeof(u32));
}

void arrayPrint(Array *a) {
    fprintf(stderr, "len=%u, cap=%u, contents=[ ", a->length, a->capacity);
    for (u32 i = 0; i < a->length; ++i) {
        fprintf(stderr, "%u ", a->contents[i]);
    }
    fprintf(stderr, "]\n");
}

// return a string with metadata of the array, up to n numbers inside it
// remember to free after using!
char *arrayString(Array *a, u32 n) {
    char *s = calloc((unsigned long)(25 + n * 11), sizeof(char));
    sprintf(s, "len=%u, cap=%u, contents=[ ", a->length, a->capacity);
    for (u32 i = 0; i < n && i < a->length; ++i) {
        char num[11];
        snprintf(num, 11, "%u ", a->contents[i]);
        strncat(s, num, 11);
    }
    strncat(s, "]", 1);

    return s;
}

Array *ArrayNew() {
    Array *rv = calloc(1, sizeof(Array));
    rv->contents = calloc(1, sizeof(u32));
    rv->capacity = 1;
    return rv;
}

Array *ArrayNewSized(u32 capacity) {
    Array *rv = calloc(1, sizeof(Array));
    rv->capacity = capacity;
    rv->contents = calloc(capacity, sizeof(u32));
    return rv;
}

void ArrayDestroy(Array *a) {
    if (a->contents != NULL) {
        free(a->contents);
        a->contents = NULL;
    }
    free(a);
}

Array *ArrayCopy(Array *a) {
    Array *rv = calloc(1, sizeof(Array));

    rv->contents = malloc(sizeof(u32) * a->capacity);
    rv->capacity = a->capacity;
    rv->length = a->length;
    rv->contents = memcpy(rv->contents, a->contents, sizeof(u32) * a->length);

    return rv;
}

// ArrayGet returns the val in the given position. Segfaults if
// the position is out of range.
u32 ArrayGet(Array *a, u32 pos) {
#ifndef NDEBUG
    if (a->contents == NULL || pos >= a->length) {
        char *s = arrayString(a, 10);
        log_error("%s: segfault: %s", __func__, s);
        free(s);
        raise(SIGSEGV);
        return U32_MAX;
    }
#endif

    return a->contents[pos];
}

void ArraySet(Array *a, u32 pos, u32 value) {
    if (a->contents == NULL || pos >= a->length || pos >= a->capacity) {
        char *s = arrayString(a, 10);
        log_error("%s: segfault: %s", __func__, s);
        free(s);
        raise(SIGSEGV);
        return;
    }

    a->contents[pos] = value;
}

void ArrayClean(Array *a) { a->length = 0; }

u32 ArrayAdd(Array *a, u32 value) {
    if (a->length >= a->capacity) {
        arrayExpand(a);
    }
    u32 pos = a->length;
    a->length++;
    a->contents[pos] = value;
    return pos;
}

EdgeArray *EdgeArrayNew() {
    EdgeArray *e = calloc(1, sizeof(EdgeArray));
    return e;
}

void EdgeArrayDestroy(EdgeArray *e) {
    for (u32 i = 0; i < e->length; i++) {
        ArrayDestroy(e->contents[i]);
    }
    free(e->contents);
    free(e);
}

EdgeArray *EdgeArrayCopy(EdgeArray *e) {
    EdgeArray *e2 = EdgeArrayNew();
    e2->length = e->length;
    e2->contents = calloc(e->length, sizeof(Array *));

    for (u32 i = 0; i < e->length; i++) {
        e2->contents[i] = ArrayCopy(e->contents[i]);
    }

    return e2;
}

u32 EdgeArrayAdd(EdgeArray *e) {
    u32 pos = e->length;
    e->length++;
    e->contents = realloc(e->contents, e->length * sizeof(Array *));
    e->contents[e->length - 1] = ArrayNew();
    return pos;
}

void EdgeArrayConnect(EdgeArray *e, u32 node1, u32 pos1, u32 node2, u32 pos2) {
#ifndef NDEBUG
    if (pos1 >= e->length || pos2 >= e->length) {
        log_error("%s: too large: pos1=%u, pos2=%u, e->len=%u", __func__, pos1,
                  pos2, e->length);
        return;
    }

#endif
    ArrayAdd(e->contents[pos1], node2);
    ArrayAdd(e->contents[pos2], node1);
}

Array *EdgeArrayGetConnections(EdgeArray *e, u32 node) {
#ifndef NDEBUG
    if (node >= e->length) {
        return NULL;
    }
#endif
    return e->contents[node];
}
