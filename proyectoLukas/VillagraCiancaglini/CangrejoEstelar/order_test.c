#include <errno.h>
#include <signal.h>
#include <stdbool.h>

#include "log.h"
#include "order.h"

bool TestNaturalOrder(void) {
    GrafoSt *g = GraphNew();
    if (g == NULL) {
        log_error("%s: failed to create a graph.", __func__);
        GraphDestroy(g);
        return false;
    }

    u32 nodes[] = {2, 3, 1, 6};
    u32 length = 4;
    u32 natural[] = {1, 2, 3, 6};

    for (u32 i = 0; i < length; i++) GraphAddNode(g, nodes[i]);

    Array *a = NaturalOrder(g);

    for (u32 i = 0; i < a->length; i++) {
        u32 v = ArrayGet(a, i);
        if (v != natural[i]) {
            log_error("%s: bad order at %u, expected %u got %u", __func__, i,
                      natural[i], v);
            ArrayDestroy(a);
            GraphDestroy(g);
            return false;
        }
    }

    ArrayDestroy(a);
    GraphDestroy(g);
    return true;
}

bool TestNaturalTranslate(void) {
    GrafoSt *g = GraphNew();
    if (g == NULL) {
        log_error("%s: failed to create a graph.", __func__);
        GraphDestroy(g);
        return false;
    }

    u32 nodes[] = {1, 2, 0, 7, 10};
    u32 length = 4;
    u32 positions[] = {1, 2, 0, 3, 4};

    for (u32 i = 0; i < length; i++) GraphAddNode(g, nodes[i]);

    Array *a = NaturalOrder(g);

    for (u32 i = 0; i < length; i++) {
        u32 n_pos = TranslateToNatural(g, a, i);
        if (n_pos == U32_MAX) {
            log_error("%s: bad position at %u, node %u not found", __func__, i,
                      nodes[i]);
            ArrayDestroy(a);
            GraphDestroy(g);
            return false;
        }
        if (n_pos != positions[i]) {
            log_error("%s: bad position at %u, expected %u got %u", __func__, i,
                      positions[i], n_pos);
            ArrayDestroy(a);
            GraphDestroy(g);
            return false;
        }
    }

    ArrayDestroy(a);
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

    char succcess = WelshPowell(g);
    if (succcess != 0) {
        log_error("%s: welshpowell failed.", __func__);
        GraphDestroy(g);
        return false;
    }

    for (u32 i = 0; i < v_length - 1; i++) {
        u32 deg_cur = Grado(i, g);
        u32 deg_nxt = Grado(i + 1, g);
        if (deg_cur < deg_nxt) {
            log_error("%s: bad degree relation at %u: %u < %u", __func__, i,
                      deg_cur, deg_nxt);
            GraphDestroy(g);
            return false;
        }
    }

    GraphDestroy(g);
    return true;
}

bool TestChicoGrandeBC(void) {
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

    char succcess = ChicoGrandeBC(g);
    if (succcess != 0) {
        log_error("%s: ChicoGrandeBC failed.", __func__);
        GraphDestroy(g);
        return false;
    }

    u32 expected_counts[] = {5, 4, 1};
    u32 gotten_counts[] = {0, 0, 0};
    for (u32 i = 0; i < v_length; i++) {
        u32 color = Color(i, g);
        if (color > 2) {
            log_error("%s: got a higher color than expected!", __func__);
            GraphDestroy(g);
            return false;
        }
        gotten_counts[color]++;
    }

    for (u32 i = 0; i < 3; i++) {
        if (gotten_counts[i] != expected_counts[i]) {
            log_error("%s: bad count, expected %u got %u", __func__,
                      expected_counts[i], gotten_counts[i]);
            GraphDestroy(g);
            return false;
        }
    }

    u32 expected_blocks[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (u32 i = 0; i < v_length; i++) {
        expected_blocks[i] = gotten_counts[Color(i, g)];
    }
    for (u32 i = 0; i < v_length - 1; i++) {
        if (expected_blocks[i] < expected_blocks[i + 1]) {
            log_error("%s: block size missmatch at %u", __func__, i);
            GraphDestroy(g);
            return false;
        }
    }

    GraphDestroy(g);
    return true;
}

bool TestRevierteBC(void) {
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

    char succcess = RevierteBC(g);
    if (succcess != 0) {
        log_error("%s: RevierteBC failed.", __func__);
        GraphDestroy(g);
        return false;
    }

    for (u32 i = 0; i < v_length - 1; i++) {
        if (Color(i, g) < Color(i + 1, g)) {
            log_error("%s: bad color at %u: %u > %u", __func__, Color(i, g),
                      Color(i + 1, g));
            GraphDestroy(g);
            return false;
        }
    }

    GraphDestroy(g);
    return true;
}

int main(void) {
    int ntests = 5;

    // array of function pointers - moral syntax, saves plenty of lines
    bool (*tests[])(void) = {TestNaturalOrder, TestNaturalTranslate,
                             TestWelshPowell, TestChicoGrandeBC,
                             TestRevierteBC};

    for (int i = 0; i < ntests; ++i) {
        bool ok = tests[i]();
        if (!ok) {
            return 1;
        }
    }

    return 0;
}
