#define STB_DS_IMPLEMENTATION

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "GrafoSt2020.h"
#include "log.h"
#include "stb_ds.h"
#include "veinteveinte.h"

enum LineStart {
    LineComment = 'c',
    LineGraphConfig = 'p',
    LineGraphEdge = 'e',
    LineNewLine = '\n',
    LineCarriageReturn = '\r'
};

typedef struct edge {
    u32 node1;
    u32 node2;
} edge;

int _edge_compare(const void *a, const void *b) {
    edge *e1 = (edge *)a;
    edge *e2 = (edge *)b;

    if (e1->node1 == e2->node1) {
        return 0;
    }
    if (e1->node1 < e2->node1) {
        return -1;
    }

    return 1;
}

bool GraphParse(GrafoSt *g, FILE *stream) {
    char line;
    u32 nodes = 0;
    u32 edges = U32_MAX;
    u32 node1, node2 = 0;
    int readchars = 0;
    u32 curredge = 0;
    edge *edgelist = NULL;
    struct {
        u32 key;
        u32 value;
    } *tmpnames = NULL;
    hmdefault(tmpnames, U32_MAX);

    // Error handling
    int matched_params = 0;

    while ((readchars = fscanf(stream, "%c", &line)) != 0) {
        if (readchars == EOF || curredge >= edges) {
            break;
        }

        switch (line) {
        case LineComment:
            log_debug("%s: found a comment, skipping...", __func__);

            // A bit to unwind here, but simplifies things a lot,
            // as it helps us completely skip comments:
            //   '*' means ignore this value, don't try to assign it
            //   '[]' matches a regular expression within
            //   '^\n' means everything from the beginning ('^') til
            //   the newline ('\n')
            fscanf(stream, "%*[^\n]\n");

            break;

        case LineGraphConfig:
            // Ignores the 'edge' word and only assigns the other values
            matched_params = fscanf(stream, "%*4s%u%u", &nodes, &edges);
            if (matched_params != 2) {
                log_error("%s: couldn't parse config: nodes='%u', edges='%u'");
                return false;
            }

            log_debug("%s: found graph config: nodes=%u, edges=%u", __func__,
                      nodes, edges);
            g->nsides = edges;
            edgelist = calloc(edges, sizeof(edge));
            if (edgelist == NULL) {
                return false;
            }
            break;

        case LineGraphEdge:
            matched_params = fscanf(stream, "%u%u", &node1, &node2);
            if (matched_params != 2) {
                log_error("%s: couldn't parse edge: (%u, %u)", __func__, node1,
                          node2);
                goto cleanup_false;
            }

            edgelist[curredge] = (edge){node1, node2};
            hmput(tmpnames, node1, 420);
            hmput(tmpnames, node2, 420);
            curredge++;

            break;

        case LineNewLine:
        case LineCarriageReturn:  // some graphs come from Windows
            break;

        default:
            log_error("%s: couldn't parse graph: found '%c'", __func__, line);
            return false;
        }
    }

    Array *names = ArrayNewSized(nodes);
    for (u32 i = 0; i < nodes; ++i) {
        ArrayAdd(names, tmpnames[i].key);
    }
    if (names->length != nodes) {
        log_error("%s: expected %u nodes, got %u", __func__, nodes,
                  names->length);
        goto cleanup_false;
    }
    arraySort(names);

    for (u32 i = 0; i < names->length; ++i) {
        GraphAddNode(g, ArrayGet(names, i));
    }
    ArrayDestroy(names);

    // make sure to insert the nodes in the correct order!
    // helps simplify the code in the long run
    for (u32 i = 0; i < edges; ++i) {
        GraphConnectNodes(g, edgelist[i].node1, edgelist[i].node2);
    }

    if (curredge != edges) {
        log_error("%s: didn't read the proper lines: expected '%u', got '%u'",
                  __func__, edges, curredge);
        goto cleanup_false;
    }

    if (g->V->length != nodes) {
        log_error("%s: didn't create the proper nodes: expected '%u', got '%u'",
                  __func__, nodes, g->V->length);
        goto cleanup_false;
    }

    if (g->Colors->length != g->V->length) {
        log_error(
            "%s: didn't create the proper colors: expected '%u', got '%u'",
            __func__, g->V->length, g->Colors->length);
        goto cleanup_false;
    }

    free(edgelist);
    hmfree(tmpnames);
    return true;

cleanup_false:
    free(edgelist);
    hmfree(tmpnames);
    return false;
}

GrafoSt *GraphNew(void) {
    GrafoSt *g = calloc(1, sizeof(GrafoSt));
    g->V = ArrayNew();
    g->Order = ArrayNew();
    g->E = EdgeArrayNew();
    g->Colors = ArrayNew();
    g->hash = NULL;
    hmdefault(g->hash, U32_MAX);
    return g;
}
void GraphDestroy(GrafoSt *g) {
    ArrayDestroy(g->V);
    ArrayDestroy(g->Order);
    ArrayDestroy(g->Colors);
    EdgeArrayDestroy(g->E);
    hmfree(g->hash);
    free(g);
}

