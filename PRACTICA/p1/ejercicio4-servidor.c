#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <time.h>
#include <errno.h>
#include <regex.h>
#include <getopt.h>
#include <unistd.h>
#include <signal.h>

#include "common.h"

//Variables Globales
mqd_t mq_servidor;//Cola del servidor
mqd_t mq_cliente;//Cola del cliente
FILE *f_log=NULL;
//Variables para el mensaje a enviar
char cola_cliente[120];
char cola_servidor[120];

void funcionLog(char *mensaje)
{
	int resultado;
	char nombreF[100];
	char cadena[200];
	time_t tiempo;

	sprintf(nombreF, "log-servidor.txt");
	if(f_log == NULL)
	{
		f_log=fopen(nombreF, "at");
		if(f_log == NULL)
		{
			perror("Error al abrir el fichero de log");
			exit(-1);
		}
	}

	tiempo=time(NULL);
	struct tm *p=localtime(&tiempo);
	
	strftime(cadena, 1000, "[%Y-%m-%d, %H:%M:%S]", p);
	sprintf(cadena, "%s --> %s\n", cadena, mensaje);

	resultado=fputs(cadena, f_log);
	if(resultado<0)
	{
		perror("Error al escribir en el fichero de log");
	}

	//Cerramos el fichero y lo volvemos a inicializar
	fclose(f_log);
	f_log=NULL;
}

void controlador(int num_signal)
{
	char mensaje[50];

	sprintf(mensaje, "Recibo la senial: %d", num_signal);
	printf("%s\n", mensaje);
	funcionLog(mensaje);

	//Enviamos la senial al cliente
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
	if(mq_unlink(cola_servidor)==(mqd_t)-1)
	{
		perror("Error al eliminar la cola del servidor");
		funcionLog("Error al eliminar la cola del servidor");
		exit(-1);
	}
	//Cerramos la cola del cliente
	if(mq_close(mq_cliente)==(mqd_t)-1)
	{
		perror("Error al cerrar la cola del cliente");
		funcionLog("Error al cerrar la cola del cliente");
		exit(-1);
	}	
	if(mq_unlink(cola_cliente)==(mqd_t)-1)
	{
		perror("Error al eliminar la cola del cliente");
		funcionLog("Error al eliminar la cola del cliente");
		exit(-1);
	}

	exit(0);
}

