//Ejercicio 5 de la practica 1

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		printf("Error en la línea de órdenes\n");// ./a.out <nº de hijos> <nombre fichero>
		exit(-1);
	}
	else
	{
		system("clear");
		pid_t pid;
		int childpid, status;
		FILE *fich;
		fich=fopen("fichero.txt", "w");
		int i, N;
		N = atoi(argv[1]);

		for(i=0;i<N;i++)
		{
			pid=fork();
			switch(pid)
			{
				case -1:
					perror("Error en el fork");
					printf("Errno value = %d \n", errno);
					exit(EXIT_FAILURE);
				case 0:
					printf("Soy el hijo %d con padre %d \n", getpid(), getppid());
					fprintf(fich, "--------------------------------------------\n");
					sleep(4);
					exit(EXIT_SUCCESS);
			}
		}
		fprintf(fich, "++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
		while((childpid=wait(&status)) > 0)
		{
			if(childpid>0)
            {
              if(childpid>0){
                if (WIFEXITED(status)){
                      printf("child %d exited, status=%d\n",childpid, WEXITSTATUS(status));
                 }
                else if (WIFSIGNALED(status)){
                      printf("child %d killed (signal %d)\n", childpid, WTERMSIG(status));
                 }
                 else if (WIFSTOPPED(status)){
                      printf("child %d stopped (signal %d)\n", childpid, WSTOPSIG(status));
                 }
              }
              else{
                  printf("Error en la invocacion de wait o la llamada ha sido interrumpida por una señal.\n");
                  exit(EXIT_FAILURE);
              }
            }
		}
		exit(EXIT_SUCCESS);
	}
}