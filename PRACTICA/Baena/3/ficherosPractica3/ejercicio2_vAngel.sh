#!/bin/bash

clear #limpia la pantalla

wget https://twitter.com/GeoPortalMityc #descargar el contenido del twitter

cat GeoPortalMityc | sed -n -e 's/.*EcoPrecio [ ]*\([[:alnum:]]* [ ]*[[:alnum:]]*\) \(.*\) es \([[:digit:]]*[\.]*[[:digit:]]*€\) en \(.*\)<.*/Precio: "\3"€ Ciudad: "\2" Combustible: "\1" Direccion: "\4"/p' >> DireccTw.txt
#FALTA PONER LAS DIRECCION
echo -e "\n\nTweets ordenados por precio de combustible:\n"
sort -nk 2 DireccTw.txt

rm -f DireccTw.txt
rm -f DireccTw
