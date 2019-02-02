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

mqd_t mq_server;
mqd_t mq_client;
FILE * f_log=NULL;
char cola_cliente[100];
char cola_servidor[100];

void funcionLog(char * mensaje) {
	
	int resultado;
	char nombreFichero[100];
	char cadena[300];
	time_t t;

	
	sprintf(nombreFichero,"log-servidor.txt");
	if(f_log==NULL){
		f_log=fopen(nombreFichero,"at");
		if(f_log==NULL){
			perror("Error abriendo el fichero de log");
			exit(1);
		}
	}

	t=time(NULL);
	struct tm * p=localtime(&t);
	strftime(cadena, 1000, "[%Y-%m-%d, %H:%M:%S]",p);

	sprintf(cadena,"%s ==> %s\n",cadena,mensaje);
	
	resultado=fputs(cadena,f_log);
	if (resultado<0){

		perror("Error escribiendo en el fichero de log");

	}

	fclose(f_log);
	f_log=NULL;
}

void controlador(int num_signal){

	char mensaje[50];

	sprintf(mensaje,"Recibida la signal=%d\n",num_signal );
	printf("%s\n",mensaje);
	funcionLog(mensaje);

	if(mq_send(mq_client,MSG_STOP,MAX_SIZE,20)!=0){
		perror("Error al enviar la signal de deteccion del programa");
		funcionLog("Error al enviar la signal de deteccion del programa");
	}

	if(mq_close(mq_server)==(mqd_t)-1){//Cerrar cola del servidor
		perror("Error al cerrar la cola del servidor");
		funcionLog("Error al cerrar la cola del servidor");
		exit(-1);
	}

	if(mq_close(mq_client)==(mqd_t)-1){//Cerrar cola del cliente
		perror("Error al cerrar la cola del cliente");
		funcionLog("Error al cerrar la cola del cliente");
		exit(-1);
	}

exit(0);
}


int main(int argc, char * argv[]){
	
	int o,H=0,time,option_index=0;
	char *_argv=NULL,mensaje[MAX_SIZE],empareja[MAX_SIZE],log_msg[MAX_SIZE];
		
	struct option l_option[]={

		{"time",required_argument,0,'t'},
		{"help",no_argument,0,'h'},
		
		{0,0,0,0}
	
	};
	
	sprintf(cola_servidor,"%s_%s",SERVER_QUEUE,getlogin());
	sprintf(cola_cliente,"%s_%s",CLIENT_QUEUE,getlogin());

	mq_server=mq_open(cola_servidor,O_WRONLY);
	if(mq_server==(mqd_t)-1){
        	perror("Error al abrir la cola del servidor");
        	funcionLog("Error al abrir la cola del servidor");
       		exit(-1);
	}
	
	mq_client=mq_open(cola_cliente,O_RDONLY);
	if(mq_client==(mqd_t)-1){
   		perror("Error al abrir la cola del cliente");
   		funcionLog("Error al abrir la cola del cliente");
      		exit(-1);
	}

	if(signal(SIGTERM,controlador)==SIG_ERR||signal(SIGINT,controlador)==SIG_ERR||signal(SIGHUP,controlador)==SIG_ERR||signal(SIGALRM,controlador)==SIG_ERR){
		printf("Error al crear las señales.\n");
		funcionLog("Error al crear las señales.");
	}

    	while((o=getopt_long(argc,argv,"t:h",l_option,&option_index))!=-1){

    		switch(o){
    			case 't':
    			_argv=optarg;//Te coge el numero de segundos
    			break;
    			case 'h':
    			H=1;
    			break;
    			case '?':
	    		printf("Uso del programa: ejercicio4-cliente [opciones]\n");
			printf("Opciones:\n");
			printf("-h,--help\t Imprimir esta ayuda\n");
			printf("-t,--time\t Cronometro de expiracion\n");
			break;
			default:
				abort();	
    		}
    	}

   	if(H==1){
    		printf("Uso del programa: ejercicio4-cliente [opciones]\n");
		printf("Opciones:\n");
		printf("-h,--help\t Imprimir esta ayuda\n");
		printf("-t,--time\t Cronometro de expiracion\n");
    	}

    	if(_argv!=NULL){
		time=atoi(_argv);
		printf("¡Temporizador en %d segundos!\n",time);
		sprintf(log_msg,"¡Temporizador en %d segundos!\n",time);
		funcionLog(log_msg);
		alarm(time);
   	}

    	printf("Mandando mensajes al servidor (escribir \"%s\" para parar):\n",MSG_STOP);

	do{
		printf("> ");
		fflush(stdout);                  // Limpiar buffer de salida
		memset(mensaje,0,MAX_SIZE);      // Poner a 0 el buffer
		fgets(mensaje,MAX_SIZE,stdin);   // Leer por teclado
		mensaje[strlen(mensaje)-1]='\0'; // Descartar el salto de línea
		
		sprintf(log_msg,"Mensaje escrito:%s\n",mensaje);
		funcionLog(log_msg);

		if(mq_send(mq_server,mensaje,MAX_SIZE,0)!=0){
			perror("Error al enviar el mensaje");
			funcionLog("Error al enviar el mensaje");
			exit(-1);
		}

		if(mq_receive(mq_client,empareja,MAX_SIZE,NULL)<0){
			perror("Error al recibir el mensaje.");
			funcionLog("Error al recibir el mensaje");
			exit(-1);
		}

		sprintf(log_msg,"Mensaje escrito:%s\n",empareja);
		funcionLog(log_msg);

		if(strncmp(empareja,MSG_STOP,strlen(MSG_STOP))==0){
			exit(0);
		}
		
		printf("%s\n",empareja);

	}while(strncmp(mensaje,MSG_STOP,strlen(MSG_STOP)));//Lo ejecuta mientras que el usuario no introduzca exit que es como introducir MSG_STOP

	if(mq_close(mq_client)==(mqd_t)-1){
		perror("Error al cerrar la cola del cliente");
		funcionLog("Error al cerrar la cola del cliente");
		exit(-1);
	}

	if(mq_close(mq_server)==(mqd_t)-1){
		perror("Error al cerrar la cola del server");
		funcionLog("Error al cerrar la cola del servidor");
		exit(-1);
	}

return 0;
}
