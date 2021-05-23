gcc_flags=-Wall -Wextra -O3 -std=c99 -g -fno-omit-frame-pointer
test_flags=-Wall -Wextra -O3 -std=c99 -Werror -Wno-error=unused-function

# --- CARGA DE GRAFOS

# Esto compila el RomaVictor. Si no anda entonces hace falta hacer `mkdir lib` antes.

roma_victor:
	gcc $(gcc_flags) carga-de-grafos/index.c carga-de-grafos/requeridas.c -fPIC -shared -o lib/RomaVictor.o
	# Se creó la librería `lib/RomaVictor.o`

ejec:
	gcc carga-de-grafos/main_posta.c -o bin/ejec $(gcc_flags) lib/RomaVictor.o
	# bin/ejec 8 5 47 7 3 1 < grafos/Octo.corr
clean:
	rm bin/* lib/*
