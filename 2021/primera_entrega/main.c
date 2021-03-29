def GraphParse(graph_file){
    content = load(graph_file)
    let length = 0
    let count = 0;
    for line in content:
        if type(line) == COMMENT:
            skip
        else if type(line) == CONFIG //p edge n m
            length = loadInfo(graph, vertex_amount = n, edge_amout = m)
        else if type(line) == EDGE:
            if length == 0
                return null
            if count > length
                return null
            loadNeigh(edges(line))
            count++;
        else if type(line) == EMPTY:
            skip
        else
            return NULL
}
