/*Ejercicio 7 practica 1*/

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define clave (key_t) 232  //Obtenemos la clave

int main()
{
	system("clear");
	int i, shmid, status, childpid;
	pid_t pid;
	int n;//numero de hijos

	int *contador;//Variable que se va a incrementar

	/*Creamos la zona la memoria compartida*/
	if((shmid = shmget(clave, sizeof(int), IPC_CREAT | 0777))== -1)
	{
		printf("Error message shmget: %s\n", strerror(errno));
        exit(EXIT_FAILURE);	
	}

	/*Obtenemos un puntero que apunte a la zona de memoria*/
	contador= (int*)shmat(shmid, NULL,0);
	if(contador == NULL)
	{
		printf("Error message shmat: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	//Inicializamos el contador a cero
	*contador=0;

	/*Ejecutamos los hijos*/
	printf("Introduce el número de hijos:");
	scanf("%d", &n);
	for ( i = 0; i < n; ++i)
	{
		pid= fork();//Creamos el proceso
		if(pid==-1)//Caso de error en el fork()
		{
			perror("fork error");
            printf("errno value= %d\n", errno); 
            exit(EXIT_FAILURE); 
		}
		else if(pid==0)//Hijo
		{
			/*Incrementamos el contador*/
			*contador+=100;
			/*Liberamos la memoria*/
			if(shmdt(contador)==-1)
			{
				printf("Error message shm_dt: %s\n", strerror(errno));
				exit(EXIT_FAILURE);	
			}
			exit(EXIT_SUCCESS);
		}
	}
	/*Esperamos a que los hijos finalicen*/
	for (i = 0; i < n; ++i)
	{
		childpid=wait(&status);
		if(childpid>0)
        {
            if (WIFEXITED(status)) {
                printf("child %d exited, status=%d\n",childpid, WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("child %d killed (signal %d)\n", childpid, WTERMSIG(status));
            } else if (WIFSTOPPED(status)) {
                printf("child %d stopped (signal %d)\n", childpid, WSTOPSIG(status));
            } 
        }
		else if (childpid==-1) {
			printf("Error message wait: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
		}
	}

	/*Imprimimos el resultado final*/
	printf("La variable contador=%d\n", *contador);
	/*Liberamos memoria*/
	if(shmdt(contador)==-1)
	{
		printf("Error message parent shm_dt: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	} 
	/*Destruimos la zona de memoria utilizada*/
	if(shmctl (shmid, IPC_RMID, NULL)==-1)
	{
		printf("Error message shm_ctl: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	} 
	exit(EXIT_SUCCESS);
}