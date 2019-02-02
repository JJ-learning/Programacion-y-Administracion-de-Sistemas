#!/bin/bash

if [ $# -eq 0 ] || [ $# -gt 2 ];
then
echo "Error en linea de ordenes"
else
	if [ -f $1 ]
	then
		nombreFich=$1
		nombreFichHTML="$(echo $nombreFich | sed -r -e 's/^(.*)\..*$/\1/g').html"
		echo "Generamos el fichero $nombreFichHTML..."
		echo "<html>" > $nombreFichHTML #Sobreescribe
		cat $nombreFich | sed -r -e 's/(.*):/<title> \1 <\/title>\n\<body\>/1' | sed -r -e '/<title>/,$s/^([^<>]*$)/<p> \1\ <\/p\>/g' | grep -E -v '^ *$'>>$nombreFichHTML
		echo "</body>">>$nombreFichHTML
		echo "</html>">>$nombreFichHTML
	fi
fi
