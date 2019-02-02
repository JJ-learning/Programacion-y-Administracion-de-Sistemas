#ª/ bin/ bash
read -p "Introduzca un número: " X
read -p "Introduzca un segundo numero: " Y
Z=$(($X+$Y))
echo "La suma de $X+$Y es: $Z"
let Z=$X-$Y
echo "La resta de $X-$Y es: $Z"

read -p "Introduzca su nombre: " nombre
if [ "$nombre" == "$USER" ]; #Se ponen las comillas para que no salga un error de sintaxis; 
then #Poner siempre que usamos cadenas
	echo "Bienvenido $nombre"
else
	echo "ERRORRRRRR"
fi

read -p "Introduzca un numero entre 1<= x <10" X
if [[ "$X" -ge 1 && "$X" -lt 10 ]]; 
then
	echo "Numero Correcto"
else
	echo "Numero Incorrecto"
fi

dia="$(date +%d-%m-%y)"
if [ ! $# -eq 1 ];
then
	echo "La invocacion es $0 nombreFichero"
elif [[ -e "$1" ]];
then
	echo "El fichero $1 existe"
	cp "$1" "$1.back_$dia"
	ls
else
	echo "El fichero no existe"
fi