#!/bin/bash
echo -n "Introduzca un numero: "
read respuesta
echo -n "Introduzca otro numero: "
read respuesta2
echo "SUMA:$(($respuesta + $respuesta2))"
echo "RESTA:$(($respuesta - $respuesta2))"
echo "MULTIPLICACION:$(($respuesta * $respuesta2))"
echo "DIVISION:$(($respuesta / $respuesta2))"
echo "MODULO:$(($respuesta % $respuesta2))"
