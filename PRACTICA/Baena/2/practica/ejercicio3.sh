#!/bin/bash

if [ $# -ne 2 ]
then
	echo "Error en el numero de argumentos introducidos"
	exit 1
else
	carpeta=$2
	if [ ${carpeta:(-1)} != / ]
	then
		carpeta=$2/
	fi
	actual=$(pwd)
	cd $1
	find -type d | while read x
	do
		cd $actual
		mayus=$(echo $x | tr 'a-z' 'A-Z')
		mkdir -p $carpeta/$mayus
	done
	cd $actual
	find $1 -type f | while read x 
	do
		cad=${x#$1} 
		cad2=$(echo $cad | tr 'a-z' 'A-Z')
		cp $x $carpeta$cad2
	done
fi
