#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "GrafoSt2020.h"
#include "log.h"
#include "veinteveinte.h"

#define MAX_REORDERS 1000

enum Reorder { reviertebc, chicograndebc, aleatorizarvertices };

bool testReorder(Grafo g, enum Reorder r) {
    u32 color = UNCOLORED;
    u32 mincolor = UNCOLORED;
    clock_t t;
    char err;
    clock_t max = {0};

    for (int i = 0; i < MAX_REORDERS; ++i) {
        t = clock();

        switch (r) {
        case reviertebc:
            err = RevierteBC(g);
            break;
        case chicograndebc:
            err = ChicoGrandeBC(g);
            break;

        case aleatorizarvertices:
            err = AleatorizarVertices(g, 420);  // FIXME: choose a random seed
            break;

        default:
            log_error("%s: didn't specify a reorder", __func__);
            return false;
        }

        if (err == 1) {
            log_error("%s: failed order, cleaning up", __func__);
            return false;
        }

        color = Greedy(g);
        if (color == U32_MAX) {
            log_error("%s: failed greedy, cleaning up", __func__);
            return false;
        }
        if (color < mincolor) {
            mincolor = color;
            log_info("%s: reorder #%d: got color %u", __func__, i, mincolor);
        }

        t = clock() - t;
        if (t > max) {
            max = t;
        }
    }

    log_info("%s: max time for reordering was %.5fs", __func__,
             ((double)t) / CLOCKS_PER_SEC);
    return true;
}

int main(int argc, char *argv[]) {
    // only emit warnings, not debug info
    log_set_level(LOG_INFO);

    if (argc < 1) {
        log_warn("%s: expected 1 argument", __func__);
        return 1;
    }

    FILE *f = fopen(argv[1], "r+");
    if (f == NULL) {
        log_error("%s: opening graph '%s': %s", __func__, argv[1],
                  strerror(errno));
        fclose(f);
        return 1;
    }

    GrafoSt *g = GraphNew();

    log_info("%s: begin parsing '%s'...", __func__, argv[1]);
    clock_t t = clock();

    bool ok = GraphParse(g, f);

    t = clock() - t;
    log_info("%s: finished parsing in %.5fs", __func__,
             ((double)t) / CLOCKS_PER_SEC);

    fclose(f);
    if (!ok) {
        log_error("%s: failed to parse '%s', cleaning up", __func__, argv[1]);
        GraphDestroy(g);
        return 1;
    }

    u32 color = Greedy(g);
    if (color == U32_MAX) {
        log_error("%s: failed greedy, cleaning up", __func__);
        GraphDestroy(g);
        return 1;
    }
    log_info("%s: initial greedy: got color %u", __func__, color);

    log_info("%s: begin welshpowell...", __func__);
    t = clock();

    char err = WelshPowell(g);
    if (err == 1) {
        log_error("%s: error on welshpowell, aborting", __func__);
        GraphDestroy(g);
        return 1;
    }

    t = clock() - t;
    log_info("%s: finished welshpowell in %.5fs", __func__,
             ((double)t) / CLOCKS_PER_SEC);

    log_info("%s: begin greedy with new order...", __func__);
    color = Greedy(g);
    if (color == U32_MAX) {
        log_error("%s: failed greedy, cleaning up", __func__);
        GraphDestroy(g);
        return 1;
    }
    log_info("%s: greedy after welshpowell: got color %u", __func__, color);

    log_info("%s: test reordering with ChicoGrandeBC...", __func__);
    ok = testReorder(g, chicograndebc);

    log_info("%s: test reordering with RevierteBC...", __func__);
    ok = testReorder(g, reviertebc);

    log_info("%s: test reordering with AleatorizarVertices...", __func__);
    ok = testReorder(g, aleatorizarvertices);

    log_info("%s: begin bipartito...", __func__);
    t = clock();

    char res = Bipartito(g);

    t = clock() - t;
    log_info("%s: finished bipartito in %.5fs", __func__,
             ((double)t) / CLOCKS_PER_SEC);
    log_info("%s: bipartito: %s", __func__, res == 1 ? "yes" : "no");

    GraphDestroy(g);
    return 0;
}
