#!/bin/bash
echo "***********************************"
echo "1)Titulos de las series:"
cat series.txt | grep -E '^[0-9]*\. .*'
echo "***********************************"
echo "2)Cadenas que producen las series:"
cat series.txt | grep -E '.*\* [A-Z]* \*.*'
echo "***********************************"
echo "3)Cadenas que producen las series sin asteriscos ni espacios:"
cat series.txt | grep -E -o '[A-Z][A-Z][A-Z]* '
echo "***********************************"
echo "4)Eliminar lineas de sinopsis:"
cat series.txt | grep -E -v '^SINOPSIS:.*'
echo "***********************************"
echo "5)Eliminar lineas vacias:"
cat series.txt | grep -E -v '^ *$'	#Posibilidad sin -v '..*'
echo "***********************************"
echo "6)Contar cuantas series produce cada cadena:"
contFOX=0
contCBS=0
contAMC=0
contABC=0
contHBO=0
contNBC=0
contWB=0
for x in `cat series.txt | grep -E -o '[A-Z][A-Z][A-Z]* '`
do
	if [ $x == "ABC" ]
	then
		contABC=$[contABC+1]
	fi
	if [ $x == "AMC" ]
	then
		contAMC=$[contAMC+1]
	fi
	if [ $x == "CBS" ]
	then
		contCBS=$[contCBS+1]
	fi
	if [ $x == "FOX" ]
	then
		contFOX=$[contFOX+1]
	fi
	if [ $x == "HBO" ]
	then
		contHBO=$[contHBO+1]
	fi
	if [ $x == "NBC" ]
	then
		contNBC=$[contNBC+1]
	fi
	if [ $x == "WB" ]
	then
		contWB=$[contWB+1]
	fi
done
echo "La cadena ABC produce $contABC series:"
echo "La cadena AMC produce $contAMC series:"
echo "La cadena CBS produce $contCBS series"
echo "La cadena FOX produce $contFOX series"
echo "La cadena HBO produce $contHBO series"
echo "La cadena NBC produce $contNBC series"
echo "La cadena WB produce $contWB series"
echo "***********************************"
echo "7)Lineas que contengan una palabra en mayuscula entre parentesis:"
cat series.txt | grep -E '\([A-Z].*\)'
echo "***********************************"
echo "8)Emparejamientos de palabras repetidas en la misma linea:"
cat series.txt | grep -E '([a-Z][a-Z][a-Z]* ){2,3}'
echo "***********************************"
echo "9)Lineas que contienen 28 aes o mas:"
cat series.txt | grep -E '(.*[aA].*){28,1000}' 
echo "***********************************"
echo "10)Nombre de serie y temporadas:"
cat series.txt | grep -E -v '^ *$' | grep -E -C 1 '^=+$' #-C coge la linea de arriba y abajo
echo "***********************************"
