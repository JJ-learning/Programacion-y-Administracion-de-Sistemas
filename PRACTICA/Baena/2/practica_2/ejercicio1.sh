#!/bin/bash

echo "Carpeta;Nombre;Tamaño;ReferenciasInodo;Permisos;Ejecutable"
if [ $# -eq 0 ] || [ $# -ge 3 ];
	then
		echo "Error en la linea de errores"
else
	if [ $# -eq 1 ];
	then
		tam=0
	else
		tam=$2
	fi
		find $1 -size +$tam | while read x
		do
			if [ -x $x ];
			then
				ejecutable=1
			else 
				ejecutable=0
			fi
			
			directorio=$(dirname $x)
			nombre=$(basename $x)
			base=${nombre%%.*}
			tamanio=$(stat -c "%s" $x)
			acceso=$(stat -c "%A" $x)
			hora=$(stat -c "%x" $x)
			nodo=$(stat -c "%i" $x)
		
			echo "$directorio;$nombre;$base;$tamanio;$nodo;$acceso;$hora;$ejecutable"
		done	|sort -t ";" -k 2 -r
fi
