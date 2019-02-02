#!/bin/bash
clear
chmod u+x .
if [ ! "$#" -ge 1 ];
then
	echo "La ejecución es ejercicio2.sh [Directorio] [Numero de bytes(Opcional)]"
else
	if [ -e "$1" ]; #Miramos si existe el directorio
	then
		if [[ "$2" > 0 ]];
		then
			echo "Nombre, Longitud, UsuarioNombre,UsuarioID, NumeroEnlaces, FechaCreacion, Permisos, Ejecutable"
			#Introducimos un numero de bytes
			for x in $(find "$1") #Buscamos los ficheros alojados en ese directorio
			do
				y=$(stat -c %s $x) #Recogemos el tamaño del archivo
				if [[ "$y" -ge "$2" ]];
				then
					z=$(basename $x) #Nombre acortado del fichero
					tam=$(expr length $z) #Tamaño del nombre del archivo
					nombreUsuario=$(stat -c %U $x) #Nombre usuario propietario
					uid=$(stat -c %u $x) #ID usuario propietario
					enlaces=$(stat -c %i $x) #Numero de enlaces
					modificacion=$(stat -c %Y $x) #Ultima modificacion
					permiso=$(stat -c %A $x) #Permisos del archivo
					if [ -x "$x" ]; #Miramos si es ejecutable o no
					then 
						ejecutable=1
					else
						ejecutable=0
					fi
					echo "$z, $tam, $nombreUsuario, $uid, $enlaces, $modificacion, $permiso, $ejecutable"
				fi
			done | sort -t ',' -nrk2
		else
			echo "Nombre, Longitud, UsuarioNombre,UsuarioID, NumeroEnlaces, FechaCreacion, Permisos, Ejecutable"
			for x in $(find "$1" ) #Buscamos los ficheros alojados es ese directorio
			do				
				z=$(basename $x) #Nombre acortado del fichero
				tam=$(expr length $z) #Tamaño del nombre del archivo
				nombreUsuario=$(stat -c %U $x) #Nombre usuario propietario
				uid=$(stat -c %u $x) #ID usuario propietario
				enlaces=$(stat -c %i $x) #Numero de enlaces
				modificacion=$(stat -c %Y $x) #Ultima modificacion
				permiso=$(stat -c %A $x) #Permisos del archivo
				if [ -x "$x" ]; #Miramos si es ejecutable o no
				then 
					ejecutable=1
				else
					ejecutable=0
				fi
				echo "$z, $tam, $nombreUsuario, $uid, $enlaces, $modificacion, $permiso, $ejecutable"
				
			done | sort -t ',' -nrk2
		
		fi
	fi
fi