import os

# probamos greedy:
# Los siguientes grafos muestran variabilidad en los
# coloreos de Greedy con los reordenamientos.
grafos_variables_greedy = [
    "rel_chico.corr",
    "queen13.corr",
    "queen10.corr",
    "school1.corr",
    "greedy_42.corr",
    "greedy_77.txt",
]
grafos_bipartitos = [
    "bipartito_6.corr",
    "b_5576.corr",
    "b_20002.corr",
    "b_20482.corr",
    "b_millon.corr",
]
grafos_memoria = [
    "mem_3080.corr",
    "mem_2210104.corr",
    "mem_importante.corr",
    "mem_221561.corr",
    "mem_23658.corr",
    "mem_grande.corr",
]


def probar_implementacion(graph_file):
    print("started calculating {}".format(graph_file))
    cmd = "./grafo < {} > {}".format(graph_file, "reordenes.md")
    # si es de a muchos,
    # ponele aca algo como
    # "test"+graph_file+".md" porque esto sobreescribe
    os.system(cmd)
    print("finished calculating {}".format(graph_file))


# descomenta lo que quieras hacer
# probar con uno solo
probar_implementacion("greedy_42.corr")

# probar con alguna lista de grafos
# list(map(probar_implementacion, grafos_variables_greedy))
