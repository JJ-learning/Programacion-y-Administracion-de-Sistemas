#!/bin/bash
temp="fichtemp.txt"
#Se guarda en el fichero el nombre de cada usuario
#Formato de conectados
who -s | sed -r -n -e 's/^([^ ]*)[ ]+[^ ]*[ ]+([^ ]*)[ ]+([^ ]*).*/\1,\2;\3/p' > $temp
for x in $(cat $temp)
do	
	#Guardar en una variable desde la conexion del usuario
	user=$(echo "$x" | cut -d "," -f 1)
	fecha=$(echo "$x" | cut -d "," -f 2 | sed -r -e 's/(.+);(.+)/\1 \2/')
	fecha=$[`date -d "$fecha" +%s`]
	fecha=$[`date +%s`-$fecha]
	#Conversion al formato preestablecido
	hours=$[$fecha/3600]
	minus=$[($fecha/60)%60]
	seg=$[$fecha%60]
	echo "El user $user lleva $hours hours, $minus minus y $seg seg conectado"
done
rm $temp	#Eliminacion del fichero temporal
