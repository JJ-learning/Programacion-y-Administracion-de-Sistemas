#!/bin/bash

cat series.txt | sed -r -n -e 's/(^[0-9]+\. .* * )\((.+)\)$/\n\1\n|-> Año de la serie: \2/p;s/^([0-9]+) TEMPORADAS$/\|-> Número de temporadas: \1/p;s/^\* ([A-Z]+) \*$/\|-> Productora de la serie: \1/p;s/SINOPSIS: (.+\.) [Ver mas]*/\|-> Sinopsis: \1/p;s/Ha recibido (.*) puntos/\|-> Número de puntos: \1/p'
