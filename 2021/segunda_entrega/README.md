# Para correr

```
gcc -Wall -Wextra -O3 -std=c99 -DNDEBUG main.c grafo.c RomaVictor.c -o a.out
./a.out < graph_file.corr
```

## Con GDB
```
gcc -Wall -Wextra -std=c99 -DNDEBUG -g main.c grafo.c RomaVictor.c -o a.out
gdb ./a.out
gdb $ run < caso5.corr
```

## con valgrind
```
valgrind --leak-check=full          --show-leak-kinds=all          --track-origins=yes          --verbose          --log-file=valgrind-out_tarda.txt          ./a.out < BxB1100_999_54_2017.corr 
```

# Performance
3/4

Octo.txt
antes: 42.91user 0.04system 0:43.35elapsed
despues:
real	0m2.048s
user	0m1.925s
sys	0m0.092s

CBQsv1000_77_150.txt
antes:
real	7m6.926s
user	7m4.719s
sys	0m0.291s
despues:
real	0m2.684s
user	0m2.523s
sys	0m0.128s



# ej construc grafo
p edge 6 3
e 0 2
e 1 3
e 4 6


a = [0,1,4]
b = [2,3,6]

[0,1,4,2,3,6]
[2,3,6,0,1,4]


[0,1,2,3,4,6]
[2,3,0,1,6,4]


## Errores encontrados
* error corriendo bipartito en https://www.famaf.unc.edu.ar/~penazzi/Discreta2018/GRD22154753987.tar.gz
