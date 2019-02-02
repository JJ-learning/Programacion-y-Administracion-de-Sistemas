#!/bin/bash

directorio="/home/$USER"

echo "Listado de archivos ocultos del directorio $directorio"

temp=$(mktemp)

for fichero in $(ls -a $directorio)
do
nomFich=$(basename $fichero)
fichOclt=$(echo $nomFich | grep '^\.')

if [[ $fichOclt == $nomFich ]]
then
	numLineas=$(echo $fichOclt | wc -c)	#Saca el numero de caracteres
	echo "$numLineas;$fichOclt" >> $temp
fi
done
cat $temp | sort -n -k 1 | sed -n -e 's/\(.*\);\(.*\)/\2/p'

echo "===="
echo -n "Escriba un nombre de fichero: "
read fichero
cat eses | grep '^.' > $fichero.sinLineasVacias
echo "El fichero sin líneas vacías se ha guardado en $fichero.sinLineasVacias"

echo "===="
echo "Listado de los procesos ejecutados por el usuario $USER: "

ps -u $USER -o pid,comm | grep '[0-9]\{1,\}' | sed -n -e 's/\([0-9]\{1,\}\) \(.*\)/PID: \"\1\" Ejecutable: \"\2\"/p'


