#include <stdio.h>
#include <grp.h>
#include <pwd.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>

FILE * f=NULL;

int main(int argc,char * argv[]){

	int i,v[2],num,random_num,flag,status;
	pid_t hijo;
	
	pipe(v);

	f=fopen("ejercicio3.txt","w");

	hijo=fork();

	switch(hijo){

		case -1:

			printf("Error al crear el hijo.\n");
			fprintf(f,"Error al crear el hijo.\n");
			exit(EXIT_FAILURE);

		case 0:
	
			close(v[1]);

			printf("[HIJO]:Mi PID es %d y mi PPID es %d\n",getpid(),getppid());
			fprintf(f,"[HIJO]:Mi PID es %d y mi PPID es %d\n",getpid(),getppid());
			
			for (i=0;i<5;i++){
				read(v[0],&num,sizeof(int));
				printf("[HIJO]:Leemos el número aleatorio %d en la tubería.\n",num);
				fprintf(f,"[HIJO]:Leemos el número aleatorio %d en la tubería.\n",num);
				sleep(rand()%2);
			}

			close(v[0]);							
			printf("[HIJO]:Tubería cerrada.Salgo!\n");
			fprintf(f,"[HIJO]:Tubería cerrada.Salgo!\n");
			exit(EXIT_SUCCESS);

		default:
			
			close(v[0]);							

			printf("[PADRE]:Mi PID es %d y el PID de mi hijo es %d\n",getpid(),hijo);
			fprintf(f,"[PADRE]:Mi PID es %d y el PID de mi hijo es %d\n",getpid(),hijo);
			
			srand(time(NULL));							

			for(i=0;i<5;i++){
				random_num=rand()%5000;					
				write(v[1],&random_num, sizeof(int));
				printf("[PADRE]:Escribimos el número aleatorio %d en la tubería.\n",random_num);
				fprintf(f,"[PADRE]:Escribimos el número aleatorio %d en la tubería.\n",random_num);
				sleep(rand()%2);

			}

			close(v[1]);							
			printf("[PADRE]:Tubería cerrada.Salgo!\n");
			fprintf(f,"[PADRE]:Tubería cerrada.Salgo!\n");
			exit(EXIT_SUCCESS);

	}

	/*Espera del padre a los hijos*/
	while ((flag=wait(&status))>0){

		if(WIFEXITED(status)){
			printf("Proceso Padre, Hijo con PID %d finalizado, status = %d\n", flag, WEXITSTATUS(status));
		} else if(WIFSIGNALED(status)){  //Para seniales como las de finalizar o matar
			printf("Proceso Padre, Hijo con PID %d finalizado al recibir la señal %d\n", flag, WTERMSIG(status));
		} else if(WIFSTOPPED(status)){ //Para cuando se para un proceso. Al usar wait() en vez de waitpid() no nos sirve.
			printf("Proceso Padre, Hijo con PID %d parado al recibir la señal %d\n", flag,WSTOPSIG(status));
		} else if(WIFCONTINUED(status)){ //Para cuando se reanuda un proceso parado. Al usar wait() en vez de waitpid() no nos sirve.
			printf("Proceso Padre, Hijo con PID %d reanudado\n",(int) flag);		  
		}
	}
	if(flag==-1&&errno==ECHILD)
	{
		printf("Proceso Padre, valor de errno = %d, definido como %s, no hay más hijos que esperar!\n",errno,strerror(errno));
	}
	else
	{
		printf("Error en la invocacion de wait o la llamada ha sido interrumpida por una señal.\n");
		exit(EXIT_FAILURE);
	}

return 0;
}
