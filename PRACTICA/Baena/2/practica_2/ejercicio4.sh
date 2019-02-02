#!/bin/bash

if [ $# -lt 1 ]||[ $# -gt 3 ]
then
	echo "Error en el numero de argumentos introducidos"
	exit 1
elif [ $# -eq 1 ]
then
	if [ -f $1 ]
	then
		gpg --symmetric $1
	fi
	if [ -d $1 ]
	then
		echo "Comprimimos carpeta"
		tar -zcf $1.tar.gz $1
		gpg --symmetric $1.tar.gz #c==symmetric
	fi
elif [ $# -eq 2 ]
then
	nombre=$2
	if [ -f $2 ]
	then
		gpg --symmetric $2
	fi
	if [ -d $2 ]
	then
		echo "Comprimimos carpeta"
		tar -zcf $2.tar.gz $2
		gpg --symmetric $2.tar.gz #c==symmetric
	fi
else #EL TERCER APARTADO NO SE
	numbytes=$3
	if [ -f $1 ]
	then
	find $1 -size +$3 | while read x
	do
		gpg --symmetric $1
	done
	fi
	if [ -d $1 ]
	then
		cd $1
		if [ -f $1 ]
		then
		find $1 -size +$3 | while read x
		do
			gpg --symmetric $1
		done
		fi
		echo "Comprimimos carpeta"
		tar -zcf $1.tar.gz $1		
		gpg --symmetric $1.tar.gz #c==symmetric		
	fi	
fi
