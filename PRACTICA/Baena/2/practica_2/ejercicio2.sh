#!/bin/bash

if [ $# -lt 1 ] || [ $# -gt 3 ]
then
	echo "Error en el numero de argumentos introducidos"
	exit 1
elif [ $# -eq 1 ]
then
	umbral1=10000
	umbral2=100000
else
	umbral1=$2
	umbral2=$3
fi

if [ -e $1/pequenos ]
then
	rm -d -r $1/pequenos
	mkdir $1/pequenos
else
	mkdir $1/pequenos
fi

if [ -e $1/medianos ]
then
	rm -d -r $1/medianos
	mkdir $1/medianos
else
	mkdir $1/medianos
fi

if [ -e $1/grandes ]
then
	rm -d -r $1/grandes
	mkdir $1/grandes
else
	mkdir $1/grandes
fi

find $1 | while read x
do

	if [ -f $x ]
	then
		tam=$(stat -c "%s" $x)
		if [ $tam -lt $umbral1 ]
		then
			cp $x $1/pequenos
		fi
		if [ $tam -gt $umbral1 ] && [ $tam -lt $umbral2 ]
		then
			cp $x $1/medianos
		fi
		if [ $tam -gt $umbral2 ]
		then
			cp $x $1/grandes
		fi
	fi

done
