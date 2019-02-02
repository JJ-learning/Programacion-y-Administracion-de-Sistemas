#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>


//Funciones
void mostrarInfo(char *avalue)
{
	char *lgn;
	struct passwd *pw;
	else if((pw = getpwnam(avalue))==NULL)
	{
		printf("Error al mostrar la informacion del usuario. Nombre no encontrado\n");
		exit(1);
	}

	printf("Nombre: %s\n", pw->pw_gecos);
	printf("Password: %s\n", pw->pw_passwd);
	printf("UID: %d\n", pw->pw_uid);
	printf("Home: %s\n", pw->pw_dir);
	printf("Número de grupo principal: %d\n\n", pw->pw_gid);
}

void mostrarInfo2(int bvalue)
{
	struct passwd *pw;
	if((pw = getpwuid(bvalue))==NULL)
	{
		printf("Error al mostrar la informacion del usuario. ID no encontrada\n");
		exit(1);
	}

	printf("Nombre: %s\n", pw->pw_gecos);
	printf("Password: %s\n", pw->pw_passwd);
	printf("UID: %d\n", pw->pw_uid);
	printf("Home: %s\n", pw->pw_dir);
	printf("Número de grupo principal: %d\n\n", pw->pw_gid);
}

void mostrarInfoActual()
{
	char *lgn;
	struct passwd *pw;
	if((lgn = getlogin()) ==NULL || (pw=getpwnam(lgn)) ==NULL)
	{
		printf("Error al mostrar la informacion del usuario\n");
		exit(1);
	}
	printf("Nombre: %s\n", pw->pw_gecos);
	printf("Password: %s\n", pw->pw_passwd);
	printf("UID: %d\n", pw->pw_uid);
	printf("Home: %s\n", pw->pw_dir);
	printf("Número de grupo principal: %d\n\n", pw->pw_gid);
}

void mostrarInfoGrupo(char *avalue)
{
	struct passwd *pw;
	struct group *gp;

	if((pw=getpwnam(avalue))==NULL)
	{
		printf("Error al mostrar la informacion del usuario\n");
		exit(1);
	}

	gp=getgrgid(pw->pw_gid);

	printf("Nombre del grupo principal: %s\n\n", gp->gr_name);
}

void printInfo(char *avalue)
{
	char *lgn;
	struct passwd *pw;
	else if((pw = getpwnam(avalue))==NULL)
	{
		printf("Error. Name not found\n");
		exit(1);
	}

	printf("Name: %s\n", pw->pw_gecos);
	printf("Password: %s\n", pw->pw_passwd);
	printf("UID: %d\n", pw->pw_uid);
	printf("Home: %s\n", pw->pw_dir);
	printf("Number of principal group: %d\n\n", pw->pw_gid);
}

void printInfo2(int bvalue)
{
	struct passwd *pw;
	if((pw = getpwuid(bvalue))==NULL)
	{
		printf("Error. Name not found\n");
		exit(1);
	}

	printf("Name: %s\n", pw->pw_gecos);
	printf("Password: %s\n", pw->pw_passwd);
	printf("UID: %d\n", pw->pw_uid);
	printf("Home: %s\n", pw->pw_dir);
	printf("Number of principal group: %d\n\n", pw->pw_gid);
}

void printActualInfo()
{
	char *lgn;
	struct passwd *pw;
	if((lgn = getlogin()) ==NULL || (pw=getpwnam(lgn)) ==NULL)
	{
		printf("Error al mostrar la informacion del usuario\n");
		exit(1);
	}
	printf("Name: %s\n", pw->pw_gecos);
	printf("Password: %s\n", pw->pw_passwd);
	printf("UID: %d\n", pw->pw_uid);
	printf("Home: %s\n", pw->pw_dir);
	printf("Number of group: %d\n\n", pw->pw_gid);
}

void printInfoGrupo(char *avalue)
{
	struct passwd *pw;
	struct group *gp;

	if((pw=getpwnam(avalue))==NULL)
	{
		printf("Error al mostrar la informacion del usuario\n");
		exit(1);
	}

	gp=getgrgid(pw->pw_gid);

	printf("Name of principal Group: %s\n\n", gp->gr_name);
}

//Programa principal
int main(int argc, char **argv)
{
	system("clear");
	int aflag=0;
	int bflag=0;
	int cflag=0;
	int dflag=0;
	int eflag=0;
	int sflag=0;

	char *avalue;
	int bvalue;

	int o;

	/*Recogemos la opcion que se va a utilizar*/
	while((o = getopt(argc,argv, "a:b:cdes")) !=-1)
	{
		/*Recorremos las distintas opciones para ver cuales se ejecutan*/
		switch(o)
		{
			case 'a':
				aflag=1;
				avalue = optarg;
				break;
			case 'b':
				bflag=1;
				bvalue = atoi(optarg);
				break;
			case 'c':
				cflag=1;
				break;
			case 'd':
				dflag=1;
				break;
			case 'e':
				eflag=1;
				break;
			case 's':
				sflag=1;
				break;
			case '?': //Opcion no reconocida o incompleta
	            if (optopt == 'b') //Para el caso de que 'c' no tenga el argumento obligatorio.
	                fprintf (stderr, "La opción %c requiere un argumento.\n", optopt);
	            else if (isprint (optopt)) //Se mira si el caracter es imprimible
	                fprintf (stderr, "Opción desconocida \"-%c\".\n", optopt);
	            else
	                fprintf (stderr, "Caracter `\\x%x'.\n", optopt);
	            return 1;
	        default:
	        	abort();
		}
		printf("optind: %d, optarg: %s, optopt: %c\n", optind, optarg, optopt);
	}

	if(aflag==1 && bflag==1)
	{
		printf("\nError. No se puden usar -a y -b al mismo tiempo\n");
		exit(1);
	}
	else if(sflag==1 && eflag==1)
	{
		printf("No se puede mostrar la informacion en español e ingles al mismo tiempo\n");
		printf("Cannot print the information in spanish and english at the same time\n");
		exit(1);
	}
	else if(aflag==1 && eflag==1)
	{
		printInfo(avalue);		
	}
	else if(aflag==1 && sflag==1)
	{
		mostrarInfo(avalue);
	}
	else if(bflag==1 && sflag==1)
	{
		mostrarInfo2(bvalue);
	}
	else if(bflag==1 && eflag==1)
	{
		printInfo2(bvalue);
	}
	else if(cflag==1 && eflag==1)
	{
		printActualInfo();
	}
	else if(dflag==1 && eflag==1)
	{
		if(avalue==NULL)
		{
			printf("\nName not found, please, put the option -a before the option -d\n");
			exit(1);
		}
		printInfoGrupo(avalue);
	}
	else if(aflag==1)
	{
		mostrarInfo(avalue);
	}
	else if(bflag==1)
	{
		mostrarInfo2(bvalue);
	}
	else if(cflag==1)
	{
		mostrarInfoActual();
	}
	else if(dflag==1)
	{
		if(avalue==NULL)
		{
			printf("\nNombre de usuario no encontrado, por favor, introduzca la opcion -a\n");
			exit(1);
		}
		mostrarInfoGrupo(avalue);
	}
	
	/*Imprimimos los flag de las opciones que se han utilizado*/
	printf("aflag=%d, bflag=%d, cflag=%d, dflag=%d, eflag=%d, sflag=%d\n", aflag, bflag, cflag, dflag, eflag, sflag);

	return 0;
}