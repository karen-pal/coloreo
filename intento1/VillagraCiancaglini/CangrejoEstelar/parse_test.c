#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "GrafoSt2020.h"
#include "log.h"

int main(int argc, char *argv[]) {
    // only emit warnings, not debug info
    log_set_level(LOG_WARN);

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
    if (!ok) {
        GraphDestroy(g);
        fclose(f);
        return 1;
    }

    GraphDestroy(g);
    fclose(f);
    return 0;
}
