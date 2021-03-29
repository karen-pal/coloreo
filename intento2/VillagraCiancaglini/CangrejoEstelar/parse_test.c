#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GrafoSt2020.h"
#include "log.h"

int main(int argc, char *argv[]) {
    // only emit warnings, not debug info
    // log_set_level(LOG_WARN);

    if (argc < 1) {
        log_warn("%s: expected 1 argument, got none", __func__);
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
    bool ok = GraphParse(g, f);
    fclose(f);
    if (!ok) {
        GraphDestroy(g);
        return 1;
    }

    u32 node1;
    u32 node2;
    for (u32 i = 0; i < g->V->length - 1; ++i) {
        node1 = g->V->contents[i];
        node2 = g->V->contents[i + 1];
        if (node1 > node2) {
            log_error("%s: didn't sort nodes properly: at pos #%u, %u > %u",
                      __func__, i, node1, node2);
            char *s = arrayString(g->V, g->V->length);
            log_error("%s: names: %s", __func__, s);
            free(s);
            GraphDestroy(g);
            return 1;
        }
        if (node1 == node2) {
            log_error("%s: didn't sort nodes properly: %u = %u", __func__,
                      node1, node2);
            GraphDestroy(g);
            return 1;
        }
    }

    GraphDestroy(g);
    return 0;
}
