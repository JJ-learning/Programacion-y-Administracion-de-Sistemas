#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Error en el numero de argumentos introducidos"
	exit 1
else
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
fi
