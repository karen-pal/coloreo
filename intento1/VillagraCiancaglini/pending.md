## 3. Funciones de coloreo (entrega requerida)
- [x] u32 Greedy(Grafo G);
- [x] char Bipartito(Grafo G);
- [x] char SwitchColores(Grafo G, u32 i, u32 j);

## 4. Funciones de ordenación (entrega requerida)
- [x] char WelshPowell(Grafo G);
- [x] char RevierteBC(Grafo G);
- [x] char ChicoGrandeBC(Grafo G);
- [x] char AleatorizarVertices(Grafo G, u32 R);

## 5. Funciones de cálculo de número de componentes conexas (entrega requerida)
- [x] u32 NumCCs(Grafo G);

## 6. Funciones de construcción/destrucción/copia del grafo
- [x] Grafo ConstruccionDelGrafo();
- [x] void DestruccionDelGrafo(Grafo G);
- [x] Grafo CopiarGrafo(Grafo G);

## 7. Funciones para extraer información de datos del grafo
- [x] u32 NumeroDeVertices(Grafo G);
- [x] u32 NumeroDeLados(Grafo G);
- [x] u32 Delta(Grafo G);

## 8. Funciones para extraer información de los vértices
- [x] u32 Nombre(u32 i, Grafo G);
- [x] u32 Color(u32 i, Grafo G);
- [x] u32 Grado(u32 i, Grafo G);

## 9. Funciones para extraer información de los vecinos de un vértice
- [x] u32 ColorVecino(u32 j, u32 i, Grafo G);
- [x] u32 NombreVecino(u32 j, u32 i, Grafo G);
- [x] u32 OrdenVecino(u32 j, u32 i, Grafo G);

## 10. Funciones para modificar los datos de los vértices
- [x] char FijarColor(u32 x, u32 i, Grafo G);
- [x] char FijarOrden(u32 i, Grafo G, u32 N);
