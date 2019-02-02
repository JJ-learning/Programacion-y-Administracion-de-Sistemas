#!/bib/bash
clear
if [[ ! "$#" -ge 1 ]];
then
	echo "Error en la línea de argumentos, la ejecución sería:"
	echo "ejercicio3.sh [Carpeta] [Num archivos pequenios] [Num archivos grandes]"
else #Si no se declara la cantidad, ponemos 512 por defecto.
	if [ "$#" -eq 1 ];
	then
		tamanioPeque=512
		tamanioGrande=1024
		numPeque=3
		numGrande=3
	else
		tamanioPeque=512
		tamanioGrande=1024
		numPeque=$2 #Numero de archivos en una carpeta "
		numGrande=$3
	fi

	#Comprobamos si existen los ficheros, si existen se borran, y sino, se crean
	if [ -e $1/pequenios ];
	then
		rm -d -r $1/pequenios
		mkdir $1/pequenios
	else
		mkdir $1/pequenios
	fi

	if [ -e $1/medianos ];
	then
		rm -d -r $1/medianos
		mkdir $1/medianos
	else
		mkdir $1/medianos
	fi

	if [ -e $1/grandes ];
	then
		rm -d -r $1/grandes
		mkdir $1/grandes
	else
		mkdir $1/grandes
	fi

	#Buscamos la carpeta y guardamos los archivos en x
	echo "">ficheros.txt
	find $1 | while read x
	do
		if [[ -f $x ]];
		then
			tamanio=$(stat -c %s $x)
			echo "$tamanio" | tee -a ficheros.txt
		fi	
	done | sort -t ',' -nk1	
	
	echo "Todo creado correctamente"

fi