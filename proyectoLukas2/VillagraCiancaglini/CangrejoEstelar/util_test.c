#include <signal.h>
#include <stdbool.h>

#include "log.h"
#include "veinteveinte.h"

bool TestVertexCount(void) {
    Grafo g = GraphNew();
    GraphAddNode(g, 1);
    GraphAddNode(g, 2);
    u32 vcount = NumeroDeVertices(g);
    if (vcount != 2) {
        log_error("%s: Wrong number of vertices, expected 2, got %u", __func__,
                  vcount);
        GraphDestroy(g);
        return false;
    }
    GraphDestroy(g);
    return true;
}

bool TestSideCount(void) {
    Grafo g = GraphNew();
    GraphAddNode(g, 1);
    GraphAddNode(g, 2);
    GraphConnectNodes(g, 1, 2);
    u32 side_count = NumeroDeLados(g);
    if (side_count != 1) {
        log_error("%s: Wrong number of sides, expected 1, got %u", __func__,
                  side_count);
        GraphDestroy(g);
        return false;
    }

    GraphDestroy(g);
    return true;
}

bool TestDelta(void) {
    Grafo g = GraphNew();
    GraphAddNode(g, 0);
    GraphAddNode(g, 1);
    GraphAddNode(g, 2);
    GraphAddNode(g, 3);
    GraphConnectNodes(g, 0, 3);
    GraphConnectNodes(g, 1, 3);
    GraphConnectNodes(g, 2, 3);

    u32 delta = Delta(g);
    if (delta != 3) {
        log_error("%s: Wrong delta, expected 3, got %u", __func__, delta);
        GraphDestroy(g);
        return false;
    }
    GraphDestroy(g);
    return true;
}

int main(void) {
    int ntests = 3;

    // array of function pointers - moral syntax, saves plenty of lines
    bool (*tests[])(void) = {TestVertexCount, TestSideCount, TestDelta};

    for (int i = 0; i < ntests; ++i) {
        bool ok = tests[i]();
        if (!ok) {
            return 1;
        }
    }

    return 0;
}
