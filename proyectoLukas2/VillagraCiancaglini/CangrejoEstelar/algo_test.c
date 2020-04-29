#include <stdbool.h>

#include "GrafoSt2020.h"
#include "log.h"
#include "veinteveinte.h"

bool TestGreedy(void) {
    Grafo g = GraphNew();

    GraphAddNode(g, 666);
    GraphAddNode(g, 420);
    GraphConnectNodes(g, 666, 420);

    log_info("%s: begin on graph with nodes=%u, sides=%u", __func__,
             NumeroDeVertices(g), NumeroDeLados(g));
    u32 colors = Greedy(g);
    if (colors != 2) {
        log_error("%s: failed coloring, expected %u, got %u", __func__, 2,
                  colors);
        GraphDestroy(g);
        return false;
    }

    log_info("%s: properly colored graph, cleaning up...", __func__);
    GraphDestroy(g);

    // test with a K3, color should be 3
    g = GraphNew();
    GraphAddNode(g, 666);
    GraphAddNode(g, 420);
    GraphAddNode(g, 69);
    GraphConnectNodes(g, 666, 420);
    GraphConnectNodes(g, 666, 69);
    GraphConnectNodes(g, 420, 69);

    log_info("%s: begin on graph with nodes=%u, sides=%u", __func__,
             NumeroDeVertices(g), NumeroDeLados(g));
    colors = Greedy(g);
    if (colors != 3) {
        log_error("%s: failed coloring, expected %u, got %u", __func__, 3,
                  colors);
        GraphDestroy(g);
        return false;
    }

    log_info("%s: properly colored graph, cleaning up...", __func__);
    GraphDestroy(g);

    // test with a K3 + 1, color should be 3
    g = GraphNew();
    GraphAddNode(g, 666);
    GraphAddNode(g, 420);
    GraphAddNode(g, 69);
    GraphAddNode(g, 42);
    GraphConnectNodes(g, 666, 420);
    GraphConnectNodes(g, 666, 69);
    GraphConnectNodes(g, 420, 69);
    GraphConnectNodes(g, 42, 69);

    log_info("%s: begin on graph with nodes=%u, sides=%u", __func__,
             NumeroDeVertices(g), NumeroDeLados(g));
    colors = Greedy(g);
    if (colors != 3) {
        log_error("%s: failed coloring, expected %u, got %u", __func__, 3,
                  colors);
        GraphDestroy(g);
        return false;
    }
    GraphDestroy(g);
    return true;
}

bool TestBipartito(void) {
    Grafo g = GraphNew();

    // first group
    GraphAddNode(g, 666);
    GraphAddNode(g, 420);

    // second group
    GraphAddNode(g, 123);
    GraphAddNode(g, 321);

    GraphConnectNodes(g, 666, 123);
    GraphConnectNodes(g, 666, 321);
    GraphConnectNodes(g, 420, 123);
    GraphConnectNodes(g, 420, 321);

    char res = Bipartito(g);
    if (res == 0) {
        log_error("%s: didn't identify bipartito graph as such", __func__);
        GraphDestroy(g);
        return false;
    }

    GraphDestroy(g);
    return true;
}

bool TestOddCycle(void) {
    GrafoSt *g = GraphNew();
    if (g == NULL) {
        log_error("%s: failed to create a graph.", __func__);
        GraphDestroy(g);
        return false;
    }

    u32 nodes[] = {0, 1, 2, 3, 4};
    struct {
        u32 x;
        u32 y;
    } cons[] = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}};
    u32 v_length = 5;
    u32 e_length = 5;
    u32 expected_coloring[] = {0, 1, 0, 1, 2};
    u32 expected_colors = 3;

    for (u32 i = 0; i < v_length; i++) GraphAddNode(g, nodes[i]);
    for (u32 i = 0; i < e_length; i++)
        GraphConnectNodes(g, cons[i].x, cons[i].y);

    u32 colors = Greedy(g);
    if (colors != expected_colors) {
        log_error("%s: Greedy failed. Expected %u colors, got %u", __func__,
                  expected_colors, colors);
        GraphDestroy(g);
        return false;
    }

    for (u32 i = 0; i < v_length; i++) {
        if (Color(i, g) != expected_coloring[i]) {
            log_error("%s: bad color at %u: got %u expected %u", __func__,
                      Color(i, g), expected_coloring[i]);
            GraphDestroy(g);
            return false;
        }
    }

    GraphDestroy(g);
    return true;
}

bool TestEvenCycle(void) {
    GrafoSt *g = GraphNew();
    if (g == NULL) {
        log_error("%s: failed to create a graph.", __func__);
        GraphDestroy(g);
        return false;
    }

    u32 nodes[] = {0, 1, 2, 3, 4, 5};
    struct {
        u32 x;
        u32 y;
    } cons[] = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 0}};
    u32 v_length = 6;
    u32 e_length = 6;
    u32 expected_coloring[] = {0, 1, 0, 1, 0, 1};
    u32 expected_colors = 2;

    for (u32 i = 0; i < v_length; i++) GraphAddNode(g, nodes[i]);
    for (u32 i = 0; i < e_length; i++)
        GraphConnectNodes(g, cons[i].x, cons[i].y);

    u32 colors = Greedy(g);
    if (colors != expected_colors) {
        log_error("%s: Greedy failed. Expected %u colors, got %u", __func__,
                  expected_colors, colors);
        GraphDestroy(g);
        return false;
    }

    for (u32 i = 0; i < v_length; i++) {
        if (Color(i, g) != expected_coloring[i]) {
            log_error("%s: bad color at %u: got %u expected %u", __func__, i,
                      Color(i, g), expected_coloring[i]);
            GraphDestroy(g);
            return false;
        }
    }

    GraphDestroy(g);
    return true;
}

