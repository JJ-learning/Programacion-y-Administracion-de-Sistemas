#!/bin/bash

reset

clearLog()
{
	if [ -e ejercicio2.log ]
		then
		rm ejercicio2.log
	fi
	CONTADOREX=0
	CONTADORLIB=0
	CONTADORIMG=0
	CONTADORH=0

}

function check()	#Comprueba si hay un archivo/fichero que ya existe o no para sobreescribir
{	
	namef=$(basename $1)
	if [ -e $2/$namef ]
		then
		echo "El archivo $1 ya existe, ¿Desea sobrescribirla? (S/n)" 
		echo ""
		read op	#CIN
		if [[ $op == "S" ]] || [[ $op == "s" ]] #Sobreescribe
			then
			date >> ejercicio2.log 
			echo "Sobrescribiendo $1" >> ejercicio2.log 
			echo "Sobrescribiendo $1"
			echo ""
			cp $1 $2/$namef
			echo "$1 copiado con exito en $2" >> ejercicio2.log
			echo "$1 copiado con exito en $2"
			echo "" 
			
			return 0 #Copio
			
		else
			echo "No se sobrescribiran los archivos" 
			echo ""
			return 1 #No copio
		fi
	else
		echo "El archivo $1 no existe" 
		echo "" #Salto de linea
		cp $1 $2/$namef
		date >> ejercicio2.log #Poner fecha
		echo "$1 copiado con exito en $2" >> ejercicio2.log
		echo "$1 copiado con exito en $2"
		echo "" 
		return 0 #Copio
	fi
}

comprobar()	#Comprueba carpeta exista o no
{

	if ! [ -e $1 ]
	then
		echo "Carpeta $1 no existe"
		echo ""
		mkdir $1
		date >> ejercicio2.log
		echo "Carpeta $1 creada" >> ejercicio2.log
		echo "Carpeta $1 creada"
		echo ""
	else
		echo "No se ha creado la carpeta $1 porque ya existe"
		echo ""
	fi

}


executables()	#ejecutables
{
	echo "Introduzca la carpeta para los ejecutables"
	read -t5 ex
	ex=${ex:-$HOME/bin} #Pasan 5 segundos y no escribes nada se asigna HOME...
	comprobar $ex
}

libs()	#librerias
{
	echo "Introduzca la carpeta para las librerias"
	read -t5 lib
	lib=${lib:-$HOME/lib} #Pasan 5 segundos y no escribes nada se asigna HOME...
	comprobar $lib
}


pictures()	#imagenes
{
	echo "Introduzca la carpeta para las imagenes"
	read -t5 img
	img=${img:-$HOME/img} #Pasan 5 segundos y no escribes nada se asigna HOME...s
	comprobar $img
}

headers()	#cabeceras
{
	echo "Introduzca la carpeta para las cabeceras"
	read -t5 head
	head=${head:-$HOME/include} #Pasan 5 segundos y no escribes nada se asigna HOME...
	comprobar $head
}

copyandcount()							#copiar y contar 
{
	echo "Los ejecutables de $1 se van a copiar en $ex"
	echo ""
	for x in $(find $1 -executable -type f) 
	do
		
		if check $x $ex						#comprueba si en ex existe x como check devuelve 0 cuando sobrescribe entra en el if
			then
			let CONTADOREX=CONTADOREX+1 			#obliga a que sea tipo numerico suma 1 a la cuenta
		fi
		
	done								#fin del bucle

	echo "Las librerias de $1 se van a copiar en $lib" 
	echo ""
	for x in $(find $1 -type f -name "lib*" ) 			#busca todos los que empizan por lib*
	do
		if check $x $lib
			then
			let CONTADORLIB=CONTADORLIB+1 			#aumenta en uno el contador
		fi
	done

	echo "Las imagenes de $1 se van a copiar en $img"
	echo ""
	for x in $(find $1 -type f -name "*.jpg" -o -name "*.png" -o -name "*.gif") 	#todas las que acaben en .gif .jpg .png
	do
		if check $x $img
			then
			let CONTADORIMG=CONTADORIMG+1 			#aumenta en uno el contador
		fi
	done

	echo "Las cabeceras de $1 se van a copiar en $include"
	echo ""
	for x in $(find $1 -type f -name "*.h") 			#todas las que acaben en .h
	do
		if check $x $include
			then
			let CONTADORH=CONTADORH+1 			#aumenta en uno el contador
		fi
	done
	
	}


pdf()								#para pasar a pdf las imagenes
{
cd $img
	for x in $(find $img)
	do

	nombre=$(basename $x)				#me quedo solo con el nombre del archivo imagen.jpg
	base=${nombre%%.*}				#me quedo solo con el nombre sin terminacion 
	convert $nombre "$base.pdf"			#paso a pdf los archivos 
	done						#fin bucle
	rm *.jpg *.png	*.gif				#borro los que no son pdf
	
}

if [ $# -eq 0 ]
	then
	echo "Solo se va a procesar el directorio actual"
	echo ""
	directorio=./
else
	echo "El numero de directorios que van a ser procesados es $#"
	echo ""


fi

#main
clearLog
executables
libs
pictures
headers
if [[ $# == 0 ]]
then
	copyandcount ./
else
	for x
	do
		copyandcount $x
	done

fi
pdf

echo "Se han copiado $CONTADOREX ejecutables" 
echo ""
echo "Se han copiado $CONTADORIMG imagenes" 
echo ""
echo "Se han copiado $CONTADORLIB librerias"
echo "" 
echo "Se han copiado $CONTADORH cabeceras"
echo ""
echo "$0 finalizado con un tiempo de $SECONDS segundos"
echo ""
