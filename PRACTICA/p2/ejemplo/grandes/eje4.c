//Ejercicio 4 Practica 1
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv)
{
	//Ejecución “./miPrograma gnome-calculator gedit fichero1.txt fichero2.txt ficheroN.txt”
	system("clear");
		pid_t pid, pid2;
		int childpid, status;
		pid = fork();
		if(pid == -1)
		{
			perror("Error en el fork.");
			printf("Errno value = %d\n", errno);
			exit(EXIT_FAILURE);
		}
		if(pid==0)
		{
			printf("Soy el hijo %d y mi padre es %d \n", getpid(), getppid());
			if(execlp(argv[1], argv[1], NULL)==-1)
			{
				perror("Fallo en la ejecución");
				printf("errno value=%d\n", errno);
				exit(EXIT_FAILURE);
			}
		}
		pid2=fork();
		if(pid2 == -1)
		{
			perror("Error en el fork.");
			printf("Errno value = %d\n", errno);
			exit(EXIT_FAILURE);
		}
		if(pid2 == 0)
		{	
			printf("Soy el hijo %d y mi padre es %d\n", getpid(), getppid());
			if(execvp(argv[2], &argv[2])<0)
			{
				perror("Error en el fork.");
				printf("Errno value = %d\n", errno);
				exit(EXIT_FAILURE);
			}
		}

		while(pid=wait(&status)!= -1)
		{
			//Control de errores
			printf("EL hijo ha terminado %d ha terminado, status=%d\n",childpid, WEXITSTATUS(status));
			 if (WIFEXITED(status)) 
					{
                    printf("child %d exited, status=%d\n",childpid, WEXITSTATUS(status));
               } 
					else if (WIFSIGNALED(status)) 
					{
                    printf("child %d killed (signal %d)\n", childpid, WTERMSIG(status));
               } 
					else if (WIFSTOPPED(status)) 
					{
                    printf("child %d stopped (signal %d)\n", childpid, WSTOPSIG(status));
               }
		}
		exit(EXIT_SUCCESS);
	
}