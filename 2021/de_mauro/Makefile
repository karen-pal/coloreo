gcc_flags=-Wall -Wextra -O3 -std=c99
test_flags=-Wall -Wextra -O3 -std=c99 -Werror -Wno-error=unused-function

# --- CARGA DE GRAFOS

# Esto compila el RomaVictor. Si no anda entonces hace falta hacer `mkdir lib` antes.

roma_victor:
	gcc $(gcc_flags) carga-de-grafos/index.c carga-de-grafos/requeridas.c -fPIC -shared -o lib/RomaVictor.o
	# Se creó la librería `lib/RomaVictor.o`
	#
# --- Ejemplo

# Esto es un ejemplo de cómo uso el RomaVictor compilado.
# Si no funciona hay que hacer `mkdir bin` antes.
# También hay que tener un archivo `grafos/KC` para probar.

ejemplo:
	gcc $(gcc_flags) src/ejemplo.c $(gcc_flags) lib/RomaVictor.o -o bin/ejemplo
	valgrind --leak-check=full bin/ejemplo < grafos/KC

# --- Para testear otros archivos de la parte 1 (ya no sirve)

array:
	gcc carga-de-grafos/Array/array_tests.c -o bin/array $(test_flags)
	valgrind --leak-check=full bin/array

string:
	gcc carga-de-grafos/String/string_tests.c -o bin/string $(test_flags)
	valgrind --leak-check=full bin/string

string_array:
	gcc carga-de-grafos/StringArray/string_array_tests.c -o bin/string_array $(test_flags)
	valgrind --leak-check=full bin/string_array

dimacs:
	gcc carga-de-grafos/Dimacs/dimacs_tests.c -o bin/dimacs $(test_flags)
	valgrind --leak-check=full bin/dimacs

line:
	gcc carga-de-grafos/Line/line_tests.c -o bin/line $(test_flags)
	valgrind --leak-check=full bin/line

nombres:
	gcc carga-de-grafos/Nombres/nombres_tests.c -o bin/nombres $(test_flags)
	valgrind --leak-check=full bin/nombres

# Tests de grafos pequeños (en los que es viable usar valgrind)
grafo:
	gcc carga-de-grafos/Grafo/grafo_tests.c -o bin/grafo $(test_flags)
	valgrind --leak-check=full bin/grafo

# Benchmark para usar en grafos grandes
benchmark:
	gcc carga-de-grafos/Grafo/grafo_benchmark.c -o bin/benchmark $(test_flags)
	# Se creó el ejecutable `bin/benchmark`

# Usar con variable de entorno GRAFO. Ejemplo:
# GRAFO=grafos/bxb15_22_10.col make carga_de_grafo
carga_de_grafo:
	gcc carga-de-grafos/roma_victor_tests.c -g -o bin/roma_victor $(gcc_flags) lib/RomaVictor.o
	bin/roma_victor < grafos/ej10.corr
main1:
	gcc carga-de-grafos/main1.c -o bin/main1 $(gcc_flags) lib/RomaVictor.o
	bin/main1 < grafos/ej10.corr
main2:
	gcc carga-de-grafos/main2.c -o bin/main2 $(gcc_flags) lib/RomaVictor.o
	bin/main2 < grafos/ej10.corr
clean:
	rm bin/* lib/*
