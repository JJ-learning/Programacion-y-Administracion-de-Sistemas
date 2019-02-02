#!/bin/bash
clear
if [[ "$#" -gt 1 || "$#" -lt 1 ]];
then
	echo "La ejecución es ejercicio2.sh [peliculass.txt]"
else 
	cat $1 | sed -r -e 's/(\(.*\)).\[.*\]$/|->Fecha: \1/' -e 's/Dirigida por (.*)/|->Director: \1/' -e 's/^[ \t].*$//' -e '/^$/d' -e '/^=/d' -e 's/Reparto:(.*)/|->Reparto: \1/' -e 's/([1-9]\h.[0-9][1-9]\min)/|->Duracion: \1/' -e 's/(^[A-Z¡].*)/Titulo: \1/' 
fi