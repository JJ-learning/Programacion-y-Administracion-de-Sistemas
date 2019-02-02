#!/bin/bash
clear
if [[ "$#" -lt 2 || "$#" -ge 3 ]];
then
	echo "La ejecución es ejercicio4.sh [IPs.txt] [nº segundos]"
else 
	#Recogemos las IPs para hacerles un ping
	for IP in $(cat $1)
	do
		#Utilizamos la opcion -c para contar el numero de paqutes y la opcion -w para poner un tiempo límite de respuesta
		tiempo=$(ping -c 1 -w $2 $IP | sed -r -n -e 's/.*time=(.*) ms$/\1/p')
		
		if [ -z "$tiempo" ];
		then
			echo "La direccion $IP no respondió tras $2 segundos."
		else
			echo "La direccion $IP respondió en $tiempo segundos."
		fi
	done | sort -k6 #Ordenamos según el tiempo que ha tardado
fi