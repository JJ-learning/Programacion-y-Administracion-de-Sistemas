#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <ctype.h>

void mostrarActual()
{
	char *lgn;
	struct passwd *pw;

	lgn=getenv("USER");
	if((pw=getpwnam(lgn))==NULL)
	{
		printf("usuario no encontrado %c\n", optopt);
	}
	puts("");
	if((lgn=getlogin())==NULL || (pw=getpwnam(lgn))==NULL)
	{
		printf("Usuario no encontrado %s\n",lgn);
	}
	fprintf(stderr, "Nombre: %s\n", pw->pw_gecos);
	fprintf(stderr, "UID: %d\n", pw->pw_uid);
}

void tuberias()
{
	int tub[2], status, errno;
	pid_t hijo, flag;
	int num;
	char frase[200];

	//Creamos la tubería
	pipe(tub);

	hijo=fork();
	switch(hijo)
	{
		case -1:
			printf("Error, errno value %d\n", errno);
			exit(EXIT_FAILURE);
			break;
		case 0:
			close(tub[1]);
			read(tub[0], &frase, 200);
			num=num-2;
			printf("Leo: %s\n", frase);
			close(tub[0]);
			exit(EXIT_SUCCESS);
			break;

		default:
			close(tub[0]);
			fprintf(stderr, "Escribe una frase:\n");
			fgets (frase, 200, stdin);
			write(tub[1], &frase, 200);
			close(tub[1]);
			
			break;
	}		
	while((flag=wait(&status))>0)
	{
		if(WIFEXITED(status))
		{
			printf("Hijo %d finalizado, status:%d\n", flag, WEXITSTATUS(status));
		}
		else if(WIFSIGNALED(status))
		{
			printf("Hijo %d parado por la señal %d\n", flag, WTERMSIG(status));
		}
		else if(WIFSTOPPED(status))
		{
			printf("Hijo %d parado por la señal %d\n", flag, WSTOPSIG(status));
		}
		else if(WIFCONTINUED(status))
		{
			printf("Hjo %d reanudado\n", flag);
		}
	}
	//Cuando no esperamos más hijos
	if(flag==(pid_t)-1 && errno==ECHILD)
	{
		printf("errno %d, %d\n", errno, strerror(errno));
	}
	else
	{
		printf("Error en la llamada al wait\n");
		exit(EXIT_FAILURE);
	}
}

void printInfo(char *uvalue)
{
	char *lgn;
	struct passwd *pw;
	if((pw=getpwnam(uvalue))==NULL)
	{
		fprintf(stderr, "Error, usuairo %s no conocido\n", uvalue);
	}
	fprintf(stderr, "Nombre:%s\n", pw->pw_gecos);
	fprintf(stderr, "UID:%d\n", pw->pw_uid);
	fprintf(stderr, "HOME:%s\n", pw->pw_dir);
	fprintf(stderr, "Password:%s\n", pw->pw_passwd);
	fprintf(stderr, "Numero de Grupo:%d\n", pw->pw_gid);
}

void printGrupo(char *gvalue)
{
	struct group *gp;
	if((gp=getgrnam(gvalue))==NULL)
	{
		fprintf(stderr, "Grupo %s no conocido\n", gvalue);
	}

	fprintf(stderr, "Nombre:%s\n", gp->gr_name);
	fprintf(stderr, "GID:%d\n", gp->gr_gid);
}

int main(int argc, char **argv)
{
	system("clear");
	int o;
	static struct option long_options[]=
	{
		{"user", required_argument, 0, 'u'},
		{"grupo", required_argument, 0, 'g'},
		{"pipes", no_argument, 0, 'p'},
		{"actual", no_argument, 0, 'a'},

		{0,0,0,0}//Siempre lo ponermos para terminar las opciones
	};
	int uflag=0, gflag=0, pflag=0, aflag=0;
	char *uvalue=NULL, *gvalue=NULL;
	int option_index=0;
	while((o = getopt_long(argc, argv, "u:g:pa", long_options, &option_index))!=-1)
	{
		switch(o)
		{
			case 'u':
				uflag=1;
				uvalue=optarg;
				break;
			case 'g':
				gflag=1;
				gvalue=optarg;
				break;
			case 'p':
				pflag=1;
				break;
			case 'a':
				aflag=1;
				break;
			case '?':
				if(isprint(optopt))
				{
					fprintf(stderr, "Opcion desconocida %c\n", optopt);
				}
				else 
				{
					fprintf(stderr, "Caracter: %x\n", optopt);
				}
				break;
		}
		fprintf(stderr, "Optind: %d, optarg: %s, optopt: %c\n", optind, optarg, optopt);
	}
	if(uvalue==NULL)
	{
		uvalue=getlogin();
	}
	
	//Combinamos:
	if(pflag==1)
	{
		tuberias();
	}
	else if(aflag==1)
	{
		mostrarActual();
	}
	else if(uflag==1 && gflag==1)
	{
		fprintf(stderr, "No se pueden poner las dos opciones a la vez\n");
	}
	else if(uflag==1)
	{
		printInfo(uvalue);
	}
	else if(gflag==1)
	{
		printGrupo(gvalue);
	}
	return 0;
}
