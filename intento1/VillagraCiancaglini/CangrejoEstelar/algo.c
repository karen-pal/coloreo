#include <stdbool.h>
#include <stdlib.h>

#include "array.h"
#include "base.h"
#include "log.h"
#include "queue.h"
#include "veinteveinte.h"

static inline void getNeighbors(u32 i, Array *neighbors, Grafo G) {
    // more than one neighbour
    // get the colors for all neighbors
    ArrayClean(neighbors);
    for (u32 j = 0; j < Grado(i, G); ++j) {
        u32 neighColor = ColorVecino(j, i, G);
        if (neighColor == U32_MAX) {
            break;
        }

        ArrayAdd(neighbors, neighColor);
    }

    arraySort(neighbors);
}

u32 Greedy(Grafo G) {
    if (G == NULL) return U32_MAX;

    const u32 n = NumeroDeVertices(G);
    u32 maxcolor = 0;

    for (u32 i = 0; i < n; ++i) {
        char ok = FijarColor(UNCOLORED, i, G);
        if (ok == 1) {
            return U32_MAX;
        }
    }

    Array *neighbors = ArrayNewSized(Delta(G));
    if (neighbors == NULL) return U32_MAX;

    for (u32 i = 0; i < n; ++i) {
        getNeighbors(i, neighbors, G);
        u32 color = findGap(neighbors);
        char ok = FijarColor(color, i, G);
        if (ok == 1) {
            ArrayDestroy(neighbors);
            return U32_MAX;
        }

        maxcolor = color > maxcolor ? color : maxcolor;
    }

    ArrayDestroy(neighbors);
    return maxcolor + 1;
}

bool some_false(bool *list, size_t size) {
    for (size_t i = 0; i < size; i++)
        if (!list[i]) return true;
    return false;
}

// Only call this if the list has some false value.
u32 first_new(bool *list, size_t size) {
    for (u32 i = 0; i < size; i++)
        if (!list[i]) return i;
    return 0;
}

u32 NumCCs(Grafo G) {
    if (NumeroDeVertices(G) == 0) return 0;
    u32 cc = 0;

    bool *visited = calloc(NumeroDeVertices(G), sizeof(bool));
    Queue *q = QueueNew(Delta(G));

    while (some_false(visited, NumeroDeVertices(G))) {
        QueueAdd(q, first_new(visited, NumeroDeVertices(G)));
        while (q->size > 0) {
            u32 node = QueuePull(q);
            visited[node] = true;
            u32 n_count = Grado(node, G);
            for (u32 j = 0; j < n_count; j++) {
                u32 neighbour = OrdenVecino(j, node, G);
                if (!visited[neighbour]) {
                    QueueAdd(q, neighbour);
                    visited[neighbour] = true;
                }
            }
        }
        cc++;
    }

    QueueDestroy(q);
    free(visited);
    return cc;
}

char SwitchColores(Grafo G, u32 i, u32 j) {
    u32 n_color = 0;
    for (u32 x = 0; x < NumeroDeVertices(G); x++)
        n_color = Color(x, G) > n_color ? Color(x, G) : n_color;
    n_color++;
    if (i > n_color || j > n_color) return 1;

    for (u32 x = 0; x < NumeroDeVertices(G); x++) {
        if (i == Color(x, G))
            FijarColor(j, x, G);
        else if (j == Color(x, G))
            FijarColor(i, x, G);
    }

    return 0;
}

char Bipartito(Grafo G) {
    if (G == NULL || NumeroDeVertices(G) == 0) {
        return 0;
    }

    bool bipartito = true;
    bool *visited = calloc(NumeroDeVertices(G), sizeof(bool));
    Queue *curgroup = QueueNew(NumeroDeVertices(G));
    Queue *othergroup = QueueNew(NumeroDeVertices(G));

    for (u32 i = 0; i < NumeroDeVertices(G); i++) {
        FijarColor(UNCOLORED, i, G);
    }

    while (some_false(visited, NumeroDeVertices(G))) {
        if (!bipartito) {
            break;
        }

        u32 f = first_new(visited, NumeroDeVertices(G));
        QueueAdd(curgroup, f);
        FijarColor(0, f, G);

        while (curgroup->size > 0 && bipartito) {
            u32 node = QueuePull(curgroup);
            visited[node] = true;

            u32 n_count = Grado(node, G);
            u32 nodecolor = Color(node, G);
            u32 neighcolor;
            if (nodecolor == 0) {
                neighcolor = 1;
            } else {
                neighcolor = 0;
            }

            for (u32 j = 0; j < n_count; j++) {
                u32 neighbour = OrdenVecino(j, node, G);
                if (Color(neighbour, G) == nodecolor) {
                    bipartito = false;
                    break;
                }

                FijarColor(neighcolor, neighbour, G);

                if (!visited[neighbour]) {
                    QueueAdd(othergroup, neighbour);
                    visited[neighbour] = true;
                }
            }

            Queue *tmp = curgroup;
            curgroup = othergroup;
            othergroup = tmp;
        }
    }

    QueueDestroy(curgroup);
    QueueDestroy(othergroup);
    free(visited);

    if (!bipartito) {
        Greedy(G);
        return 0;
    }

    return 1;
}
