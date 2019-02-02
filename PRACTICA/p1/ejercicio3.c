#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <grp.h>
#include <pwd.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include <errno.h>
#include <string.h>

FILE *f=NULL;

int main(int argc, char **argv)
{
	system("clear");
	int i, tub[2], num, flag, status, errno;
	//Variables para hallar el primo
	int j, contador, z, numprimo=0;
	pid_t hijo;

	pipe(tub);
	f=fopen("dump.dat", "w");

	//Calculamos el numero total de numeros primos que contiene el numero introducido
	printf("[PADRE] Introduce el numero para hallar sus primos\n");
	scanf("%d", &num);

	for(j=1;j<=num;j++)
	{
		contador=0;
		for(z=1;z<=j;z++)
		{
			if((j%z)==0)
			{
				contador++;
			}
		}
		if(contador==2)
		{
			numprimo++;
		}
	}

	//Creamos el proceso
	hijo=fork();

	switch(hijo)
	{
		case -1:
			perror("Error en la llamada fork");
			printf("Erno value: %d\n", errno);
			fprintf(f, "Errno value %d\n", errno);
			exit(EXIT_FAILURE);
		case 0:
			//Cerramos la tuberia por la parte que se va a escribir
			close(tub[1]);
			//Imprimimos los datos del hijo
			printf("[HIJO] Soy el hijo %d y mi padre es %d\n", getpid(), getppid());
			fprintf(f, "[HIJO] Soy el hijo %d y mi padre es %d\n", getpid(), getppid());

			//Realizamos las operaciones que tiene que realizar el hijo
			for(i=0;i<numprimo;i++)
			{
				read(tub[0], &num, sizeof(int));
				printf("[HIJO] Leo el numero primo %d \n", num);
				fprintf(f, "[HIJO] Leo el numero primo %d \n", num);
				//sleep(rand()%2);
			}
			//Cerramos la tuberia por la parte de lectura
			close(tub[0]);
			printf("[HIJO] Tuberia cerrada. Salgo!\n");
			fprintf(f, "[HIJO] Tuberia cerrada. Salgo!\n");
			exit(EXIT_SUCCESS);
		default:
			//Cerramos la tuberia por la parte de lectura
			close(tub[0]);
			//Imprimimos los datos del padre
			printf("[PADRE] Soy el pid %d y mi hijo es %d\n", getpid(), hijo);
			fprintf(f, "[PADRE] Soy el pid %d y mi hijo es %d\n", getpid(), hijo);

			
			//Realizamos las operaciones que tiene que realizar el padre

			for(j=1;j<=num;j++)
			{
				contador=0;
				for(z=1;z<=j;z++)
				{
					if((j%z)==0)
					{
						contador++;
					}
				}
				if(contador==2)
				{
					write(tub[1], &j, sizeof(int));
					printf("[PADRE] Escribo el numero primo %d<%d\n", j, num);
					fprintf(f, "[PADRE] Escribo el numero primo %d<%d\n", j, num);
				}
				//sleep(rand()%2);
			}
			
			
			close(tub[1]);
			printf("[PADRE] Tuberia cerrada. Salgo!\n");
			fprintf(f, "[PADRE] Tuberia cerrada. Salgo!\n");
			//Hacemos que el padre espere a los hijos
			while((flag=wait(&status))>0)
			{
				if(WIFEXITED(status))
				{
					printf("Proceso Padre, Hijo con PID %d finalizado, status = %d\n", flag, WEXITSTATUS(status));
				} 
				else if(WIFSIGNALED(status))
				{  
					printf("Proceso Padre, Hijo con PID %d finalizado al recibir la señal %d\n", flag, WTERMSIG(status));
				} 
				else if(WIFSTOPPED(status))
				{ 
					printf("Proceso Padre, Hijo con PID %d parado al recibir la señal %d\n", flag,WSTOPSIG(status));
				} 
				else if(WIFCONTINUED(status))
				{
					printf("Proceso Padre, Hijo con PID %d reanudado\n",(int) flag);		  
				}
			}
			//Cuando no hay mas hijos por los que esperar
			if(flag==-1 && errno==ECHILD)
			{
				printf("Proceso Padre, valor de errno = %d, definido como %s, no hay más hijos que esperar!\n",errno,strerror(errno));
			}
			else
			{
				printf("Error en la invocacion de wait o la llamada ha sido interrumpida por una señal.\n");
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
	}

		
	return 0;
}