#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>

#include "array.h"
#include "log.h"
#include "veinteveinte.h"

bool TestNodeAddition(void) {
    GrafoSt *g = GraphNew();
    u32 realPos;

    GraphAddNode(g, 420);
    realPos = NameToPos(g, 420);
    if (realPos != 0) {
        log_error("%s: wrong position. Expected %u got %u.", __func__, 0,
                  realPos);
        GraphDestroy(g);
        return false;
    }

    GraphAddNode(g, 69);
    GraphAddNode(g, 413);

    realPos = NameToPos(g, 420);
    if (realPos != 0) {
        log_error("%s: wrong position. Expected %u got %u.", __func__, 0,
                  realPos);
        GraphDestroy(g);
        return false;
    }

    realPos = NameToPos(g, 413);
    if (realPos != 2) {
        log_error("%s: wrong position. Expected %u got %u.", __func__, 2,
                  realPos);
        GraphDestroy(g);
        return false;
    }

    GraphDestroy(g);
    return true;
}

bool TestCreate(void) {
    GrafoSt *g = GraphNew();
    if (g == NULL) {
        log_error("%s: failed to create a graph.", __func__);
        GraphDestroy(g);
        return false;
    }
    GraphDestroy(g);
    return true;
}

bool TestNames(void) {
    Grafo g = GraphNew();
    if (g == NULL) {
        log_error("%s: failed to create a graph.", __func__);
        return false;
    }
    GraphAddNode(g, 23);
    GraphAddNode(g, 42);
    u32 name = Nombre(0, g);
    if (name != 23) {
        log_error("%s: bad name, expected %u got %u", __func__, 23, name);
        GraphDestroy(g);
        return false;
    }
    GraphDestroy(g);
    return true;
}

bool TestDegree(void) {
    Grafo g = GraphNew();
    if (g == NULL) {
        log_error("%s: failed to create a graph.", __func__);
        return false;
    }
    GraphAddNode(g, 23);
    GraphAddNode(g, 42);
    GraphAddNode(g, 476);

    GraphConnectNodes(g, 23, 42);
    GraphConnectNodes(g, 23, 476);

    u32 degree = Grado(0, g);
    if (degree != 2) {
        log_error("%s: bad degree: expected %u got %u", __func__, 2, degree);
        GraphDestroy(g);
        return false;
    }

    GraphDestroy(g);
    return true;
}

bool TestCopiarGrafo(void) {
    Grafo g = GraphNew();

    for (u32 i = 0; i <= 10; ++i) {
        GraphAddNode(g, i);
    }

    for (u32 i = 10; i > 0; --i) {
        GraphConnectNodes(g, i, 0);
    }

    Grafo newG = CopiarGrafo(g);
    if (!arrayEqual(g->V, newG->V) || !arrayEqual(g->Order, newG->Order) ||
        !arrayEqual(g->Colors, newG->Colors) ||
        g->E->length != newG->E->length || g->nsides != newG->nsides ||
        g->delta != newG->delta) {
        goto cleanup_false;
    }

    for (u32 i = 0; i < g->E->length; ++i) {
        if (!arrayEqual(g->E->contents[i], newG->E->contents[i])) {
            goto cleanup_false;
        }
    }

    GraphDestroy(g);
    GraphDestroy(newG);
    return true;

cleanup_false:
    GraphDestroy(newG);
    GraphDestroy(g);
    return false;
}

bool TestOrdenVecino(void) {
    Grafo g = GraphNew();

    GraphAddNode(g, 1);
    GraphAddNode(g, 2);
    GraphConnectNodes(g, 1, 2);

    u32 place = OrdenVecino(0, 1, g);
    if (place != 0) {
        log_error("%s: Bad order: expected %u got %u", __func__, 0, place);
        GraphDestroy(g);
        return false;
    }

    GraphDestroy(g);
    return true;
}

bool TestNombreVecino(void) {
    Grafo g = GraphNew();

    GraphAddNode(g, 1);
    GraphAddNode(g, 2);
    GraphConnectNodes(g, 1, 2);

    u32 name = NombreVecino(0, 1, g);
    if (name != 1) {
        log_error("%s: Bad name: expected %u got %u", __func__, 1, name);
        GraphDestroy(g);
        return false;
    }

    GraphDestroy(g);
    return true;
}

