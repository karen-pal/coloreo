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

void VerificarWelshPowell(Grafo G) {
    bool ok = true;
    u32 n = NumeroDeVertices(G);
    for (u32 i = 0; i < n - 1 && ok; i++) {
        if (Grado(i, G) < Grado(i + 1, G)) {
            log_error("%s: bad order for nodes %u and %u: %u < %u", __func__, i,
                      i + 1, Grado(i, G), Grado(i + 1, G));
            ok = false;
        }
    }

    if (!ok) {
        Array *degrees = ArrayNewSized(Delta(G));
        for (u32 i = 0; i < n; ++i) {
            ArrayAdd(degrees, Grado(i, G));
        }

        char *s = arrayString(degrees, 10);
        log_error("%s: degrees: %s", __func__, s);
        free(s);
        ArrayDestroy(degrees);

        s = arrayString(G->V, 10);
        log_error("%s: names: %s", __func__, s);
        free(s);
        return;
    }

    log_info("%s: ok", __func__);
}

void VerificarRevierteBC(Grafo G) {
    bool ok = true;
    u32 n = NumeroDeVertices(G);
    for (u32 i = 0; i < n - 1 && ok; i++) {
        if (Color(i, G) > Color(i + 1, G)) {
            log_error("%s: bad color for nodes %u and %u: %u > %u", __func__, i,
                      i + 1, Color(i, G), Color(i + 1, G));
            ok = false;
        }
    }

    if (!ok) {
        Array *colors = ArrayNewSized(Delta(G));
        for (u32 i = 0; i < n; ++i) {
            ArrayAdd(colors, Color(i, G));
        }

        char *s = arrayString(colors, 10);
        log_error("%s: colors: %s", __func__, s);
        free(s);
        ArrayDestroy(colors);

        s = arrayString(G->V, 10);
        log_error("%s: names: %s", __func__, s);
        free(s);
        return;
    }
    log_info("%s: ok", __func__);
}

void VerificarChicoGrandeBC(Grafo G) {
    bool ok = true;
    u32 n = NumeroDeVertices(G);
    u32 prevBlockSize = 0;
    u32 curBlockSize = 1;

    for (u32 i = 0; i < n - 1 && ok; i++) {
        if (Color(i, G) == Color(i + 1, G)) {
            curBlockSize++;
            continue;
        }

        if (prevBlockSize > curBlockSize) {
            log_error("%s: failed color block order: %u > %u", __func__,
                      prevBlockSize, curBlockSize);
            ok = false;
        }

        prevBlockSize = curBlockSize;
        curBlockSize = 1;
    }

    if (!ok) {
        Array *colors = ArrayNewSized(Delta(G));
        for (u32 i = 0; i < n; ++i) {
            ArrayAdd(colors, Color(i, G));
        }

        char *s = arrayString(colors, 10);
        log_error("%s: colors: %s", __func__, s);
        free(s);
        ArrayDestroy(colors);

        s = arrayString(G->V, 10);
        log_error("%s: names: %s", __func__, s);
        free(s);
        return;
    }
}

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
            if (err == 0 && (i % 2 == 0)) {
                VerificarRevierteBC(g);
            }
            break;
        case chicograndebc:
            err = ChicoGrandeBC(g);
            if (err == 0) {
                VerificarChicoGrandeBC(g);
            }

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
        if (color > mincolor) {
            log_warn("%s: reorder #%d: got bigger color %u", __func__, i,
                     color);
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
    bool ok = GraphParse(g, f);
    log_info("%s: finished parsing", __func__);

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

    log_info("welshpowell: begin...");
    char err = WelshPowell(g);
    if (err == 1) {
        log_error("welshpowell: aborting");
        GraphDestroy(g);
        return 1;
    }
    log_info("welshpowell: finished");
    log_info("welshpowell: verifying...");
    VerificarWelshPowell(g);

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
    char res = Bipartito(g);
    log_info("%s: finished bipartito in %.5fs", __func__);
    log_info("%s: bipartito: %s", __func__, res == 1 ? "yes" : "no");

    GraphDestroy(g);
    return 0;
}
