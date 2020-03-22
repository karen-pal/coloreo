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

def existe_vecino(nodo, color):
    for vec in nodo["vecinos"]:
        if vec in color:
            return True
    return False

def greedy(g):
    idx = g["orden"][0]
    nodo = g["nodos"][idx]
    colores = []
    #color = a["color"]
    #nodo["color"]=0
    #print(nodo)
    print("nodo antes: ")
    print(g)
    primer_nodo = g["nodos"][0]
    g["nodos"][0]["color"]=0
    colores.append([primer_nodo["nombre"]])
    colores.append([])
    for i in nodo["vecinos"]:
        g["nodos"][i]["color"]=1
        colores[1].append(g["nodos"][i]["nombre"])

    for i in range(g["cant_vert"]):
        idx = g["orden"][i]
        nodo = g["nodos"][idx]
        if nodo not in colores[0] and not existe_vecino(nodo, colores[0]):
            nodo["color"]=0
            colores[0].append(nodo["nombre"])
        print(nodo["color"])
    print(colores)
    print("grafo despues :")
    print(g)



greedy(grafo_ejemplo)
