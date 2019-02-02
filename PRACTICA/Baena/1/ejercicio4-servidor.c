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

	
	if(mq_unlink(cola_servidor)==(mqd_t)-1){//Eliminar cola del servidor
		perror("Error al eliminar la cola del servidor");
		funcionLog("Error al eliminar la cola del servidor");
		exit(-1);
	}

	if(mq_close(mq_client)==(mqd_t)-1){//Cerrar cola del cliente
		perror("Error al cerrar la cola del cliente");
		funcionLog("Error al cerrar la cola del cliente");
		exit(-1);
	}

	
	if(mq_unlink(cola_cliente)==(mqd_t)-1){//Eliminar cola del cliente
		perror("Error al eliminar la cola del cliente");
		funcionLog("Error al eliminar la cola del cliente");
		exit(-1);
	}
	

	exit(0);
}

int main(int argc,char * argv[]){

	
	struct mq_attr atributo;
	
	regex_t expresion;

	int reg_error,must_stop=0,option_index=0,H=0,o,error=0;

	char cad[MAX_SIZE+1],* reg=NULL,empareja[MAX_SIZE],mensaje_cad[100],log_msg[MAX_SIZE];//Por el \n

	struct option l_option[]={

		{"regex",required_argument,0,'r'},
		{"help",no_argument,0,'h'},

		{0,0,0,0}

	};

	atributo.mq_maxmsg=10;        
	atributo.mq_msgsize=MAX_SIZE; 

	while((o=getopt_long(argc,argv,"r:h",l_option,&option_index))!=-1){

		switch(o){
		
			case 'r':
			reg=optarg;
			break;
			case 'h':
			H=1;
			break;
			case '?':
			error=1;
			break;
			default:
				abort();

		}

	}

	if(H==1||error==1){
		printf("Uso del programa: ejercicio4-servidor [opciones]\n");
		printf("Opciones:\n");
		printf("-h, --help,\t Imprimir esta ayuda\n");
		printf("-t, --time,\t Cronometro de expiracion\n");
	}

	sprintf(cola_servidor,"%s_%s",SERVER_QUEUE,getlogin());
	sprintf(cola_cliente,"%s_%s",CLIENT_QUEUE,getlogin());
	
	mq_server=mq_open(cola_servidor,O_CREAT|O_RDONLY,0644,&atributo);
	
	if(mq_server==(mqd_t)-1){
        	perror("Error al abrir la cola del servidor");
		funcionLog("Error al abrir la cola del servidor");
       		exit(-1);
	}

	mq_client=mq_open(cola_cliente,O_CREAT|O_WRONLY,0644,&atributo);

	if(mq_client==(mqd_t)-1){
        	perror("Error al abrir la cola del cliente");
        	funcionLog("Error al abrir la cola del cliente");
       		exit(-1);
	}

	if(signal(SIGTERM,controlador)==SIG_ERR||signal(SIGINT,controlador)==SIG_ERR||signal(SIGHUP,controlador)==SIG_ERR){
		perror("Error en la recepcion de la signal");
		funcionLog("Error en la recepcion de la signal");
	}

	if(reg!=NULL)//Comparamos con lo obtenido de linea de ordenes
	{
		// Compilar la expresion regular. Volcar la cadena a la estructura regex 
        reg_error=regcomp(&expresion,reg,0);
        if(reg_error)//Comprobacion de errores.
    	{ 
    		sprintf(mensaje_cad,"No se pudo crear la expresion regular");
    		perror("No se pudo crear la expresion regular.\n"); 
    		funcionLog(mensaje_cad);//Ejemplo de implentacion de mensajes mas complejos con el uso del sprintf. 

    		if(mq_send(mq_server,MSG_STOP,MAX_SIZE,0)!=0){
    			perror("Error al enviar el mensaje de parada.");
    			funcionLog("Error al enviar el mensaje de parada");
    		}

    		exit(-1); 
    	}
	}
    	else{
		printf("No existe expresion regular.No la ha incluido\n");
		funcionLog("No existe expresion regular.No la ha incluido");
		exit(-1);
	}
    	
	do {
		
		ssize_t bytes_read;//Bytes que se leeran del mq_receive

		bytes_read=mq_receive(mq_server,cad,MAX_SIZE,NULL);
		
		if(bytes_read<0){
			perror("Error al recibir el mensaje");
			funcionLog("Error al recibir el mensaje");
			exit(-1);
		}
        	
		cad[bytes_read]='\0';//Sustituimos el \n por \0 a la cadena

		reg_error=regexec(&expresion,cad,0,NULL,0);
	
	        if(!reg_error){
    		sprintf(empareja,"Empareja");

          	if(mq_send(mq_client,empareja,MAX_SIZE, 0)!=0){
		perror("Error al enviar el mensaje");
		funcionLog("Error al enviar el mensaje");
		exit(-1);
		}
		sprintf(log_msg,"Mensaje escrito:%s\n",empareja);
		funcionLog(log_msg);
        	}	
        	else if(reg_error!=0){
    		
        	sprintf(empareja,"No Empareja");

		if(mq_send(mq_client,empareja,MAX_SIZE,0)!=0){
		perror("Error al enviar el mensaje");
		funcionLog("Error al enviar el mensaje");
		exit(-1);
		}
		sprintf(log_msg, "Mensaje escrito:%s\n",empareja);
		funcionLog(log_msg);
        	}

       		else{
            	regerror(reg_error,&expresion,empareja,sizeof(empareja));
            	fprintf(stderr,"Error al evaluar la expresion regular:%s\n",empareja);
            	funcionLog("Error al evaluar la expresion regular");
            	exit(1);
       		}
		
		if(strncmp(cad,MSG_STOP,strlen(MSG_STOP))==0){
		must_stop=1;
		}
		else{
		printf("Recibido el mensaje:%s\n",cad);
		sprintf(log_msg,"Mensaje escrito :%s\n",cad);
		funcionLog(cad);
		}			
	
	}while(!must_stop);

	
	if(mq_close(mq_server)==(mqd_t)-1){
		perror("Error al cerrar la cola del servidor");
		funcionLog("Error al cerrar la cola del servidor");
		exit(-1);
	}

	
	if(mq_unlink(cola_servidor)==(mqd_t)-1){
		perror("Error al eliminar la cola del servidor");
		funcionLog("Error al eliminar la cola del servidor");
		exit(-1);
	}

	if(mq_close(mq_client)==(mqd_t)-1){
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
