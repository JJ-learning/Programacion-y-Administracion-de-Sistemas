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
#include <getopt.h>

#include "common.h"

//Variables Globales
mqd_t mq_servidor;
mqd_t mq_cliente;
FILE *f_log=NULL;
char cola_servidor[120];
char cola_cliente[120];

void funcionLog(char * mensaje) {
	
	int resultado;
	char nombreF[100];
	char cadena[300];
	time_t tiempo;

	sprintf(nombreF,"log-servidor.txt");
	if(f_log==NULL)
	{
		f_log=fopen(nombreF,"at");
		if(f_log==NULL)
		{
			perror("Error abriendo el fichero de log");
			exit(1);
		}
	}

	tiempo=time(NULL);
	struct tm * p=localtime(&tiempo);
	strftime(cadena, 1000, "[%Y-%m-%d, %H:%M:%S]",p);

	sprintf(cadena,"%s ==> %s\n",cadena,mensaje);
	
	resultado=fputs(cadena,f_log);
	if (resultado<0)
	{
		perror("Error escribiendo en el fichero de log");
	}

	fclose(f_log);
	f_log=NULL;
}

void controlador(int num_signal)
{

	char mensaje[50];

	sprintf(mensaje,"Recibida la signal=%d\n",num_signal );
	printf("%s\n",mensaje);
	funcionLog(mensaje);

	if(mq_send(mq_cliente,MSG_STOP,MAX_SIZE,20)!=0)
	{
		perror("Error al enviar la signal de deteccion del programa");
		funcionLog("Error al enviar la signal de deteccion del programa");
	}

	//Cerramos la cola del servidor
	if(mq_close(mq_servidor)==(mqd_t)-1)
	{
		perror("Error al cerrar la cola del servidor");
		funcionLog("Error al cerrar la cola del servidor");
		exit(-1);
	}
	//Cerramos la cola del cliente
	if(mq_close(mq_cliente)==(mqd_t)-1)
	{
		perror("Error al cerrar la cola del cliente");
		funcionLog("Error al cerrar la cola del cliente");
		exit(-1);
	}

exit(0);
}

int main(int argc, char **argv)
{
	int o, time, option_index=0, error=0;
	int hflag=0;
	char *_argv=NULL, mensaje[MAX_SIZE], empareja[MAX_SIZE], log_msg[MAX_SIZE];

	struct option l_option[]=
	{
		{"time", required_argument, 0, 't'}, 
		{"help", no_argument, 0, 'h'}, 

		{0,0,0,0}
	};

	sprintf(cola_servidor, "%s_%s", SERVER_QUEUE, getlogin());
	sprintf(cola_cliente, "%s_%s", CLIENT_QUEUE, getlogin());

	//Abrimos la cola del servidor
	mq_servidor = mq_open(cola_servidor, O_WRONLY);
	if(mq_servidor == (mqd_t)-1)
	{
		perror("Error al abrir la cola del servidor");
		funcionLog("Error al abrir la cola del servidor");
		exit(-1);
	}
	//Abrimos la cola del cliente
	mq_cliente=mq_open(cola_cliente, O_RDONLY);
	if(mq_cliente == (mqd_t)-1)
	{
		perror("Error al abrir la cola del cliente");
		funcionLog("Error al abrir la cola del cliente");
		exit(-1);
	}

	//Recogemos las seniales
	if(signal(SIGTERM, controlador) == SIG_ERR || signal(SIGINT, controlador) == SIG_ERR || signal(SIGHUP, controlador) == SIG_ERR)
	{
		perror("Error en la recepcion de la senial");
		funcionLog("Error en la recepcion de la senial");
	}


	while((o = getopt_long(argc, argv, "t:h", l_option, &option_index)) != -1)
	{
		switch(o)
		{
			case 't':
				//Recogemos el numero de segundos
				_argv=optarg;
				break;
			case 'h':
				hflag=1;
				break;
			case '?':
				error=1;
				break;
			default:
				abort();
		}
	}

	if(hflag==1 || error==1)
	{
		printf("Uso del programa: ./ejercicio4-servidor [opciones]\n");
		printf("Opciones\n");
		printf("-h, --help, \t Imprimir la ayuda\n");
		printf("-t, --time \t Cronometro de expiracion\n");
	}

	if(_argv != NULL)
	{
		time=atoi(_argv);
		printf("Temporizador en %d s.\n", time);
		sprintf(log_msg, "Temporizador en %d s.\n", time);
		funcionLog(log_msg);
		alarm(time);
	}
	printf("Mandando mensaje al servidor (Teclear \"%s\" para salir)\n", MSG_STOP);

	do
	{
		printf("> ");
		//Limpiamos el buffer
		fflush(stdout);
		//Ponemos a 0 el buffer
		memset(mensaje, 0, MAX_SIZE);
		//Leemos por teclado
		fgets(mensaje, MAX_SIZE, stdin);
		//Descartamos el \n
		mensaje[strlen(mensaje)-1]='\0';

		sprintf(log_msg, "Mensaje escrito: %s", mensaje);
		funcionLog(log_msg);

		//Enviamos el mensaje al servidor
		if(mq_send(mq_servidor, mensaje, MAX_SIZE, 0) != 0)
		{
			perror("Error al enviar el mensaje");
			funcionLog("Error al enviar el mensaje");
			exit(-1);	
		}

		//Recibimos el mensaje del servidor
		if(mq_receive(mq_cliente, empareja, MAX_SIZE, NULL) < 0)
		{
			perror("Error al recibir el mensaje.");
			funcionLog("Error al recibir el mensaje");
			exit(-1);
		}

		sprintf(log_msg, "Mensaje escrito: %s\n", empareja);
		funcionLog(log_msg);

		//Comparamos por si es un mensaje para parar la ejecucion
		if(strncmp(empareja, MSG_STOP, strlen(MSG_STOP)) == 0)
		{
			exit(-1);
		}

		printf("%s\n", empareja);
	}while(strncmp(mensaje, MSG_STOP, strlen(MSG_STOP)));

	//Cerramos la cola del cliente
	if(mq_close(mq_cliente)==(mqd_t)-1){
		perror("Error al cerrar la cola del cliente");
		funcionLog("Error al cerrar la cola del cliente");
		exit(-1);
	}
	//Cerramos la cola del servidor
	if(mq_close(mq_servidor)==(mqd_t)-1){
		perror("Error al cerrar la cola del server");
		funcionLog("Error al cerrar la cola del servidor");
		exit(-1);
	}

	return 0;
}