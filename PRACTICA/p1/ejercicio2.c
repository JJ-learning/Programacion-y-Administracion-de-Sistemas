#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>


//Funciones
void mostrarInfo(char *gvalue)
{
	struct group *gp;

	gp = getgrnam(gvalue);

	if(gp==NULL)
	{
		printf("Grupo no encontrado en el sistema\n");
	}

	printf("Nombre del grupo: %s\n", gp->gr_name);
	printf("Identificador del grupo: %d\n", gp->gr_gid);
}

void mostrarInfo2()
{
	char *grupo;
	struct group *gp;

	grupo = getenv("USER");
	gp = getgrnam(grupo);

	if(gp==NULL)
	{
		printf("Grupo no encontrado en el sistema\n");
	}

	printf("Nombre del grupo: %s\n", gp->gr_name);
	printf("Identificador del grupo: %d\n", gp->gr_gid);
}

void printInfo(char *gvalue)
{
	struct group *gp;

	gp = getgrnam(gvalue);

	if(gp==NULL)
	{
		printf("Group not found in the system\n");
	}

	printf("Group name: %s\n", gp->gr_name);
	printf("Group identifier: %d\n", gp->gr_gid);
}

void printInfo2()
{
	char *grupo;
	struct group *gp;

	grupo = getenv("USER");
	gp = getgrnam(grupo);

	if(gp==NULL)
	{
		printf("Group not found in the system\n");
	}

	printf("Group name: %s\n", gp->gr_name);
	printf("Group identifier: %d\n", gp->gr_gid);
}

void mostrarTodos()
{
	struct group *gp;
	FILE *f=NULL;
	char cadena[130], nombre[130];
	int i;

	if((f=fopen("/etc/group", "r"))!=NULL)
	{
		while(fscanf(f, "%s", cadena)!=EOF)
		{
			for(i=0;cadena[i]!=':';i++)
			{
				nombre[i]=cadena[i];
			}

			nombre[i]='\0';//Para evitar que el salto de linea se quede en el nombre
			gp = getgrnam(nombre);

			printf("Nombre del grupo: %s\n", gp->gr_name);
			printf("Identificador del grupo: %d\n", gp->gr_gid);
			puts("");
		}
	}
}

void printAll()
{
	struct group *gp;
	FILE *f=NULL;
	char cadena[130], nombre[130];
	int i;

	if((f=fopen("/etc/group", "r"))!=NULL)
	{
		while(fscanf(f, "%s", cadena)!=EOF)
		{
			for(i=0;cadena[i]!=':';i++)
			{
				nombre[i]=cadena[i];
			}

			nombre[i]='\0';//Para evitar que el salto de linea se quede en el nombre
			gp = getgrnam(nombre);

			printf("Group name: %s\n", gp->gr_name);
			printf("Group identifier: %d\n", gp->gr_gid);
			puts("");
		}
	}
}

void ayuda()
{
	printf("Uso del programa: ./a.out [opcion]\n");
	printf("Opciones:\n");
	printf("-h, --help	Imprimir esta ayuda\n");
	printf("-g, --group	Grupo a analizar\n");
	printf("-r, --groupr Grupo actual del sistema\n");
	printf("-e, --english	Mensaje en ingles\n");
	printf("-s, --spanish	Mensaje en castellano\n");
	printf("-x, --allgroup	Imprimir grupos del sistema\n");
}

void help()
{
	printf("Use of the program: ejercicio2 [option]\n");
	printf("Options:\n");
	printf("-h,--help	Print this help\n");
	printf("-g,--group	Group to analyze\n");
	printf("-r, --groupr Actual group of the system\n");
	printf("-e,--english	Message in english\n");
	printf("-s,--spanish	Message in spanish\n");
	printf("-a,--all	Print the system groups\n");
}

//Programa principal
int main(int argc, char **argv)
{
	system("clear");
	int o;
	static struct option long_options[]=
	{
		/*Opciones que puede tener el programa*/
		{"group", required_argument, 0, 'g'},
		{"groupr", no_argument, 0, 'r'},
		{"english", no_argument, 0, 'e'},
		{"spanish", no_argument, 0, 's'},
		{"help", no_argument, 0, 'h'},
		{"allgroup", no_argument, 0, 'x'},

		{0,0,0,0}//Lo ponemos para indicar la finalización de las opciones
	};

	//Flags
	int gflag = 0;
	int rflag = 0;
	int eflag = 0;
	int sflag = 0;
	int hflag = 0;
	int xflag = 0;

	char *gvalue=NULL;

	int option_index=0;//Para guardar el indice de la opcion

	while((o = getopt_long(argc, argv, "g:reshx", long_options, &option_index))!=1)
	{
		if(o==-1)
		{
			break;
		}
		switch(o)
		{
			case 'g':
				gflag=1;
				gvalue = optarg;
				break;
			case 'r':
				rflag=1;
				break;
			case 'e':
				eflag=1;
				break;
			case 's':
				sflag=1;
				break;
			case 'h':
				hflag=1;
				break;
			case 'x':
				xflag=1;
				break;
			case '?':
				//optget_long tiene su propio mensaje de error, si queremos cambiarlo hay que poner opterr=0
				return 1;
			default:
				abort();
		}
		printf("optind: %d, optarg: %s, optopt: %c\n", optind, optarg, optopt);
	}
	if(sflag==1 && eflag==1)
	{
		printf("No se puede mostrar la informacion en español e ingles al mismo tiempo\n");
		printf("Cannot print the information in spanish and english at the same time\n");
		exit(1);
	}
	else if(gflag==1 && rflag==1)
	{
		printf("Las opciones -g y -r no se pueden usar al mismo tiempo\n");
		printf("The options -g and -r cannot use at the same time\n");
		exit(1);
	}
	else if(gflag==1 && sflag==1)
	{
		mostrarInfo(gvalue);
	}
	else if(gflag==1 && eflag==1)
	{
		printInfo(gvalue);
	}
	else if(rflag==1 && sflag==1)
	{
		mostrarInfo2();
	}
	else if(rflag==1 && eflag==1)
	{
		printInfo2();
	}
	else if(xflag==1 && sflag==1)
	{
		mostrarTodos();
	}
	else if(xflag==1 && eflag==1)
	{
		printAll();
	}
	else if(gflag==1)
	{
		mostrarInfo(gvalue);
	}
	else if(rflag==1)
	{
		mostrarInfo2();
	}
	else if(xflag==1)
	{
		mostrarTodos();
	}
	else if(hflag==1 && sflag==1)
	{
		ayuda();
	}
	else if(hflag==1 && eflag==1)
	{
		help();
	}
	else if(hflag==1)
	{
		ayuda();
		puts("");
		help();
	}

	return 0;
}