void GraphAddNode(GrafoSt *g, u32 name) {
    u32 realName = ArrayAdd(g->V, name);
    hmput(g->hash, name, realName);
    ArrayAdd(g->Order, realName);
    ArrayAdd(g->Colors, UNCOLORED);
    EdgeArrayAdd(g->E);
}

// Invariant the node is already in the set.
u32 NameToPos(GrafoSt *g, u32 name) { return hmget(g->hash, name); }

void GraphConnectNodes(GrafoSt *g, u32 name1, u32 name2) {
    u32 pos1 = NameToPos(g, name1);
    if (pos1 == U32_MAX) {
        log_error("%s: nametopos: expected pos for %u", __func__, name1);
        return;
    }
    u32 pos2 = NameToPos(g, name2);
    if (pos2 == U32_MAX) {
        log_error("%s: nametopos: expected pos for %u", __func__, name2);
        return;
    }
    EdgeArrayConnect(g->E, name1, pos1, name2, pos2);
    g->nsides++;
    u32 d1 = g->E->contents[pos1]->length;
    u32 d2 = g->E->contents[pos2]->length;
    u32 d = d1 > d2 ? d1 : d2;
    g->delta = g->delta < d ? d : g->delta;
}

Grafo ConstruccionDelGrafo(void) {
    GrafoSt *g = GraphNew();
    bool ok = GraphParse(g, stdin);
    if (!ok) {
        GraphDestroy(g);
        return NULL;
    }

    u32 colors = Greedy(g);
    if (colors == U32_MAX) {
        GraphDestroy(g);
        return NULL;
    }

    return g;
}

void DestruccionDelGrafo(Grafo G) { GraphDestroy(G); }
Grafo CopiarGrafo(Grafo G) {
    Grafo newG = malloc(sizeof(GrafoSt));
    newG->V = ArrayCopy(G->V);
    newG->Order = ArrayCopy(G->Order);
    newG->E = EdgeArrayCopy(G->E);
    newG->Colors = ArrayCopy(G->Colors);

    newG->nsides = G->nsides;
    newG->delta = G->delta;
    newG->maxcolor = G->maxcolor;

    newG->hash = NULL;
    hmdefault(newG->hash, U32_MAX);

    for (int i = 0; i < hmlen(G->hash); ++i) {
        hmput(newG->hash, G->hash[i].key, G->hash[i].value);
    }

    return newG;
}

void uncolor(Grafo G) {
    G->maxcolor = 0;
    for (u32 i = 0; i < NumeroDeVertices(G); i++) {
        FijarColor(UNCOLORED, i, G);
    }
}

u32 Nombre(u32 i, Grafo G) {
    u32 real = ArrayGet(G->Order, i);
    return ArrayGet(G->V, real);
}

u32 Color(u32 i, Grafo G) {
    if (i >= NumeroDeVertices(G)) {
        return U32_MAX;
    }
    u32 real = ArrayGet(G->Order, i);
    return ArrayGet(G->Colors, real);
}

u32 Grado(u32 i, Grafo G) {
    if (i >= NumeroDeVertices(G)) {
        return U32_MAX;
    }
    u32 real = ArrayGet(G->Order, i);
    Array *connections = EdgeArrayGetConnections(G->E, real);
    return connections->length;
}

char FijarColor(u32 x, u32 i, Grafo G) {
    if (i >= NumeroDeVertices(G)) {
        return 1;
    }

    u32 real = ArrayGet(G->Order, i);
    ArraySet(G->Colors, real, x);

    if (x > G->maxcolor) {
        G->maxcolor = x;
    }

    return 0;
}

char FijarOrden(u32 i, Grafo G, u32 N) {
    if (i >= NumeroDeVertices(G) || N >= NumeroDeVertices(G)) {
        return 1;
    }
    ArraySet(G->Order, i, N);

    return 0;
}

// Get the jth neighbour of the ith node in the current order.
u32 NthNeighbour(u32 j, u32 i, Grafo G) {
    u32 vertex = ArrayGet(G->Order, i);
    Array *neighbours = EdgeArrayGetConnections(G->E, vertex);
    return ArrayGet(neighbours, j);
}

u32 ColorVecino(u32 j, u32 i, Grafo G) {
    if (i >= NumeroDeVertices(G) || j >= Grado(i, G)) {
        return U32_MAX;
    }
    u32 neighbour = NthNeighbour(j, i, G);
    return ArrayGet(G->Colors, NameToPos(G, neighbour));
}

u32 NombreVecino(u32 j, u32 i, Grafo G) { return NthNeighbour(j, i, G); }

u32 OrdenVecino(u32 j, u32 i, Grafo G) {
    u32 neighbour = NthNeighbour(j, i, G);  // Name
    u32 internal = NameToPos(G, neighbour);
    for (u32 i = 0; i < G->Order->length; i++) {
        if (ArrayGet(G->Order, i) == internal) return i;
    }
    return U32_MAX; // Should never be reached.
}
