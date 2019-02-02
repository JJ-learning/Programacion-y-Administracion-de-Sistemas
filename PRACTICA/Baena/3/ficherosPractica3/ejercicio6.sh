#!/bin/bash

if [ $# -ne 1 ] || [ ! -d $1 ]
then
	echo -e "Error\n"
else
	directorios=$(find $1 -type d | wc -l) #mostramos los directorios y luego contamos las lineas para saber cuantos hay
	enlaces=$(find $1 -type l | wc -l) #numero de enlaces simbolicos
	fich=0
	
	#Recorro todos los ficheros y aumento el contador
	for x in $(find $1 -type f) #numero de ficheros ejecutables
	do
		if [ -x $x ]
		then
			let fich++ #numero de ficheros ejecutables
		fi
	done
	
	echo Numero de enlaces simbolicos: $enlaces
	echo Numero de directorios: $directorios
	echo Numero de ficheros convencionales ejecutables: $fich
	
	echo -e "*********************** \n"
	
	
	echo "Interpretes en $1/etc/init.d/ es:" #numero y tipo de interpretes
	for x in $(ls $1/etc/init.d/)
	do
		cat $1/etc/init.d/$x | sed -n 's/^#!\ *\([^ ]\{1,\}\).*/\1/p'  #extraigo solo el nombre de interpretes
	
	done | sort | uniq -c | sed -n 's/^[^0-9]\{1,\}\([^ ]\{1,\}\) \(.*\)$/\1 scripts con interprete \2/p'  #ordeno, uniq me sale cada interprete con su numero, doy formato
					#Cojo el número, el espacio y lo siguiente que es el interprete
					#si quito ^[^0-9]\{1,\} estoy cogiendo el espacio que hay delante del número
	echo -e "*********************** \n"
	
	
	for x in $(ls -1 $1/etc/rc?.d)  #leo todos los ficheros de los rc?.d
	do
		echo $x | sed -n 's/[SK][0-9][0-9]\(.*\)/\1/p'  #extraigo solo el nombre del script
	done | sort | uniq >> temp.txt #ordeno, los pongo unicos y guardo en fichero
	
	OLDIFS=$IFS
	IFS=$'\n'
	
	for y in $(cat temp.txt) #recorro fichero con nombres de scripts
	do
		if [ ! -z $y ] #si la linea no esta vacia
		then
			echo -e "--------------\nScript $y"
			for z in $(find $1/etc/rc?.d -name "*$y*") #busco todos los scrips con ese nombre y muestro si son de arranque o prioridad segun el SED, no hace falta if
			do
				#echo $z --> ./etc/rcS.d/S18kbd
				# para saber el NIVEL miramos el caracter que hay despues de rc
				# S --> arranque --> y el numero que hay despues es la prioridad
				# k --> parada --> y el numero que hay despues es la prioridad
				# en el ejemplo puesto arriba seria nivel S, arranque con prioridad 18
				echo $z  | sed -n 's/.*rc\([[:alnum:]]\).*S\([0-9][0-9]\).*/En el nivel \1: Arranque con prioridad \2./p '
				echo $z  | sed -n 's/.*rc\([[:alnum:]]\).*K\([0-9][0-9]\).*/En el nivel \1: Parada con prioridad \2./p '
			done | sort | uniq 
		fi 
	done
	
	IFS=$OLDIFS
	rm temp.txt	#borro archivo temporal
	
	echo -e "\n=============\n"
	
	
	read -p "Introduzca el nivel deseado: " op #leo el nivel
	if [ -d $1/etc/rc$op.d ] #compruebo que existe
	then
		for x in $(ls -1 $1/etc/rc$op.d) #recorro todo el nivel
		do
			echo $x | sed -n 's/K\([0-9][0-9]\)\(.*\)/Detener \2 con prioridad \1/p' 
			echo $x | sed -n 's/S\([0-9][0-9]\)\(.*\)/Iniciar \2 con prioridad \1/p' 
		done | sort -t' ' -k 1,1 -k 5,5n #muestro segun sean k o s, ordenado
	else 
		echo "Ese nivel no existe."
	fi

fi
