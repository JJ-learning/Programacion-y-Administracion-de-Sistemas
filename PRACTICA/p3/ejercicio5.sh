#!/bin/bash
clear
if [[ "$#" -lt 0 || "$#" -ge 1 ]];
then
	echo "La ejecución es ./ejercicio5.sh"
else
	cpu="/proc/cpuinfo"
	montajes="/proc/mounts"
	particiones="/proc/partitions"

	echo "INFORMACION CPU"
	cat $cpu | sed -r -n -e 's/model name.*: (.*)$/Modelo Procesador: \1/p' -e 's/cpu MHz.*: (.*)$/Megahercios: \1/p' -e 's/cache size.*: (.*)$/Tamaño caché: \1/p' -e 's/vendor_id.*: (.*)/ID vendedor: \1/p' -e 's/sibling.*: (.*)$/Numero maximo de hilos: \1/p'
	echo""
	echo "PUNTOS DE MONTAJE"
	cat $montajes | sed -r -n -e 's/(.*) (.*) (.*) (.*) 0 0$/|-> Punto de montaje: \2,\tDispositivo: \1,\tTipo de montaje: \3/p' | sort -r
	echo""
	echo "PARTICIONES"
	cat $particiones | sed -r -n -e 's/([0-9]+) (.*) (.*) (.*)$/|-> Particion: \4,\tNumero de bloques: \3/p' | sort -r
fi