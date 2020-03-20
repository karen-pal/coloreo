nodos_ejemplo = [{"vecinos": [1], "nombre":0, "grado":1, "color":"nada"},
        {"vecinos": [0], "nombre":1, "grado":1, "color":"nada"},
        {"vecinos": [3], "nombre":2, "grado":1, "color":"nada"},
        {"vecinos": [2], "nombre":3, "grado":1, "color":"nada"}
        ]

grafo_ejemplo = {"cant_vert" : 4, "cant_lados" : 2, "delta":1,"nodos":nodos_ejemplo, "orden":[0,1,2,3]}

"""
es el grafo
0°---°1

2°---°3

"""

def greedy(g):
    idx = g["orden"][0]
    nodo = g["nodos"][idx]
    #color = a["color"]
    #nodo["color"]=0
    #print(nodo)
    g["nodos"][0]["color"]=0
    for i in nodo["vecinos"]:
        g["nodos"][i]["color"]=1

    for i in range(g["cant_vert"]):
        idx = g["orden"][i]
        nodo = g["nodos"][idx]

        print(nodo["color"])




greedy(grafo_ejemplo)
