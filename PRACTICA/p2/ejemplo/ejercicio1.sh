#!/bin/bash
if [ ! "$#" -eq 1 ];
then
	echo "La ejecución es ejercicio1.sh [Directorio]"
else
	if [ -d "$1" ];
	then
		for x in $(find -name "*.c")
		do
			y=$(wc -l $x)

			echo "Contiene $y "
			
		done
	else
		echo "No existe el directorio escrito"
	fi
fi