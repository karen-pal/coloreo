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


# Performance
3/4

Octo.txt
42.91user 0.04system 0:43.35elapsed

CBQsv1000_77_150.txt
real	7m6.926s
user	7m4.719s
sys	0m0.291s


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
