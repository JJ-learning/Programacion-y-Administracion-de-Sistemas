#!/bin/bash
echo "***********************************"
echo "1)Duración de la pelicula:"
cat peliculas.txt | grep -E -o '^[0-9]* min.'
echo "***********************************"
echo "2)Pais de la pelicula:"
cat peliculas.txt | grep -E '\[[A-Z].*\]'
echo "***********************************"
echo "3)Pais de la pelicula(SIN LINEA):"
cat peliculas.txt | grep -E -o '\[[A-Z].*\]'
echo "***********************************"
echo "4)Pelis 2014/2015:"
cont2014=0
cont2015=0
for x in `cat peliculas.txt | grep -E -o '\([0-9][0-9][0-9][0-9]\)'`
do
	if [ $x == "(2014)" ]
	then
		cont2014=$[cont2014+1]
	fi
	if [ $x == "(2015)" ]
	then
		cont2015=$[cont2015+1]
	fi
done
echo "En 2014 se hicieron $cont2014 peliculas"
echo "En 2015 se hicieron $cont2015 peliculas"
echo "***********************************"
echo "5)Sin lineas vacias:"
cat peliculas.txt | grep -E -v '^ *$'
echo "***********************************"
echo "6)Comienzan por C:"
cat peliculas.txt | grep -E '^ *C'
echo "***********************************"
echo "7)Lineas que contengan d(vocal)d l(vocal)l t(vocal)t:"
cat peliculas.txt | grep -E '([dlt])[aeiou]\1'
echo "***********************************"
echo "8)8 o mas aes:"
cat peliculas.txt | grep -E '(.*[aA].*){8,1000}'
echo "***********************************"
echo "9)Que terminan por ... y no empiezan por espacio:"
cat peliculas.txt | grep -i -E '^[^ ].*(\.\.\.)$'
echo "***********************************"
echo "10)Vocales con tildes:"
cat peliculas.txt | sed -r -n -e 's/([áÁéÉíÍóÓúÚ])/"\1"/p'