bool TestColorVecino(void) {
    Grafo g = GraphNew();

    GraphAddNode(g, 1);
    GraphAddNode(g, 2);
    GraphConnectNodes(g, 1, 2);

    u32 color = ColorVecino(0, 1, g);
    if (color != UNCOLORED) {
        log_error("%s: Bad color: expected %u got %u", __func__, UNCOLORED,
                  color);
        GraphDestroy(g);
        return false;
    }

    GraphDestroy(g);
    return true;
}

bool TestFijarColor(void) {
    Grafo g = GraphNew();

    GraphAddNode(g, 23);
    GraphAddNode(g, 42);
    GraphAddNode(g, 476);

    GraphConnectNodes(g, 23, 42);
    GraphConnectNodes(g, 23, 476);

    char res = FijarColor(0, 0, g);
    if (res == 1) {
        log_error("%s: failed coloring", __func__);
        goto cleanup_false;
    }

    res = FijarColor(1, 1, g);
    if (res == 1) {
        log_error("%s: failed coloring", __func__);
        goto cleanup_false;
    }

    u32 color = ColorVecino(0, 0, g);
    if (color != 1) {
        log_error("%s: expected %u, got %u", __func__, 1, color);
        goto cleanup_false;
    }

    res = FijarColor(0, 10, g);
    if (res == 0) {
        log_error("%s: failed coloring", __func__);
        goto cleanup_false;
    }

    GraphDestroy(g);
    return true;

cleanup_false:
    GraphDestroy(g);
    return false;
}

bool TestFijarOrden(void) {
    Grafo g = GraphNew();

    GraphAddNode(g, 23);
    GraphAddNode(g, 42);
    GraphAddNode(g, 476);

    GraphConnectNodes(g, 23, 42);
    GraphConnectNodes(g, 23, 476);

    char res = FijarOrden(0, g, 1);
    if (res == 1) {
        log_error("%s: failed order setting", __func__);
        GraphDestroy(g);
        return false;
    }

    u32 order = ArrayGet(g->Order, 0);
    if (order != 1) {
        log_error("%s: expected %u, got %u", __func__, 1, order);
        GraphDestroy(g);
        return false;
    }

    res = FijarOrden(3, g, 0);
    if (res != 1) {
        log_error("%s: failed order setting", __func__);
        GraphDestroy(g);
        return false;
    }

    GraphDestroy(g);
    return true;
}

