#!/bin/bash
clear
if [ "$#" -lt 1 ];
then
	echo "La ejecución es ejercicio1.sh [peliculas.txt]"
else
	echo "1)*****************************************" #Buscamos las líneas que identifican la duración
	cat $1 | grep -E '[1-9]\h.[0-9][1-9]\min'
	echo ""
	echo "2)*****************************************"
	cat $1 | grep -E '\['
	echo ""
	echo "3)*****************************************" #Con el -o hacemos que sólo se muestre el caracter que empareja
	cat $1 | grep -E -o '\[.*'
	echo ""
	echo "4)*****************************************" #Contamos el núemro de peliculas del 2017/16
	echo "Hay $(cat $1 | grep -E -c '[0-9][0-9][0-9][7]') peliculas del 2017 y $(cat $1 | grep -E -c '[0-9][0-9][0-9][6]') del 2016"
	echo ""
	echo "5)*****************************************"
	grep -E -v '^$' $1 #No mostramos las líneas vacías
	echo ""
	echo "6)*****************************************" #Buscamos las que comiencen con un espacio y con E o L
	grep -E '^(..[E|L]|E|L)' $1 #No muestra las que empiezan con espacio L
	echo ""
	echo "7)*****************************************"
	grep -E '(d[aeiou]d|t[aeiou]t|l[aeiou]l)' $1
	echo ""
	echo "8)*****************************************" #Buscamos por las que tengan más de 8 A/a
	grep -E -i '(.*a.*){8,100}' $1 #El -i es para no diferenciar entre mayúsculas o minúsculas
	echo ""
	echo "9)*****************************************"
	grep -E '^[^ ].*\.{3}$' $1 #\s es el espacio
	echo ""
	echo "10)*****************************************"
	sed -r -e 's/([áéíóú])/"\1"/g' $1
fi
