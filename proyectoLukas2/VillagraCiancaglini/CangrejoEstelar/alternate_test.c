#include <assert.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "base.h"
#include "log.h"
#include "veinteveinte.h"
// Algo que ayude en el testeo (DISCRETA II - FaMAF)
// Si hacen logs del trabajo con los grafos los print pueden venir bien

// Imprimir el nombre de la variable para el control del test
#define NombreDeVariable(variable) #variable

// Comparar el orden interno de los grafos
char CompararOrdenGrafos(Grafo G1, Grafo G2) {
    assert(G1 != NULL);
    assert(G2 != NULL);
    u32 num_vertices_1 = NumeroDeVertices(G1);
    if (num_vertices_1 != NumeroDeVertices(G2) ||
        NumeroDeLados(G1) != NumeroDeLados(G2) || Delta(G1) != Delta(G2)) {
        return 0;
    }
    char ok = 1;
    for (u32 i = 0; i < num_vertices_1 && ok; i++) {
        ok = ok && (Nombre(i, G1) == Nombre(i, G2));
        if (!ok) {
            log_error(
                "%s: FALLA: El nombre de algún vértice en el orden interno no "
                "corresponde al del otro grafo.\n",
                __func__);
            break;
        }
    }
    return ok;
}

// Comparar los grafos
char CompararGrafos(Grafo G1, Grafo G2) {
    assert(G1 != NULL);
    assert(G2 != NULL);
    u32 num_vertices_1 = NumeroDeVertices(G1);
    if (num_vertices_1 != NumeroDeVertices(G2) ||
        NumeroDeLados(G1) != NumeroDeLados(G2) || Delta(G1) != Delta(G2)) {
        return 0;
    }
    char ok = 1;
    u32 grado_i;
    for (u32 i = 0; i < num_vertices_1 && ok; i++) {
        ok = ok && (Nombre(i, G1) == Nombre(i, G2));
        if (!ok) {
            log_error(
                "%s: FALLA: El nombre de algún vértice en el orden interno no "
                "corresponde al del otro grafo.\n",
                __func__);
            break;
        }
        ok = ok && (Color(i, G1) == Color(i, G2));
        if (!ok) {
            log_error(
                "%s: FALLA: El color de algún vértice en el orden interno no "
                "corresponde al del otro grafo.\n",
                __func__);
            break;
        }
        grado_i = Grado(i, G1);
        ok = ok && (grado_i == Grado(i, G2));
        if (!ok) {
            log_error(
                "%s: FALLA: Los grados de algún vértice en el orden interno no "
                "son "
                "iguales! No es válido el test...\n",
                __func__);
            break;
        }

        // Verificamos los vértices vecinos
        for (u32 j = 0; j < grado_i; j++) {
            ok = ok && (NombreVecino(j, i, G1) == NombreVecino(j, i, G2));
            if (!ok) {
                log_error(
                    "%s: FALLA: El nombre de algún vértice vecino no es igual "
                    "al "
                    "correspondiente en el otro grafo.\n",
                    __func__);
                break;
            }
            ok = ok && (ColorVecino(j, i, G1) == ColorVecino(j, i, G2));
            if (!ok) {
                log_error(
                    "%s: FALLA: El color de algún vértice vecino no es igual "
                    "al "
                    "correspondiente en el otro grafo.\n",
                    __func__);
                break;
            }
            ok = ok && (OrdenVecino(j, i, G1) == OrdenVecino(j, i, G2));
            if (!ok) {
                log_error(
                    "%s: FALLA: El orden de algún vértice vecino no es igual "
                    "al "
                    "correspondiente en el otro grafo.\n",
                    __func__);
                break;
            }
        }
    }
    return ok;
}

/* REORDNEAMIENTO */

// Verificar orden Natural en el nombre real de los vértices
char VerificarNatural(Grafo G) {
    char ok = 1;
    u32 numero_vertices = NumeroDeVertices(G);
    u32 nombre_anterior = Nombre(0, G);
    for (u32 i = 1; i < numero_vertices && ok; i++) {
        ok = ok && (nombre_anterior < Nombre(i, G));
        if (!ok)
            log_error("%s: FALLA (%u): %u >= %u\n", __func__, i,
                      nombre_anterior, Nombre(i, G));
        nombre_anterior = Nombre(i, G);
    }
    return ok;
}

// Verificar orden Welsh-Powell
char VerificarWelshPowell(Grafo G) {
    char ok = 1;
    u32 n = NumeroDeVertices(G);
    for (u32 i = 0; i < n - 1; i++) {
        if (Grado(i, G) < Grado(i + 1, G)) {
            log_error("%s: FALLA (%u): %u < %u\n", __func__, i, Grado(i + 1, G),
                      Grado(i, G));
            ok = 0;
            break;
        }
    }
    return ok;
}

// Verificar orden revertido de los bloques de colores, es decir,
// van primero los de mayor color
char VerificarRevierteBC(Grafo G) {
    RevierteBC(G);
    char ok = 1;
    u32 numero_vertices = NumeroDeVertices(G);
    u32 color_anterior = Color(0, G);
    for (u32 i = 1; i < numero_vertices && ok; i++) {
        ok = ok && (color_anterior >= Color(i, G));
        if (!ok)
            log_error("%s: FALLA (%u): %u < %u\n", __func__, i, color_anterior,
                      Color(i, G));
        color_anterior = Color(i, G);
    }
    return ok;
}

