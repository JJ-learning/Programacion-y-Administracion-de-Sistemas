#!/bin/bash
clear
if [ "$#" -lt 0 ];
then
	echo "La ejecución es ejercicio3.sh"
else 
	#Creamos un fichero temporal para guardar los datos
	fichero="who.txt"

	#Leemos el formato del comando who e incluimos el nombre, la fecha y la hora
	who -s | sed -r -n -e 's/^([^ ]*)[ ]+[^ ]*[ ]+([^ ]*)[ ]+([^ ]*).*/\1,\2;\3/p' > $fichero
	#Recorremos el fichero para guardar las variables
	for x in $(cat $fichero) 
	do
		#Utilizamos el comando cut para sacar del fichero los datos deseados
		usuario=$(echo "$x" | cut -d "," -f 1 )
		fecha=$(echo "$x" | cut -d "," -f 2  | sed -r -e 's/([0-9]+)-([0-9]+)-([0-9]+);(.+)/\1\/\2\/\3/')
		fecha=$(date -d $fecha +%s)
		#Utilizamos el let para poder utilizar operaciones matemáticas
		let fecha=$(date +%s)-$fecha
		#Convertimos los datos a horas, minutos y segundos
		let horas=$[$fecha/3600]
		let minutos=$[($fecha/60)%60]
		let segundos=$[$fecha%60]

		echo "El usuario $usuario lleva conectado $horas horas, $minutos minutos y $segundos segundos"
	done
rm $fichero
fi