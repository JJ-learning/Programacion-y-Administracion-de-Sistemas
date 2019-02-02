#!/bib/bash
clear
chmod u+x .
read -p "Introduzca su edad: " x
if [ "$x" -lt 18 ];
then
	let edad=18-$x
	echo "No tienes edad para conducir. Te faltan $edad años" 
else
	numConducidas=$(($RANDOM%99))
	totalCondu=$[$numConducidas*$x]
	echo "Tienes permitido conducir y has conducido $totalCondu veces hasta la fecha."
fi