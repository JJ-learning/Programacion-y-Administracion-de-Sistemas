/*
Ejemplo extraído del estándar POSIX y glibc.
Enlace a la página de documentación de la implemencación de getopt de glibc:
http://pubs.opengroup.org/onlinepubs/9699919799/functions/getopt.html
http://www.gnu.org/software/libc/manual/html_node/Getopt.html
http://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
*/

#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int
main (int argc, char **argv)
{
    int aflag = 0;
    int bflag = 0;
    char *cvalue = NULL;
    int index;
    int c;

    /* Las variables optind, optarg, opterr y optopt son variables externas incluidas en <unistd.h> 
    Busca información sobre ellas para ver cuando y porque se actualizan*/
    opterr = 0;

    /* "abc:" -> busca como opciones a y b sin argumentos y c con un argumento OBLIGATORIO.
       ':' indica que la opcion debe llevar un argumento obligario
    
    /* getopt va iterando, y devuelve -1 si ya hemos consultado toda la línea de argumentos.
       Sino, devuelve el caracter de opción encontrado para caracteres válidos 
             o devuelve ? si el caracter no es válido según la cadena especificada.*/
    while ((c = getopt (argc, argv, "abc:")) != -1)
    {
        /* Podemos observar qué pasa con las variables externas que va gestionando
           getopt() durante las sucesivas llamadas. 
           - optarg recoge el valor del argumento obligario de una opcion optind 
           - optopt recoge el valor del argumento cuando la opción es desconocida (?)*/
        switch (c)
        {
        case 'a':
            aflag = 1; //Bandera que indica que la opción -a se ha pasado por linea de argumentos
            break;
        case 'b':
            bflag = 1;
            break;
        case 'c':
            cvalue = optarg; //En optarg se guarda el valor de argumento obligatorio que requiere c
            break;
        case '?': //Opcion no reconocida o incompleta
            if (optopt == 'c') //Para el caso de que 'c' no tenga el argumento obligatorio.
                fprintf (stderr, "La opción %c requiere un argumento.\n", optopt);
            else if (isprint (optopt)) //Se mira si el caracter es imprimible
                fprintf (stderr, "Opción desconocida \"-%c\".\n", optopt);
            else
                fprintf (stderr, "Caracter `\\x%x'.\n", optopt);
            return 1;
        default:
            abort ();
        }
        printf("optind: %d, optarg: %s, optopt: %c\n", optind, optarg, optopt);
    }

    /* Este último bucle controla opciones introducidas por el usuario que no hayan sido procesadas. 
       Compara el número de argumentos recibidos con el número de opciones reconocidas. 
       Como getopt() internamente reordena los valores de argv, las primeras posiciones de argv corresponden
       a opciones conocidas y las últimas, a partir de optind, a opciones no reconocidas */
    for (index = optind; index < argc; index++)
        printf ("Argumento \"%s\" de la línea de comandos que NO es una opción.\n", argv[index]);

    /* El siguiente condicional permitiría dar un valor por defecto a la opción c.
    Comprueba y razona si se llegaría a ejecutar y en qué circunstancias */
    if(cvalue==NULL)
        cvalue="ValorPorDefecto";

    //Para visualizar que opciones se han activado y sus argumentos
    printf ("aflag = %d, bflag = %d, cvalue = %s\n", aflag, bflag, cvalue);
    
    return 0;
}


