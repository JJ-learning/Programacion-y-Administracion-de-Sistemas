#!/bin/bash

function esta(){

	y=$(echo $2 | sha1sum | cut -b -40)
	
	if [ $y == $1 ]
	then
		echo -e "Encontrado!!\nLa pass es $2"
		exit 0
	fi
}

if [ $# -ne 2 ]
then
	echo "Error en el numero de argumentos introducidos"
	exit 1
else
	if [ $2 -eq 1 ]
	then
		echo "Buscando contraseñas de 1 caracter..."
		for x in $(echo {a..z})
		do
			esta $1 $x
		done
		echo "No se ha encontrado la pass"
	fi
	if [ $2 -eq 2 ]
	then
		echo "Buscando contraseñas de 2 caracteres..."
		for x in $(echo {a..z}{a..z})
		do
			esta $1 $x
		done
		echo "No se ha encontrado la pass"
	fi
	if [ $2 -eq 3 ]
	then 
		echo "Buscando contraseñas de 3 caracteres..."
		for x in $(echo {a..z}{a..z}{a..z})
		do
			esta $1 $x
		done
		echo "No se ha encontrado la pass"
	fi
fi
