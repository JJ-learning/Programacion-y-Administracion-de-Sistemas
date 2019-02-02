#!/bin/bash
clear
chmod u+x .
if [[ ! "$#" -ge 1 ]];
then
	echo "Error en la línea de argumentos, la ejecución sería:"
	echo "ejercicio6.sh [Directorio]"
else
	if [[ -d "$1" ]];
	then
		echo "<h1>Listado de la carpeta $1</h1>" > $1.html #Para limpiar el fichero
		echo "<ul>" >> $1.html
		for x in $(find "$1" -type d)
		do
			if [[ $x != "$1" ]];
			then
				echo "<li><strong>$x</strong></li>" >> $1.html
				echo "<ul>" >> $1.html
				for y in $(find "$x")
				do
					if [[ $y != $x ]];
					then
						if [[ -d $y ]];
						then
							echo "<li><strong>$y</strong></li>" >> $1.html
						else
							echo "<li>$y</li>" >> $1.html 
						fi
					fi									
				done
				echo "</ul>" >> $1.html
			fi
		done
		for z in $(find "$1" -type f)
		do
			echo "<li>$z</li>">>$1.html
		done
		echo "</ul>" >> $1.html			
		echo "Todo realizado correctamente."
	else
		echo "ERROR:"
		echo "El segundo argumento debe de ser un directorio"
	fi
fi