bool TestNumCCs(void) {
    GrafoSt *g = GraphNew();
    if (g == NULL) {
        log_error("%s: failed to create a graph.", __func__);
        GraphDestroy(g);
        return false;
    }

    u32 nodes[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    struct {
        u32 x;
        u32 y;
    } cons[] = {{1, 3}, {1, 4}, {3, 6}, {4, 7}, {6, 9},
                {7, 9}, {2, 5}, {5, 8}, {2, 8}};
    /*
      This is a gnostic circle graph. 3 Connex componenets, loops and a single
      node. Perfect for testing shit.
     */
    u32 v_length = 10;
    u32 e_length = 9;
    u32 coloring[] = {0, 0, 0, 1, 1,
                      1, 0, 0, 2, 1};  // Manual coloring. Also best coloring.

    for (u32 i = 0; i < v_length; i++) GraphAddNode(g, nodes[i]);
    for (u32 i = 0; i < e_length; i++)
        GraphConnectNodes(g, cons[i].x, cons[i].y);
    for (u32 i = 0; i < v_length; i++) FijarColor(coloring[i], i, g);

    u32 count = NumCCs(g);
    if (count != 3) {
        log_error("%s: count failed: expected 3 got %u.", __func__, count);
        GraphDestroy(g);
        return false;
    }

    GraphDestroy(g);
    return true;
}

bool TestSwitchColores(void) {
    GrafoSt *g = GraphNew();
    if (g == NULL) {
        log_error("%s: failed to create a graph.", __func__);
        GraphDestroy(g);
        return false;
    }

    u32 nodes[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    struct {
        u32 x;
        u32 y;
    } cons[] = {{1, 3}, {1, 4}, {3, 6}, {4, 7}, {6, 9},
                {7, 9}, {2, 5}, {5, 8}, {2, 8}};
    /*
      This is a gnostic circle graph. 3 Connex componenets, loops and a single
      node. Perfect for testing shit.
     */
    u32 v_length = 10;
    u32 e_length = 9;
    u32 coloring[] = {0, 0, 0, 1, 1,
                      1, 0, 0, 2, 1};  // Manual coloring. Also best coloring.

    for (u32 i = 0; i < v_length; i++) GraphAddNode(g, nodes[i]);
    for (u32 i = 0; i < e_length; i++)
        GraphConnectNodes(g, cons[i].x, cons[i].y);
    for (u32 i = 0; i < v_length; i++) FijarColor(coloring[i], i, g);

    char res = SwitchColores(g, 999, 998);
    if (res != 1) {
        log_error("%s: bad colors should fail", __func__);
        GraphDestroy(g);
        return false;
    }

    res = SwitchColores(g, 0, 1);
    if (res != 0) {
        log_error("%s: good colors should succeed", __func__);
        GraphDestroy(g);
        return false;
    }

    u32 new_coloring[] = {1, 1, 1, 0, 0, 0, 1, 1, 2, 0};
    for (u32 i = 0; i < v_length; i++) {
        u32 color = Color(i, g);
        if (color != new_coloring[i]) {
            log_error("%s: bad color at %u: expected %u got %u", __func__, i,
                      new_coloring[i], color);
            GraphDestroy(g);
            return false;
        }
    }

    GraphDestroy(g);
    return true;
}

bool TestAleatorizarVertices(void) {
    GrafoSt *g = GraphNew();
    if (g == NULL) {
        log_error("%s: failed to create a graph.", __func__);
        GraphDestroy(g);
        return false;
    }

    u32 nodes[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    struct {
        u32 x;
        u32 y;
    } cons[] = {{1, 3}, {1, 4}, {3, 6}, {4, 7}, {6, 9},
                {7, 9}, {2, 5}, {5, 8}, {2, 8}};
    /*
      This is a gnostic circle graph. 3 Connex componenets, loops and a single
      node. Perfect for testing shit.
     */
    u32 v_length = 10;
    u32 e_length = 9;
    u32 coloring[] = {0, 0, 0, 1, 1,
                      1, 0, 0, 2, 1};  // Manual coloring. Also best coloring.

    for (u32 i = 0; i < v_length; i++) GraphAddNode(g, nodes[i]);
    for (u32 i = 0; i < e_length; i++)
        GraphConnectNodes(g, cons[i].x, cons[i].y);
    for (u32 i = 0; i < v_length; i++) FijarColor(coloring[i], i, g);

    char res = AleatorizarVertices(g, 12);
    if (res != 0) {
        log_error("%s: AleatorizarVertices failed", __func__);
        DestruccionDelGrafo(g);
        return false;
    }

    u32 new_order[] = {8, 6, 1, 4, 3, 2, 9, 7, 0, 5};
    for (u32 i = 0; i < v_length; i++) {
        if (Nombre(i, g) != new_order[i]) {
            log_error("%s: bad order at %u: expected %u got %u", __func__, i,
                      new_order[i], Nombre(i, g));
            DestruccionDelGrafo(g);
            return false;
        }
    }

    DestruccionDelGrafo(g);
    return true;
}

int main(void) {
    int ntests = 13;

    // array of function pointers - moral syntax, saves plenty of lines
    bool (*tests[])(void) = {TestCreate,
                             TestNodeAddition,
                             TestNames,
                             TestDegree,
                             TestCopiarGrafo,
                             TestFijarColor,
                             TestFijarOrden,
                             TestOrdenVecino,
                             TestNombreVecino,
                             TestColorVecino,
                             TestNumCCs,
                             TestSwitchColores,
                             TestAleatorizarVertices};

    for (int i = 0; i < ntests; ++i) {
        bool ok = tests[i]();
        if (!ok) {
            return 1;
        }
    }

    return 0;
}
