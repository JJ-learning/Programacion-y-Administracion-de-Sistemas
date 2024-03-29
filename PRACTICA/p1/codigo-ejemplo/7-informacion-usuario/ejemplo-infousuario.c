#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

/*
Consulte el fichero /etc/passwd para ver que significa cada uno de sus campos,
como por ejemplo en esta linea:
jfcaballero:x:1000:1000:Juan Carlos Fernández Caballero,,,:/home/jfcaballero:/bin/bash

Haga lo mismo para el fichero /etc/group, identifique cada campo, como por ejemplo:
adm:x:4:syslog,jfcaballero
*/


int main(int argc, char *argv[])
{
	 // Declaracion de las estructuras que almacenaran la informarcion de un usuario y de un grupo
    char *lgn;
    struct passwd *pw;
    struct group *gr;

    /*En el caso de que no se ha introducido ningún nombre de usuario,
    por defecto en lgn almacenara el usuario que ha ejecutado este programa*/
    if (argc < 2)
    {
        if ((lgn = getlogin()) == NULL || (pw = getpwnam(lgn)) == NULL)
        {
            fprintf(stderr, "Get of user information failed.\n");
            exit(1);
        }
    }
    else //Se indica el nombre de usaurio en el primer argumento.
    {
        lgn = argv[1];

        if ((pw = getpwnam(lgn)) == NULL)
        {
            fprintf(stderr, "Get of user information failed.\n");
            exit(1);
        }
    }

	//Aqui ya se dispone de la estructura que contiene informacion del usuario
	printf("Nombre: %s\n", pw->pw_gecos);
	printf("Password: %s\n", pw->pw_passwd);
	printf("UID: %d\n", pw->pw_uid);
	printf("Home: %s\n", pw->pw_dir);
	printf("Número de grupo principal: %d\n", pw->pw_gid);

	// Obtenemos la estructura de información del grupo a través del número de grupo al que pertenece el usuasrio
	gr = getgrgid(pw->pw_gid);

	//Se imprime el campo de la estructura que nos interesa
	printf("Nombre del grupo principal: %s\n", gr->gr_name);

	exit(0);

}

