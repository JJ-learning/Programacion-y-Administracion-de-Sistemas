#!/bin/bash

echo "Modelo de procesador: "
awk -F":" ' $1 ~/model name/ { print $2 } ' /proc/cpuinfo
echo "Megahercios: "
awk -F":" ' $1 ~/cpu MHz/ { print $2 } ' /proc/cpuinfo
echo "Numero maximo de hilos en ejecucion: "
awk -F":" ' $1 ~/siblings/ { print $2 } ' /proc/cpuinfo
echo "Puntos de montaje: "
sort -k 3 /proc/mounts | awk '{ print "-> Punto de montaje: "$2 " Dispositivo: "$1 " Tipo de dispositivo: "$3 }'
echo "Particiones y numero de bloques: "
sed -n '1,2!p' /proc/partitions | sort -n -r -k 3 | awk '{ print "-> Particion "$4 ", Numero de bloques: "$3 }'
