#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include "common.h"

/* @author Andrés Millán Alcaide
@date 2/3/16 */

void funcionLog(char *mensaje);
void manejador(int num_signal);//funcion manejadora de la senyal.
FILE *fLog = NULL;


//Las usamos como variables globales ya que son usadas por las funciones auxiliares.
	mqd_t mq_server;//Cola del server
	mqd_t mq_cliente;//Cola del cliente

	
	char client_queue[50];//Cadenas que sobreesciberemos el nombre de la cola + el usuario que ejecute el programa en mi caso /client-queue-i42miala
	char server_queue[50];


int main(int argc, char **argv)
{
	
	int opcion, H=0;//Opcion para el buffer.// Flag de la ayuda.
	char *num=NULL;//Numero recibido por linea de ordenes
	int tiempo;

	char buffer[MAX_SIZE];//Buffer de escritura/lectura
	char emparejador[MAX_SIZE];//LA de si empareja o no


	//Anyadimos al nombre de las colas el login del usuario que la ejecuta.
	sprintf(server_queue, "%s_%s", SERVER_QUEUE, getlogin());
	sprintf(client_queue, "%s_%s", CLIENT_QUEUE, getlogin());

	// Abrir el server, n cremaos. Permiso de escritura WRONLY
	mq_server = mq_open(server_queue, O_WRONLY);
	if(mq_server == (mqd_t)-1 ){
        	perror("Error al abrir la cola del servidor");
        	funcionLog("Error al abrir la cola del servidor");
       		exit(-1);
	}
	//Abrimos el cliente, no creamos. Permisos de lectura RDONLY
	mq_cliente = mq_open(client_queue, O_RDONLY);
	if(mq_cliente == (mqd_t)-1 ){
   		perror("Error al abrir la cola del cliente");
   		funcionLog("Error al abrir la cola del cliente");
      	exit(-1);
	}

	//Capturamos los posibles errores posibles que se den al recibir los distintos tipos de senyal.
	if(signal(SIGTERM, manejador) == SIG_ERR || signal(SIGINT, manejador) == SIG_ERR || signal(SIGHUP, manejador) == SIG_ERR || signal(SIGALRM, manejador) == SIG_ERR)
	{
		printf("Error al crear las señales.\n");
	}






    	while ((opcion = getopt (argc, argv, "t:h")) != -1)//En opcion asignamos la opcion. Los dos ptos necesariamente necesita de un argumento
    {
    	switch(opcion)
    	{
    	case 't':
    		num=optarg;//Te coge el numero de seconds
    	break;
    	
    	case 'h':
    		H=1;
    	break;
    	
    	case '?'://Podriamos utilizar syntaxError=1 pero lo imprimimos directamente porq el caracter no es reconocible.
    		printf("Uso del programa: ejercicio4-cliente [opciones]\n");
			printf("Opciones:\n");
			printf("-h, --help\t Imprimir esta ayuda\n");
			printf("-t, --time\t Cronometro de expiracion\n");
			break;

		default:
				abort();//En caso de algun problema en linea de ordenes.	
    	}
    }

    if(H==1)//Ayuda del programa activada por linea de ordenes.
    {
    		printf("Uso del programa: ejercicio4-cliente [opciones]\n");
			printf("Opciones:\n");
			printf("-h, --help\t Imprimir esta ayuda\n");
			printf("-t, --time\t Cronometro de expiracion\n");
    }



    if(num!=NULL)//Si le hemos pasado un argumento como el tiempo en segundos
    {
	
		tiempo = atoi(num);
		printf("Temporizador en %d segundos!!\n", tiempo);
		alarm(tiempo);//Alarma activada-> Se cierra el programa a los x segundos.
		
    }

    	printf("Mandando mensajes al servidor (escribir \"%s\" para parar):\n", MSG_STOP);

	do {

		
		printf("> ");
		fflush(stdout);                  // Limpiar buffer de salida
		memset(buffer, 0, MAX_SIZE);     // Poner a 0 el buffer
		fgets(buffer, MAX_SIZE, stdin);  // Leer por teclado
		buffer[strlen(buffer)-1] = '\0'; // Descartar el salto de línea
		

		
		
		if(mq_send(mq_server, buffer, MAX_SIZE, 0) != 0){//Enviar la cadena que queramos poner al server.
			perror("Error al enviar el mensaje");
			funcionLog("Error al enviar el mensaje");
			exit(-1);
		}

	
			if(mq_receive(mq_cliente, emparejador, MAX_SIZE, NULL) < 0)//Recibimos el emparejador
		{
			//Error al recibir el mensaje del servidor.
			perror("Error al recibir el mensaje.");
			funcionLog("Error al recibir el mensaje");
			exit(-1);
		}

if(strncmp(emparejador, MSG_STOP, strlen(MSG_STOP))==0)
			exit(0);

		printf("%s\n", emparejador);//Imprimimos el emparejador
		


	
	
	} while (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)));//Lo ejecuta mientras que el usuario no teclee exit que es equivalente a MSG_STOP

	
	

	if(mq_close(mq_cliente) == (mqd_t)-1){//Cerrar cola del cliente. Solo cerrar ya que eliminarla se encarga el servidor.
		perror("Error al cerrar la cola del cliente");
		funcionLog("Error al cerrar la cola del cliente");
		exit(-1);
	}

	

	if(mq_close(mq_server) == (mqd_t)-1){//Cerrar cola del server
		perror("Error al cerrar la cola del server");
		funcionLog("Error al cerrar la cola del servidor");
		exit(-1);
	}

	

	return 0;


    }








//Funcion para imprimir los errores en un txt a modo de log.

void funcionLog(char *mensaje) {
	int resultado;
	char nombreFichero[100];
	char mensajeAEscribir[300];
	time_t t;

	
	sprintf(nombreFichero,"log-cliente.txt");
	if(fLog==NULL){
		fLog = fopen(nombreFichero,"at");
		if(fLog==NULL){
			perror("Error abriendo el fichero de log");
			exit(1);
		}
	}

	
	t = time(NULL);
	struct tm * p = localtime(&t);
	strftime(mensajeAEscribir, 1000, "[%Y-%m-%d, %H:%M:%S]", p);

	
	sprintf(mensajeAEscribir, "%s ==> %s\n", mensajeAEscribir, mensaje);
	
	
	resultado = fputs(mensajeAEscribir,fLog);
	if ( resultado < 0)
		perror("Error escribiendo en el fichero de log");

	fclose(fLog);
	fLog=NULL;
}


void manejador(int num_signal)
{
	char msg[50];

	//Escribimos en el fichero la senyal recibida.	
	sprintf(msg,"Recibida la senyal=%d\n", num_signal );
	printf("%s\n", msg);
	funcionLog(msg);

	//Cerramos la colas pertinentes
	if(mq_send(mq_server, MSG_STOP, MAX_SIZE,20)!=0)
	{
		perror("Error al enviar la senyal de deteccion del programa");
		funcionLog("Error al enviar la senyal de deteccion del programa");
	}

	if(mq_close(mq_server) == (mqd_t)-1){//Cerrar cola del server
		perror("Error al cerrar la cola del servidor");
		funcionLog("Error al cerrar la cola del servidor");
		exit(-1);
	}


if(mq_close(mq_cliente) == (mqd_t)-1){//Cerrar cola del cleinte
		perror("Error al cerrar la cola del cliente");
		funcionLog("Error al cerrar la cola del cliente");
		exit(-1);
	}



	exit(0);
}