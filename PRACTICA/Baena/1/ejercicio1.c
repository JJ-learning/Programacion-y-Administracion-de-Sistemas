/* Código que soluciona el ejercicio resumen 1 */

#include <unistd.h>
#include <grp.h>     //Sirve para mostrar los grupos
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>     //Sirve para mostrar el usuario(passwd)

int main(int argc, char * argv[]){

	int flag,E=0,S=0,G=0;	//Banderas
	char * login=NULL, * uid=NULL, * idioma; //Variables que tomaran el valor con optarg

	struct passwd * pw;
	struct group * gr;

	//While con switch para escoger y ver que banderas se activan

	while((flag=getopt(argc,argv,"n:u:esg"))!=-1){

		switch(flag){

			case 'n':

			login=optarg;
			break;

			case 'u':
			
			uid=optarg;
			break;
	
			case 'e':

			E=1;
			break;

			case 's':
		
			S=1;
			break;
			
			case 'g':

			G=1;
			break;			

		}

	}

	//Si se introduce el login y no el uid

	if(login!=NULL&&uid==NULL){
	
		pw=getpwnam(login);

		if(pw==NULL){

		printf("Usuario no encontrado en el sistema\n");
		printf("User not found in the system\n");

		}

		else{

			//Activa el castellano

			if(S==1&&E==0){

			printf("Nombre del usuario:%s\n", (*pw).pw_gecos);
	            	printf("Id. del usuario:%d\n",(*pw).pw_uid);
	        	printf("Contraseña:%s\n",(*pw).pw_passwd);
	   		printf("Carpeta de inicio:%s\n",(*pw).pw_dir);
	   		printf("Interprete por defecto:%s\n",(*pw).pw_shell);

				//Activa mostrar grupo

				if(G==1){
				gr=getgrgid((*pw).pw_gid);
				printf("Número del grupo principal:%d\n",(*gr).gr_gid);
	    			printf("Nombre del grupo principal:%s\n",(*gr).gr_name);
				}

			}

			//Activa ingles

			else if(S==0&&E==1){

			printf("User name:%s\n",(*pw).pw_gecos);
	            	printf("User id:%d\n",(*pw).pw_uid);
	        	printf("Password:%s\n",(*pw).pw_passwd);
	   		printf("Home:%s\n",(*pw).pw_dir);
	   		printf("Default shell:%s\n",(*pw).pw_shell);

				//Activa mostrar grupo

				if(G==1){
				gr=getgrgid((*pw).pw_gid);
				printf("Main Group Number:%d\n",(*gr).gr_gid);
	    			printf("Main Group Name:%s\n",(*gr).gr_name);

				}

			}

			//Se coge el idioma predeterminado en el sistema ya que no se ha introducido ninguno.

			else if(E==0&&S==0){

			//Se coge el idioma de entorno
			
			idioma=getenv("LANG");
			
				//Activa castellano

				if(idioma[1]=='s'){

				printf("Nombre del usuario:%s\n", (*pw).pw_gecos);
	            		printf("Id. del usuario:%d\n",(*pw).pw_uid);
	        		printf("Contraseña:%s\n",(*pw).pw_passwd);
	   			printf("Carpeta de inicio:%s\n",(*pw).pw_dir);
	   			printf("Interprete por defecto:%s\n",(*pw).pw_shell);

					//Activa grupo

					if(G==1){
					gr=getgrgid((*pw).pw_gid);
					printf("Número del grupo principal:%d\n",(*gr).gr_gid);
	    				printf("Nombre del grupo principal:%s\n",(*gr).gr_name);
				
					}
				
				}

				//Activa ingles

				else if(idioma[1]=='n'){

				printf("User name:%s\n",(*pw).pw_gecos);
	            		printf("User id:%d\n",(*pw).pw_uid);
	        		printf("Password:%s\n",(*pw).pw_passwd);
	   			printf("Home:%s\n",(*pw).pw_dir);
	   			printf("Default shell:%s\n",(*pw).pw_shell);

					//Activa mostrar grupo

					if(G==1){
					gr=getgrgid((*pw).pw_gid);
					printf("Main Group Number:%d\n",(*gr).gr_gid);
	    				printf("Main Group Name:%s\n",(*gr).gr_name);

					}

				}

			}

			//ERROR SE HAN ACTIVADO DOS A LA VEZ

			else{

			printf("No se pueden activar dos idiomas al mismo tiempo\n");
			printf("Two languages can't be used at same time\n");

			}

		}

	}

	//Se introduce el uid y no el login

	else if(login==NULL&&uid!=NULL){
	
		//Transformamos el uid(char *) en int con atoi

		pw=getpwuid(atoi(uid));

		if(pw==NULL){

		printf("Usuario no encontrado en el sistema\n");
		printf("User not found in the system\n");

		}

		else{

			//Activa castellano

			if(S==1&&E==0){

			printf("Nombre del usuario:%s\n", (*pw).pw_gecos);
	            	printf("Id. del usuario:%d\n",(*pw).pw_uid);
	        	printf("Contraseña:%s\n",(*pw).pw_passwd);
	   		printf("Carpeta de inicio:%s\n",(*pw).pw_dir);
	   		printf("Interprete por defecto:%s\n",(*pw).pw_shell);

				//Activa grupo

				if(G==1){
				gr=getgrgid((*pw).pw_gid);
				printf("Número del grupo principal:%d\n",(*gr).gr_gid);
	    			printf("Nombre del grupo principal:%s\n",(*gr).gr_name);
				}

			}

			//Activa ingles

			else if(S==0&&E==1){

			printf("User name:%s\n",(*pw).pw_gecos);
	            	printf("User id:%d\n",(*pw).pw_uid);
	        	printf("Password:%s\n",(*pw).pw_passwd);
	   		printf("Home:%s\n",(*pw).pw_dir);
	   		printf("Default shell:%s\n",(*pw).pw_shell);

				//Activa grupo

				if(G==1){
				gr=getgrgid((*pw).pw_gid);
				printf("Main Group Number:%d\n",(*gr).gr_gid);
	    			printf("Main Group Name:%s\n",(*gr).gr_name);

				}

			}

			//No se introduce idioma y se coge el de entorno

			else if(E==0&&S==0){

			idioma=getenv("LANG");
			
				//Castellano

				if(idioma[1]=='s'){

				printf("Nombre del usuario:%s\n", (*pw).pw_gecos);
	            		printf("Id. del usuario:%d\n",(*pw).pw_uid);
	        		printf("Contraseña:%s\n",(*pw).pw_passwd);
	   			printf("Carpeta de inicio:%s\n",(*pw).pw_dir);
	   			printf("Interprete por defecto:%s\n",(*pw).pw_shell);

					//Activa grupo

					if(G==1){
					gr=getgrgid((*pw).pw_gid);
					printf("Número del grupo principal:%d\n",(*gr).gr_gid);
	    				printf("Nombre del grupo principal:%s\n",(*gr).gr_name);
				
					}
				
				}

				//Ingles

				else if(idioma[1]=='n'){

				printf("User name:%s\n",(*pw).pw_gecos);
	            		printf("User id:%d\n",(*pw).pw_uid);
	        		printf("Password:%s\n",(*pw).pw_passwd);
	   			printf("Home:%s\n",(*pw).pw_dir);
	   			printf("Default shell:%s\n",(*pw).pw_shell);

					//Activa grupo

					if(G==1){
					gr=getgrgid((*pw).pw_gid);
					printf("Main Group Number:%d\n",(*gr).gr_gid);
	    				printf("Main Group Name:%s\n",(*gr).gr_name);

					}

				}

			}

			//ERROR DOS LENGUAJES A LA VEZ

			else{

			printf("No se pueden activar dos idiomas al mismo tiempo\n");
			printf("Two languages can't be used at same time\n");

			}

		}

	}

	//No se introduce ni uid ni login

	else if(login==NULL&&uid==NULL){
		
		//Se coge el usuario de entorno

		login=getenv("USER");
		pw=getpwnam(login);

		if(pw==NULL){

		printf("Usuario no encontrado en el sistema\n");
		printf("User not found in the system\n");

		}

		else{

			//Castellano

			if(S==1&&E==0){

			printf("Nombre del usuario:%s\n", (*pw).pw_gecos);
	            	printf("Id. del usuario:%d\n",(*pw).pw_uid);
	        	printf("Contraseña:%s\n",(*pw).pw_passwd);
	   		printf("Carpeta de inicio:%s\n",(*pw).pw_dir);
	   		printf("Interprete por defecto:%s\n",(*pw).pw_shell);

				//Activa grupo

				if(G==1){
				gr=getgrgid((*pw).pw_gid);
				printf("Número del grupo principal:%d\n",(*gr).gr_gid);
	    			printf("Nombre del grupo principal:%s\n",(*gr).gr_name);
				}

			}

			//Ingles

			else if(S==0&&E==1){

			printf("User name:%s\n",(*pw).pw_gecos);
	            	printf("User id:%d\n",(*pw).pw_uid);
	        	printf("Password:%s\n",(*pw).pw_passwd);
	   		printf("Home:%s\n",(*pw).pw_dir);
	   		printf("Default shell:%s\n",(*pw).pw_shell);

				//Activa grupo

				if(G==1){
				gr=getgrgid((*pw).pw_gid);
				printf("Main Group Number:%d\n",(*gr).gr_gid);
	    			printf("Main Group Name:%s\n",(*gr).gr_name);

				}

			}

			//Se coge idioma de entorno

			else if(E==0&&S==0){

			idioma=getenv("LANG");
			
				if(idioma[1]=='s'){

				printf("Nombre del usuario:%s\n", (*pw).pw_gecos);
	            		printf("Id. del usuario:%d\n",(*pw).pw_uid);
	        		printf("Contraseña:%s\n",(*pw).pw_passwd);
	   			printf("Carpeta de inicio:%s\n",(*pw).pw_dir);
	   			printf("Interprete por defecto:%s\n",(*pw).pw_shell);

					//Activa grupo

					if(G==1){
					gr=getgrgid((*pw).pw_gid);
					printf("Número del grupo principal:%d\n",(*gr).gr_gid);
	    				printf("Nombre del grupo principal:%s\n",(*gr).gr_name);
				
					}
				
				}
	
				//Ingles

				else if(idioma[1]=='n'){

				printf("User name:%s\n",(*pw).pw_gecos);
	            		printf("User id:%d\n",(*pw).pw_uid);
	        		printf("Password:%s\n",(*pw).pw_passwd);
	   			printf("Home:%s\n",(*pw).pw_dir);
	   			printf("Default shell:%s\n",(*pw).pw_shell);

					//Activa grupo

					if(G==1){
					gr=getgrgid((*pw).pw_gid);
					printf("Main Group Number:%d\n",(*gr).gr_gid);
	    				printf("Main Group Name:%s\n",(*gr).gr_name);

					}

				}

			}

			//ERROR DOS IDIOMAS A LA VEZ

			else{

			printf("No se pueden activar dos idiomas al mismo tiempo\n");
			printf("Two languages can't be used at same time\n");

			}

		}

	}
	
	//UID Y LOGIN PASADOS A LA VEZ ERROR

	else{
		
	printf("El nombre de Usuario y el UID no deben ser usados al mismo tiempo\n");
	printf("UID and user name should not be used at the same time\n");

	}

return 0;
}
