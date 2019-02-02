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
		echo $cad 
		cad2=$(echo $cad | tr 'a-z' 'A-Z')
		echo $cad2
		nombre=$(basename $x)
		echo $nombre
		base=${nombre%%.*}
		echo $base
		base2=$(echo $base | tr 'a-z' 'A-Z')
		echo $base2
		cp $base $carpeta$base2
	done
fi
