#!/bin/bash

#Para mostrar ficheros ocultos ls -a
ls -a $HOME| grep '^\.' | awk '{ print length(), $0 | "sort -n" }' #Usar opcion para ordenarlos por tamaño
if [ $# -ne 1 ]
then
	echo "Error en el numero de argumentos introducidos"
	exit 1
else
	nombre=$1
	cat $nombre | grep -E -v '^ *$'>>$nombre.sinlineasvacias
fi

echo "Listado de los procesos ejecutados por el usuario $USER: "

ps -u $USER | sed -n -e 's/\([0-9]\{1,\}\)[ ]*.*[ ]*\([0-9][0-9]:[0-9][0-9]:[0-9][0-9]\)[ ]*\(.*\)/PID: \"\1\" Hora: \"\2\" Ejecutable: \"\3\"/p'
