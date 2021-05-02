
Instrucciones para compilar RomaVictor y usarlo:

Primero, crear la carpeta "grafos" y descargar el KC porque ese es el que uso de ejemplo.

```
mkdir lib
mkdir bin
make roma_victor
make ejemplo
```

En el Makefile se encuentra qué hacen los últimos comandos.

Tiene que existir el archivo `grafos/KC` para que el `make ejemplo` funcione.
