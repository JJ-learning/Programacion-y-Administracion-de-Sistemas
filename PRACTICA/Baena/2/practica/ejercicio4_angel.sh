#!/bin/bash

#GPG es un software híbrido de cifrado que combina tanto criptografía simétrica o de clave privada y criptografía asimétrica o de clave pública. 

#En primer lugar comprobamos el número de argumentos
if [ $# -ne 1 ];
then
	echo "ERROR: número de arguntos incorrecto"
else
	#Aquí realizaremos el ejercicio
	#Primero comprobamos si es un directorio o un fichero
	if [ -d $1 ];
	then
		#Comprimimos la carpeta a tar.gz
		tar -zcf $1.tar.gz $1

		#Ciframos el fichero comprimido
		gpg -c $1.tar.gz
	fi	
	if [ -f $1 ];
	then
		gpg -c $1
	fi

fi
