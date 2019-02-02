#!/bin/bash
clear
chmod u+x .
if [ ! "$#" -eq 1 ];
then
	echo "La ejecución es ejercicio1.sh [Directorio]"
else
	if [ -e "$1" ]; #Miramos si existe el directorio
	then
		for x in $(find -name "*.c") #Recorremos todos los fichero buscando los *.c
		do
			y=$(cat $x | wc -l) #Recogemos el numero de lineas del fichero con wc -l

			echo "$x contiene $y lineas"
			
		done
	else
		echo "No existe el directorio escrito"
	fi
fi