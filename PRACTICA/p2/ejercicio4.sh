#!/bin/bash
clear
chmod u+x .
if [ "$#" -lt 1 ];
then
	echo "La ejecución es ejercicio4.sh [ficheros]"
else
	touch aux
	x=$(find -name "aux")
	tiempo=$(stat -c %Y $x)
	tar -zcf copia-$USER-$tiempo.tar.gz $@
	rm $x

	if [ ! -e Copia ];
	then
		mkdir Copia
		for y in $(find -name "*-$tiempo.tar.gz")
		do
			cp $y Copia
			rm -d -r $y
		done
	else
		for y in $(find -name "*-$tiempo.tar.gz")
		do
			cp $y Copia
			rm -d -r $y
		done
	fi
	echo "Copia de seguridad creada"
	cd Copia
	for y in $(find -name "*.tar.gz")
	do
		tiempo2=$(stat -c %Y $y)
		let total=$tiempo-$tiempo2
		if [[ "$total" -gt 200 ]]
		then
			echo "Borramos la carpeta $y con más de $total segundos."
			rm $y
		fi
	done
fi