bool TestGnosticGraph(void) {
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
    u32 v_length = 10;
    u32 e_length = 9;
    u32 expected_coloring[] = {0, 0, 0, 1, 1, 1, 0, 0, 2, 1};
    u32 expected_colors = 3;

    for (u32 i = 0; i < v_length; i++) GraphAddNode(g, nodes[i]);
    for (u32 i = 0; i < e_length; i++)
        GraphConnectNodes(g, cons[i].x, cons[i].y);

    u32 colors = Greedy(g);
    if (colors != expected_colors) {
        log_error("%s: Greedy failed. Expected %u colors, got %u", __func__,
                  expected_colors, colors);
        GraphDestroy(g);
        return false;
    }

    for (u32 i = 0; i < v_length; i++) {
        if (Color(i, g) != expected_coloring[i]) {
            log_error("%s: bad color at %u: got %u expected %u", __func__, i,
                      Color(i, g), expected_coloring[i]);
            GraphDestroy(g);
            return false;
        }
    }

    GraphDestroy(g);
    return true;
}

bool TestWelshPowell(void) {
    GrafoSt *g = GraphNew();
    if (g == NULL) {
        log_error("%s: failed to create a graph.", __func__);
        GraphDestroy(g);
        return false;
    }

    u32 nodes[] = {0, 1, 2, 3, 4, 5};
    struct {
        u32 x;
        u32 y;
    } cons[] = {{0, 1}, {0, 2}, {0, 4}, {0, 3}, {2, 3}, {4, 3}, {3, 5}};
    u32 v_length = 6;
    u32 e_length = 7;
    u32 expected_coloring[] = {0, 1, 2, 2, 1, 0};
    u32 expected_colors = 3;

    for (u32 i = 0; i < v_length; i++) GraphAddNode(g, nodes[i]);
    for (u32 i = 0; i < e_length; i++)
        GraphConnectNodes(g, cons[i].x, cons[i].y);

    char res = WelshPowell(g);
    if (res != 0) {
        log_error("%s: Welshpowell failed.", __func__);
        GraphDestroy(g);
        return false;
    }
    u32 colors = Greedy(g);
    if (colors != expected_colors) {
        log_error("%s: Greedy failed. Expected %u colors, got %u", __func__,
                  expected_colors, colors);
        GraphDestroy(g);
        return false;
    }

    for (u32 i = 0; i < v_length; i++) {
        if (Color(i, g) != expected_coloring[i]) {
            log_error("%s: bad color at %u: got %u expected %u", __func__, i,
                      Color(i, g), expected_coloring[i]);
            GraphDestroy(g);
            return false;
        }
    }

    GraphDestroy(g);
    return true;
}

bool TestComplete(void) {
    GrafoSt *g = GraphNew();
    if (g == NULL) {
        log_error("%s: failed to create a graph.", __func__);
        GraphDestroy(g);
        return false;
    }

    u32 nodes[] = {1, 2, 3, 4, 5};
    struct {
        u32 x;
        u32 y;
    } cons[] = {{1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 3},
                {2, 4}, {2, 5}, {3, 4}, {3, 5}, {4, 5}};
    u32 v_length = 5;
    u32 e_length = 10;
    u32 expected_colors = 5;

    for (u32 i = 0; i < v_length; i++) GraphAddNode(g, nodes[i]);
    for (u32 i = 0; i < e_length; i++)
        GraphConnectNodes(g, cons[i].x, cons[i].y);

    u32 colors = Greedy(g);
    if (colors != expected_colors) {
        log_error("%s: Greedy failed. Expected %u colors, got %u", __func__,
                  expected_colors, colors);
        GraphDestroy(g);
        return false;
    }

    char res = WelshPowell(g);
    if (res != 0) {
        log_error("%s: WelshPowell failed!", __func__);
        GraphDestroy(g);
        return false;
    }

    colors = Greedy(g);
    if (colors != expected_colors) {
        log_error("%s: Greedy failed. Expected %u colors, got %u", __func__,
                  expected_colors, colors);
        GraphDestroy(g);
        return false;
    }

    res = ChicoGrandeBC(g);
    if (res != 0) {
        log_error("%s: ChicoGrandeBC failed!", __func__);
        GraphDestroy(g);
        return false;
    }

    colors = Greedy(g);
    if (colors != expected_colors) {
        log_error("%s: Greedy failed. Expected %u colors, got %u", __func__,
                  expected_colors, colors);
        GraphDestroy(g);
        return false;
    }

    res = RevierteBC(g);
    if (res != 0) {
        log_error("%s: RevierteBC failed!", __func__);
        GraphDestroy(g);
        return false;
    }

    colors = Greedy(g);
    if (colors != expected_colors) {
        log_error("%s: Greedy failed. Expected %u colors, got %u", __func__,
                  expected_colors, colors);
        GraphDestroy(g);
        return false;
    }

    GraphDestroy(g);
    return true;
}

int main(void) {
    int ntests = 7;

    // array of function pointers - moral syntax, saves plenty of lines
    bool (*tests[])(void) = {TestGreedy,    TestBipartito,    TestOddCycle,
                             TestEvenCycle, TestGnosticGraph, TestWelshPowell,
                             TestComplete};

    for (int i = 0; i < ntests; ++i) {
        bool ok = tests[i]();
        if (!ok) {
            return 1;
        }
    }

    return 0;
}
