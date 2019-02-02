#!/bin/bash
fich="peliculas.txt"
if [ ! -e $fich ]		
then
	echo -e "\nPara ejecutar el script debe de encontrarse en la misma carpeta que el .txt "
	exit 1
fi
tempo="fichTemp.txt"
#Guarda el sichero ya modificado
cat $fich | sed -r -e 's/(.{50,50}).*/\1\.\.\./' > $tempo
#Se cambia el estilo y se imprime lo deseado
cat $tempo | sed -r -e 's/\((.*)\) \[(.*)\]/\1, \2/'
rm $tempo