int main(int argc, char**argv)
{
	struct mq_attr atributo;
	regex_t expresion;
	int reg_error, must_stop=0, option_index=0, o, error=0;
	int hflag=0;
	char cad[MAX_SIZE+1], *reg=NULL, empareja[MAX_SIZE], mensaje_cad[120], log_msg[MAX_SIZE];//Ponemos +1 para controlar el \n

	//Opciones
	struct option l_option[]=
	{
		{"regex", required_argument, 0, 'r'},
		{"help", no_argument, 0, 'h'},

		{0,0,0,0}
	};

	atributo.mq_maxmsg=10;
	atributo.mq_msgsize=MAX_SIZE;

	while((o=getopt_long(argc, argv, "r:h", l_option, &option_index))!=-1)
	{
		switch(o)
		{
			case 'r':
				reg = optarg;
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
		printf("-r, --regex \t Palabra para emparejar\n");
	}

	sprintf(cola_servidor, "%s_%s", SERVER_QUEUE, getlogin());
	sprintf(cola_cliente, "%s_%s", CLIENT_QUEUE, getlogin());

	//Abrimos la cola del servidor
	mq_servidor=mq_open(cola_servidor, O_CREAT|O_RDONLY, 0644, &atributo);
	if(mq_servidor == (mqd_t)-1)
	{
		perror("Error al abrir la cola del servidor");
		funcionLog("Error al abrir la cola del servidor");
		exit(-1);
	}

	//Abrimos la cola del cliente
	mq_cliente=mq_open(cola_cliente, O_CREAT|O_WRONLY, 0644, &atributo);
	if(mq_cliente == (mqd_t)-1)
	{
		perror("Error al abrir la cola del cliente");
		funcionLog("Error al abrir la cola del cliente");
		exit(-1);
	}

	if(signal(SIGTERM, controlador) == SIG_ERR || signal(SIGINT, controlador) == SIG_ERR || signal(SIGHUP, controlador) == SIG_ERR)
	{
		perror("Error en la recepcion de la senial");
		funcionLog("Error en la recepcion de la senial");
	}

	//Comparamos con lo que hemos obtenido por la linea de ordenes
	if(reg != NULL)
	{
		//Compilamos la expresion regular
		reg_error=regcomp(&expresion, reg, 0);
		//Comprobamos en caso de error
		if(reg_error)
		{
			sprintf(mensaje_cad, "No se pudo crear la expresion regular");
			perror("No se pudo crear la expresion regular");
			funcionLog(mensaje_cad);

			//Enviamos el mensaje de parada
			if(mq_send(mq_servidor, MSG_STOP, MAX_SIZE, 0) != 0)
			{
				perror("Error al enviar el mensaje de parada");
				funcionLog("Error al enviar el mensaje de parada");
			}
			//Salimos del programa
			exit(-1);
		}
	}
	else
	{
		printf("No existe expresion regular\n");
		funcionLog("No existe expresion regular");
		exit(-1);
	}

	do
	{
		ssize_t bytes_leidos;//Numero de bytes que se leeran del mq_receive

		bytes_leidos = mq_receive(mq_servidor, cad, MAX_SIZE, NULL);
		if(bytes_leidos < 0)
		{
			perror("Error al recibir el mensaje");
			funcionLog("Error al recibir el mensaje");
			exit(-1);
		}

		//Hacemos esto para quitar el \n y poner un \0
		cad[bytes_leidos]='\0';

		//Comparamos la cadena recibida con el regex
		reg_error = regexec(&expresion, cad, 0, NULL, 0);
		if(!reg_error)
		{
			sprintf(empareja, "Empareja");
			//Enviamos el mensaje
			if(mq_send(mq_cliente, empareja, MAX_SIZE, 0) != 0)
			{
				perror("Error al enviar el mensaje");
				funcionLog("Error al enviar el mensaje");
				exit(-1);
			}
			sprintf(log_msg, "Mensaje escrito: %s", empareja);
			funcionLog(log_msg);
		}
		else if(reg_error != 0)
		{
			sprintf(empareja, "No Empareja");
			//Enviamos el mensaje
			if(mq_send(mq_cliente, empareja, MAX_SIZE, 0) != 0)
			{
				perror("Error al enviar el mensaje");
				funcionLog("Error al enviar el mensaje");
				exit(-1);
			}
			sprintf(log_msg, "Mensaje escrito: %s", empareja);
			funcionLog(log_msg);
		}
		else
		{
			regerror(reg_error, &expresion, empareja, sizeof(empareja));
			fprintf(stderr, "Error al evaluar la expresion regular: %s\n", empareja);
			funcionLog("Error al evaluar la expresion regular");
			exit(-1);
		}

		if(strncmp(cad, MSG_STOP, strlen(MSG_STOP)) == 0)
		{
			must_stop=1;
		}
		else
		{
			printf("Mensaje recibido: %s\n", cad);
			sprintf(log_msg, "Mensaje recibido: %s\n", cad);
			funcionLog(cad);
		}
	}while(!must_stop);

	//Cerramos la cola del servidor
	if(mq_close(mq_servidor)==(mqd_t)-1){
		perror("Error al cerrar la cola del servidor");
		funcionLog("Error al cerrar la cola del servidor");
		exit(-1);
	}	
	if(mq_unlink(cola_servidor)==(mqd_t)-1){
		perror("Error al eliminar la cola del servidor");
		funcionLog("Error al eliminar la cola del servidor");
		exit(-1);
	}
	//Cerramos la cola del cliente
	if(mq_close(mq_cliente)==(mqd_t)-1){
		perror("Error al cerrar la cola del cliente");
		funcionLog("Error al cerrar la cola del cliente");
		exit(-1);
	}	
	if(mq_unlink(cola_cliente)==(mqd_t)-1){
		perror("Error al eliminar la cola del cliente");
		funcionLog("Error al eliminar la cola del cliente");
		exit(-1);
	}

	return 0;
}	