// Verificar orden creciente de las cardinalidades de los bloques
// de colores
char VerificarChicoGrandeBC(Grafo G) {
    ChicoGrandeBC(G);
    char ok = 1;
    u32 numero_vertices = NumeroDeVertices(G);
    u32 color_actual = Color(0, G);
    u32 cardinal_bloque_anterior = 0;
    u32 cardinal_bloque_actual = 1;
    for (u32 i = 1; i < numero_vertices && ok; i++) {
        if (Color(i, G) == color_actual) {
            cardinal_bloque_actual++;
        } else {
            ok = ok && (cardinal_bloque_anterior <= cardinal_bloque_actual);
            if (!ok)
                log_error(
                    "%s: FALLA: cardinal_bloque_anterior (%u) > "
                    "cardinal_bloque_actual (%u)\n",
                    __func__, cardinal_bloque_anterior, cardinal_bloque_actual);
            cardinal_bloque_anterior = cardinal_bloque_actual;
            color_actual = Color(i, G);
            cardinal_bloque_actual = 1;
        }
    }
    return ok;
}

// Verifica que bajo la misma semilla den el mismo orden
char VerificarAleatorizarVertices(Grafo G) {
    char ok = 1;
    u32 semilla = (u32)time(NULL);
    Grafo copia_1 = CopiarGrafo(G);
    if (copia_1 == NULL) {
        log_error("Hubo un problema en CopiarGrafo para %s...\n",
                  NombreDeVariable(copia_1));
        return 0;
    }
    Grafo copia_2 = CopiarGrafo(G);
    if (copia_2 == NULL) {
        log_error("Hubo un problema en CopiarGrafo para %s...\n",
                  NombreDeVariable(copia_2));
        DestruccionDelGrafo(copia_1);
        return 0;
    }

    if (!AleatorizarVertices(copia_1, semilla) &&
        !AleatorizarVertices(copia_2, semilla)) {
        if (CompararOrdenGrafos(copia_1, copia_2)) {
            ok = 1;
        } else {
            log_error(
                "%s: FALLA: Los grafos pseudoaleatorizados no son iguales bajo "
                "la "
                "misma semilla...\n",
                __func__);
            ok = 0;
        }
    } else {
        log_error(
            "%s: FALLA: Algo ocurrió cuando se aleatorizaba...\nTest "
            "invalido...\n",
            __func__);
        ok = 0;
    }

    DestruccionDelGrafo(copia_1);
    DestruccionDelGrafo(copia_2);

    return ok;
}

/* COLOREO */

// Chequear coloreo propio
char VerificarColoreoPropio(Grafo G) {
    char ok = 1;
    u32 numero_vertices = NumeroDeVertices(G);
    u32 grado_vertice, color_vertice, nombre_vertice, nombre_vecino;
    for (u32 i = 0; i < numero_vertices && ok; i++) {
        grado_vertice = Grado(i, G);
        color_vertice = Color(i, G);
        nombre_vertice = Nombre(i, G);
        for (u32 j = 0; j < grado_vertice && ok; j++) {
            nombre_vecino = NombreVecino(j, i, G);
            ok = ok && (color_vertice != ColorVecino(j, i, G));
            if (!ok)
                log_error("%s: FALLA color(%u) = %u == %u = color(%u)\n",
                          __func__, nombre_vertice, color_vertice,
                          ColorVecino(j, i, G), nombre_vecino);
        }
    }
    return ok;
}

int main(int argc, char *argv[]) {
    /* log_set_level(LOG_INFO); */

    if (argc < 1) {
        log_warn("%s: expected 1 arguments", __func__);
        return 1;
    }

    FILE *f = fopen(argv[1], "r+");
    if (f == NULL) {
        log_error("%s: opening graph '%s': %s", __func__, argv[1],
                  strerror(errno));
        fclose(f);
        return 1;
    }
    Grafo g = GraphNew();

    log_info("%s: begin parsing '%s'...", __func__, argv[1]);
    bool parsed = GraphParse(g, f);
    fclose(f);
    if (!parsed) {
        log_error("%s: failed to parse '%s', cleaning up", __func__, argv[1]);
        GraphDestroy(g);
        return 1;
    }
    Greedy(g);

    Grafo g2 = CopiarGrafo(g);

    int ntests1 = 2;

    // array of function pointers - ungodly syntax, saves plenty of lines
    char (*tests1[])(Grafo G1, Grafo G2) = {CompararOrdenGrafos,
                                            CompararGrafos};
    for (int i = 0; i < ntests1; ++i) {
        char ok = tests1[i](g, g2);
        if (!ok) {
            GraphDestroy(g);
            return 1;
        }
    }
    GraphDestroy(g2);

    int ntests2 = 4;

    // array of function pointers - moral syntax, saves plenty of lines
    char (*tests2[])(Grafo G) = {VerificarWelshPowell, VerificarRevierteBC,
                                 VerificarChicoGrandeBC,
                                 VerificarAleatorizarVertices};

    for (int i = 0; i < ntests2; ++i) {
        tests2[i](g);
    }

    GraphDestroy(g);
    return 0;
}
