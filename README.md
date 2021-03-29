# Correcciones
lun, 6 jul 2020 22:45

1) Tienen un error con ChicoGrandeBC

Si ordenamos con ChicoGrandeBC, y luego imprimimos el color de cada vértice en el orden que queda,
agrupando vértices del mismo color e indicando cuantos son con ese color,
obtenemos salidas como:

0(8),1(7),2(8),3(9),4(8),5(9),6(7),7(6),8(7),9(8),10(7),11(8),12(3),13(5)

que significa que hay primero 8 vertices de color 0, luego 7 de color 1, etc.

Esta claro que están ordenando POR COLOR y no por CANTIDAD DE VÉRTICES CON ESE COLOR,
que es lo que se pide que haga ChicoGrande.

(en este caso deberian haber estado los de color 12 primero, luego los de 13, luego los de color 7, etc)

Al menos, ordenan por bloque de colores, eso si esta bien.

2) Bipartito no da el valor de retorno que se pide.
Con las otras funciones que piden retornar un char 0 o 1, uds retornan correctamente 0 o 1, pero aca retornan '0' o '1' (o incluso '2', que no se pedia)
asi que deduzco que distintas personas programaron las distintas funciones.


3) MAYOR PROBLEMA: Demoran muchísimo con grafos grandes.
Con
https://www.famaf.unc.edu.ar/~penazzi/DisII2016/BxB1100_999_54_2017.tar.gz
lo detuve cuando no habían hecho ni 3 Greedys e iban

78m15.320s

así que no se cuanto van a tardar en hacer los 1000 Greedys requeridos.

Con
https://www.famaf.unc.edu.ar/~penazzi/DisII2016/R1999999_10123123_1.gz
El error está en
for (u32 j = 0; j < Grado(i,G); j++) {
            if (OrdenVecino(j, i, G) < i) {
                if (ColorVecino(j, i, G) == color) {
                    color++;
                    j = -1;
                }
            }
        }

que  no es una forma adecuada de programar Greedy.

Proyecto desaprobado. Los errores 1 y 2 no son para desaprobar, el 3) si.


