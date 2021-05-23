valgrind --tool=callgrind bin/ejec 1 1 1 1 1 1 < grafos/BxB1100_999_54_2017.corr
callgrind_annotate callgrind.out.8923 

gprof2dot --format=callgrind --output=out.dot ./callgrind.out.8923 
dot -Tpng out.dot -o graph.png

