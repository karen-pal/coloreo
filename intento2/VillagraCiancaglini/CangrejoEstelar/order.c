#include "order.h"
#include "veinteveinte.h"

#include <stdlib.h>

int _blocks_compare(const void *_a, const void *_b) {
    Array *a = (Array *)_a;
    Array *b = (Array *)_b;
    if (a->length == b->length)
        return 0;
    else if (a->length < b->length)
        return -1;
    else
        return 1;
}

typedef struct {
    Array **contents;
    u32 size;
} ColorBlocks;

ColorBlocks *ColorBlocksNew(u32 size) {
    ColorBlocks *cb = calloc(1, sizeof(ColorBlocks));
    cb->size = size;
    cb->contents = calloc(size, sizeof(Array *));
    for (u32 i = 0; i < size; i++) cb->contents[i] = ArrayNew();
    return cb;
}

void ColorBlocksDestroy(ColorBlocks *cb) {
    for (u32 i = 0; i < cb->size; i++) ArrayDestroy(cb->contents[i]);
    free(cb->contents);
    free(cb);
}

void ColorBlocksSortSize(ColorBlocks *cb) {
    qsort(cb->contents, cb->size, sizeof(Array *), _blocks_compare);
}

void ColorBlocksReverse(ColorBlocks *cb) {
    Array *worker;
    if (cb->size % 2 == 0) {
        for (u32 i = 0; i <= cb->size / 2; i++) {
            worker = cb->contents[i];
            cb->contents[i] = cb->contents[cb->size - 1 - i];
            cb->contents[cb->size - 1 - i] = worker;
        }
    } else {
        for (u32 i = 0; i < (cb->size + 1) / 2; i++) {
            worker = cb->contents[i];
            cb->contents[i] = cb->contents[cb->size - 1 - i];
            cb->contents[cb->size - 1 - i] = worker;
        }
    }
}

Array *ColorBlocksIndex(ColorBlocks *cb, u32 i) { return cb->contents[i]; }

char ColorBlocksWrite(ColorBlocks *cb, Grafo G) {
    Array *natural = NaturalOrder(G);
    if (natural == NULL) return 1;

    Grafo Gp = CopiarGrafo(G);
    if (Gp == NULL) {
        ArrayDestroy(natural);
        return 1;
    }

    u32 pos = 0;
    for (u32 i = 0; i < cb->size; i++) {
        Array *block = ColorBlocksIndex(cb, i);
	for (u32 j = 0; j < block->length; j++) {
	    u32 pos_current = ArrayGet(block, j);
            u32 pos_natural = TranslateToNatural(Gp, natural, pos_current);
	    if (pos_natural == U32_MAX) {
  	        ArrayDestroy(natural);
		DestruccionDelGrafo(Gp);
	        return 1;
	    }
	    ArraySet(block, j, pos_natural);
	}
	arraySort(block);
        for (u32 j = 0; j < block->length; j++) {
            u32 pos_nat = ArrayGet(block, j);
            char res = FijarOrden(pos, G, pos_nat);
            if (res != 0) {
                ArrayDestroy(natural);
                DestruccionDelGrafo(Gp);
                return 1;
            }
            pos++;
        }
    }
    ArrayDestroy(natural);
    DestruccionDelGrafo(Gp);
    return 0;
}

u32 MaxColor(Grafo G) {
    u32 mc = 0;
    for (u32 i = 0; i < NumeroDeVertices(G); i++) {
        u32 col = Color(i, G);
        mc = col > mc ? col : mc;
    }
    return mc + 1;
}

char RevierteBC(Grafo G) {
    ColorBlocks *cb = ColorBlocksNew(MaxColor(G));
    if (cb == NULL) return 1;

    for (u32 i = 0; i < NumeroDeVertices(G); i++) {
        u32 color = Color(i, G);
        ArrayAdd(ColorBlocksIndex(cb, color), i);
    }
    ColorBlocksReverse(cb);
    char res = ColorBlocksWrite(cb, G);

    ColorBlocksDestroy(cb);
    return res;
}

char ChicoGrandeBC(Grafo G) {
    ColorBlocks *cb = ColorBlocksNew(MaxColor(G));
    if (cb == NULL) return 1;

    for (u32 i = 0; i < NumeroDeVertices(G); i++) {
        u32 color = Color(i, G);
        ArrayAdd(ColorBlocksIndex(cb, color), i);
    }
    ColorBlocksSortSize(cb);
    char res = ColorBlocksWrite(cb, G);

    ColorBlocksDestroy(cb);
    return res;
}

char WelshPowell(Grafo G) {
    ColorBlocks *cb = ColorBlocksNew(Delta(G) + 1);
    if (cb == NULL) return 1;

    for (u32 i = 0; i < NumeroDeVertices(G); i++) {
        u32 deg = Grado(i, G);
        ArrayAdd(ColorBlocksIndex(cb, deg), i);
    }
    ColorBlocksReverse(cb);
    char res = ColorBlocksWrite(cb, G);

    ColorBlocksDestroy(cb);
    return res;
}

int _natural_compare(const void *_a, const void *_b) {
    u32 a = *(u32 *)_a;
    u32 b = *(u32 *)_b;
    if (a == b)
        return 0;
    else if (a < b)
        return -1;
    else
        return 1;
}

Array *NaturalOrder(Grafo G) {
    Array *list = ArrayNewSized(NumeroDeVertices(G));
    for (u32 i = 0; i < NumeroDeVertices(G); i++) {
        u32 name = Nombre(i, G);
        ArrayAdd(list, name);
    }
    qsort(list->contents, list->length, sizeof(u32), _natural_compare);
    return list;
}

int _natural_search_compare(const void *_a, const void *_b) {
    u32 key = *(u32 *)_a;
    u32 val = *(u32 *)_b;
    if (key == val)
        return 0;
    else if (key < val)
        return -1;
    else
        return 1;
}

// Guaranteed to finish
u32 TranslateToNatural(Grafo G, Array *natural, u32 pos) {
    u32 name = Nombre(pos, G);
    u32 *val = bsearch(&name, natural->contents, natural->length, sizeof(u32),
                       _natural_search_compare);
    if (val == NULL) return U32_MAX;
    return (u32)(val - natural->contents);
}

char AleatorizarVertices(Grafo G, u32 R) {
    u32 N = NumeroDeVertices(G);
    Array *n_order = ArrayNewSized(N);
    if (n_order == NULL) return 1;
    for (u32 i = 0; i < N; i++) ArrayAdd(n_order, i);
    srand(R);
    for (u32 i = 0; i < N * 10; i++) {
        u32 p1 = rand() % N;
        u32 p2 = rand() % N;
        u32 v1 = ArrayGet(n_order, p1);
        u32 v2 = ArrayGet(n_order, p2);
        ArraySet(n_order, p1, v2);
        ArraySet(n_order, p2, v1);
    }
    for (u32 i = 0; i < N; i++) {
        char res = FijarOrden(i, G, ArrayGet(n_order, i));
        if (res != 0) {
            ArrayDestroy(n_order);
            return 1;
        }
    }
    ArrayDestroy(n_order);
    return 0;
}
