nodos_ejemplo = [{"vecinos": [1,2], "nombre":0, "grado":2, "color":"nada"},
        {"vecinos": [2,0], "nombre":1, "grado":2, "color":"nada"},
        {"vecinos": [1,0], "nombre":2, "grado":2, "color":"nada"}
        ]

grafo_ejemplo = {"cant_vert" : 3, "cant_lados" : 3, "delta":2,"nodos":nodos_ejemplo, "orden":[0,1,2]}


def greedy(g):
    idx = g["orden"][0]
    nodo = g["nodos"][idx]
    #color = a["color"]
    #nodo["color"]=0
    #print(nodo)
    g["nodos"][0]["color"]=0
    for i in range(len(nodo["vecinos"])):
        g["nodos"][i]["color"]=1
    """
    for i in range(g["cant_vert"]):
        idx = g["orden"][i]
        nodo = g["nodos"][idx]

        print(nodo["color"])
    """
    print(g)




greedy(grafo_ejemplo)
