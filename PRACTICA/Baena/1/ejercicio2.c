#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <getopt.h>
#include <unistd.h>

int main(int argc,char * argv[]){

	int flag,E=0,S=0,H=0,A=0,option_index=0,i;
	char * group=NULL,*idioma;
	char cadena[125],nombre[125];
	FILE * f=NULL;
	struct passwd * pw;
	struct group * gr;
	struct option lopt[]={

		{"group",required_argument,0,'g'},
		{"spanish",no_argument,0,'s'},
		{"english",no_argument,0,'e'},
		{"help",no_argument,0,'h'},
		{"all",no_argument,0,'a'},

		{0,0,0,0}
	
	};

	while((flag=getopt_long(argc,argv,"g:seha",lopt,&option_index))!=-1){

		switch(flag){	

			case 'g':

			group=optarg;
			break;

			case 's':
		
			S=1;
			break;
			
			case 'e':
	
			E=1;
			break;

			case 'h':
		
			H=1;
			break;
	
			case 'a':
	
			A=1;
			break;

			case '?':

			printf("#SPANISH#\n");
			printf("Uso del programa: ejercicio2 [Opciones]\n");
			printf("Opciones:\n");
			printf("-h,--help	Imprimir esta ayuda\n");
			printf("-g,--group	Grupo a analizar\n");
			printf("-e,--english	Mensaje en ingles\n");
			printf("-s,--spanish	Mensaje en castellano\n");
			printf("-a,--all	Imprimir grupos del sistema\n");

			printf("#ENGLISH#\n");
			printf("Use of the program: ejercicio2 [Options]\n");
			printf("Options:\n");
			printf("-h,--help	Print this help\n");
			printf("-g,--group	Group to analyze\n");
			printf("-e,--english	Message in english\n");
			printf("-s,--spanish	Message in spanish\n");
			printf("-a,--all	Print the system groups\n");
			break;

			default:
			
			exit(1);

		}

	}

	if(H==1){	//Para mostrar la ayuda

	printf("Uso del programa: ejercicio2 [Opciones]\n");
	printf("Opciones:\n");
	printf("-h,--help	Imprimir esta ayuda\n");
	printf("-g,--group	Grupo a analizar\n");
	printf("-e,--english	Mensaje en ingles\n");
	printf("-s,--spanish	Mensaje en castellano\n");
	printf("-a,--all	Imprimir grupos del sistema\n");

	printf("Use of the program: ejercicio2 [Options]\n");
	printf("Options:\n");
	printf("-h,--help	Print this help\n");
	printf("-g,--group	Group to analyze\n");
	printf("-e,--english	Message in english\n");
	printf("-s,--spanish	Message in spanish\n");
	printf("-a,--all	Print the system groups\n");

	}	

	if(group!=NULL){

		gr=getgrnam(group);

		if(gr==NULL){

		printf("Grupo no encontrado en el sistema\n");
		printf("Group not found in the system\n");

		}

		else{

			if(S==1&&E==0){

			printf("Número del grupo principal:%d\n",(*gr).gr_gid);
	    		printf("Nombre del grupo principal:%s\n",(*gr).gr_name);

			}

			else if(S==0&&E==1){

			printf("Main Group Number:%d\n",(*gr).gr_gid);
	    		printf("Main Group Name:%s\n",(*gr).gr_name);

			}

			else if(S==0&&E==0){

			idioma=getenv("LANG");

				if(idioma[1]=='s'){

				printf("Número del grupo principal:%d\n",(*gr).gr_gid);
	    			printf("Nombre del grupo principal:%s\n",(*gr).gr_name);

				}
				
				else if(idioma[1]=='n'){

				printf("Main Group Number:%d\n",(*gr).gr_gid);
	    			printf("Main Group Name:%s\n",(*gr).gr_name);

				}

			}

			else{

			printf("No se pueden activar dos idiomas al mismo tiempo\n");
			printf("Two languages can't be used at same time\n");

			}

		}

	}
	
	if(group==NULL){

		group=getenv("USER");

		gr=getgrnam(group);

		if(gr==NULL){

		printf("Grupo no encontrado en el sistema\n");
		printf("Group not found in the system\n");

		}

		else{

			if(S==1&&E==0){

			printf("Número del grupo principal:%d\n",(*gr).gr_gid);
	    		printf("Nombre del grupo principal:%s\n",(*gr).gr_name);

			}

			else if(S==0&&E==1){

			printf("Main Group Number:%d\n",(*gr).gr_gid);
	    		printf("Main Group Name:%s\n",(*gr).gr_name);

			}

			else if(S==0&&E==0){

			idioma=getenv("LANG");

				if(idioma[1]=='s'){

				printf("Número del grupo principal:%d\n",(*gr).gr_gid);
	    			printf("Nombre del grupo principal:%s\n",(*gr).gr_name);

				}
				
				else if(idioma[1]=='n'){

				printf("Main Group Number:%d\n",(*gr).gr_gid);
	    			printf("Main Group Name:%s\n",(*gr).gr_name);

				}

			}

			else{

			printf("No se pueden activar dos idiomas al mismo tiempo\n");
			printf("Two languages can't be used at same time\n");

			}

		}

	}

	if(A==1){

		if((f=fopen("/etc/group","r"))!=NULL){

			while(fscanf(f,"%s",cadena)!=EOF){

				for(i=0;cadena[i]!=':';i++){

					nombre[i]=cadena[i];
		
				}

				

				nombre[i]='\0';
				printf("%s\n",nombre);

				gr=getgrnam(nombre);

				if(S==1&&E==0){

				printf("Número del grupo principal:%d\n",(*gr).gr_gid);
				printf("Nombre del grupo principal:%s\n",(*gr).gr_name);

				}
	
				else if(E==1&&S==0){

				printf("Main Group Number:%d\n",(*gr).gr_gid);
				printf("Main Group Name:%s\n",(*gr).gr_name);

				}

				else if(S==0&&E==0){

				idioma=getenv("LANG");

				if(idioma[1]=='s'){

				printf("Número del grupo principal:%d\n",(*gr).gr_gid);
				printf("Nombre del grupo principal:%s\n",(*gr).gr_name);

				}
				
				else if(idioma[1]=='n'){

				printf("Main Group Number:%d\n",(*gr).gr_gid);
				printf("Main Group Name:%s\n",(*gr).gr_name);

				}

				}

				else{

				printf("No se pueden usar dos idiomas a la vez\n");
				printf("Two languages can't be used at sime time\n");

				}

			}

		}
	
		else{

		printf("ERROR al leer el fichero\n");
		exit(-1);

		}

	}

	else{
		
	printf("El nombre del Grupo y el GID no deben ser usados al mismo tiempo\n");
	printf("GID and group name should not be used at the same time\n");

	}

return 0;
}
