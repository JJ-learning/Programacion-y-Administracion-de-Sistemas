#!/bin/bash
IFS=$'\n'
clear
if [[ "$#" -lt 1 || "$#" -gt 2 ]];
then
	echo "La ejecución es ./ejercicio6.sh [0-1]"
else
	fichero="/etc/passwd"
	if [[ $1 -eq 0 ]];
	then
		for x in $(cat $fichero)
		do
			# cut -elemento -d "separador"
			usuario=$(echo $x | cut -f 1 -d ":")
			uid=$(echo $x | cut -f 3 -d ":")
			gid=$(echo $x | cut -f 4 -d ":")
			gecos=$(echo $x | cut -f 5 -d ":")
			home=$(echo $x | cut -f 6 -d ":")
			shell=$(echo $x | cut -f 7 -d ":")

			echo "*******************************"
			echo "Logname: $usuario"
			echo "|->UID: $uid"
			echo "|->GID: $gid"
			echo "|->gecos: $gecos"
			echo "|->Home: $home"
			echo "|->Shell: $shell"
		done
	elif [[ $1 == 1 ]];
	then
		for x in $(cat $fichero)
		do
			# cut -f elemento -d "separador"
			
			uid=$(echo $x | cut -f3 -d ":")
			
			if [[ $uid -ge 1000 ]];
			then
				usuario=$(echo $x | cut -f1 -d ":")
				gid=$(echo $x | cut -f4 -d ":")
				gecos=$(echo $x | cut -f5 -d ":")
				home=$(echo $x | cut -f6 -d ":")
				shell=$(echo $x | cut -f7 -d ":")
				echo "*******************************"
				echo "Logname: $usuario"
				echo "|->UID: $uid"
				echo "|->GID: $gid"
				echo "|->gecos: $gecos"
				echo "|->Home: $home"
				echo "|->Shell: $shell"			
			fi			
		done
	fi
fi