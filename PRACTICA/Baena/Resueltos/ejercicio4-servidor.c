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

/* @author Andrés Millán Alcaide
@date 2/3/16 */

void funcionLog(char *mensaje);
void manejador(int num_signal);
FILE *fLog = NULL;
//Las usamos como variables globales ya que son usadas por las funciones auxiliares.
mqd_t mq_server;//Crear la cola del servidor
mqd_t mq_cliente;//Para mandar al cliente el Empareja o no Empareja.

char client_queue[50];//Cadenas que sobreesciberemos el nombre de la cola + el usuario que ejecute el programa en mi caso /client-queue-i42miala
char server_queue[50];

int main(int argc, char **argv)
{
	FILE *fLog = NULL;


	struct option lopt[]={

		{"regex",required_argument,0,'r'},
		{"help",no_argument,0,'h'},
		//Creamos las dependencias con los diferentes parametros que se le pasan.

		{0,0,0,0}
	
	};
	
	
	char *rex=NULL;
	struct mq_attr attr;//Crear estructura de atributos. Num max de mensajes y tam max de mensajes. Configuracion de la cola.
	
	regex_t regex;//Variable para ver si empareja
        int reti;//Igual que la de arriba.

      char emparejador[MAX_SIZE];//Para enviar el mensaje de emparejar.
	
	char buffer[MAX_SIZE + 1];//Buffer para leer los mensajes. Contemplamos el /n con el MAX_SIZE+1. Aqui intercambiamos los mensajes.
	char msgBuff[100];//Cadena para indicar los mensajes mas completos.
	
	int must_stop = 0;//Flag del exit.
	int opt, H=0, option_index=0;//Flags y opciones
	int error = 0;//Flag que nos dice cuando el usuario ha introducido una opcion no identificada.


	
	attr.mq_maxmsg = 10;    //Numero maximo de mensajes permitidos    
	attr.mq_msgsize = MAX_SIZE; //Tamanyo maximo de los mensajes


	while((opt=getopt_long(argc,argv,"r:h",lopt,&option_index))!=-1){//Activamos las banderas segun la opcion indicada

		switch(opt){	

			case 'r':
			rex=optarg;
			break;

			case 'h':
			H=1;
			break;

			case '?':
					error=1;// opcion erronea no identificada
					break;

			default:
					abort();		
			}
	}
			if(H==1 || error)//Si se ha puesto la opcion de help o se ha puesto una opcion no identificada.
			{
				printf("Uso del programa: ejercicio4-servidor [opciones]\n");
				printf("Opciones:\n");
				printf("-h, --help,\t Imprimir esta ayuda\n");
				printf("-t, --time,\t Cronometro de expiracion\n");

			}
	
	sprintf(server_queue, "%s_%s", SERVER_QUEUE, getlogin());
	sprintf(client_queue, "%s_%s", CLIENT_QUEUE, getlogin());

	mq_server = mq_open(server_queue, O_CREAT | O_RDONLY, 0644, &attr);//abrir la cola del servidor.Importante: le otorgamos permiso lectura RDONLY.			
	if(mq_server == (mqd_t)-1 ){
        	perror("Error al abrir la cola del servidor");
        	funcionLog("Error al abrir la cola del servidor");
       		exit(-1);
	}

	mq_cliente = mq_open(client_queue, O_CREAT | O_WRONLY, 0644, &attr);//abrir la cola del cliente.//Importante: le otorgamos permiso escritura WRONLY				
	if(mq_cliente == (mqd_t)-1 ){
        	perror("Error al abrir la cola del cliente");
        	funcionLog("Error al abrir la cola del cliente");
       		exit(-1);
	}

//Capturamos los posibles errores posibles que se den al recibir los distintos tipos de senyal.
	if(signal(SIGTERM,manejador)==SIG_ERR || signal(SIGINT, manejador)==SIG_ERR || signal(SIGHUP,manejador)==SIG_ERR)
	{
		perror("Error en la recepcion de la senyal");
		funcionLog("Error en la recepcion de la senyal");
	}


	//Creamos la expresion regular.
	if(rex != NULL)//Comparamos con lo obtenido de linea de ordenes
	{
		// Compilar la expresion regular 
        reti = regcomp(&regex, rex, 0);
        if( reti )//Comprobacion de errores.
    	{ 
    		sprintf(msgBuff,"No se pudo crear la expresion regular");
    		perror("No se pudo crear la expresion regular.\n"); 
    		funcionLog(msgBuff);//Ejemplo de implentacion de mensajes mas complejos con el uso del sprintf. 

    		//Enviamos el mensaje de parada a la cola del servidor para que el cliente acabe su ejecucion.
    		if(mq_send(mq_server, MSG_STOP, MAX_SIZE, 0) != 0)
    		{
    			perror("Error al enviar el mensaje de parada.");
    			funcionLog("Error al enviar el mensaje de parada");
    		}

    		exit(-1); 
    	}
	}
    	else//Aqui indicamos que no hemos puesto nada en --regex como argumento.
	{
		printf("No existe expresion regular. No la ha incluido\n");
		funcionLog("No existe expresion regular. No la ha incluido");
		exit(-1);
	}


	do {
		
		ssize_t bytes_read;//Bytes que se leeran del mq_receive

		
		bytes_read = mq_receive(mq_server, buffer, MAX_SIZE, NULL);//Leer de la cola. Devuelve el nº de bytes recibidos. NULL es la prioridad
		
		if(bytes_read < 0){//Porq no ha leido nada.
			perror("Error al recibir el mensaje");
			funcionLog("Error al recibir el mensaje");
			exit(-1);
		}
        	

		// Cerrar la cadena
		buffer[bytes_read] = '\0';//Le ponemos el /0 al buffer

			/* Ejecutar la expresion regular */
        reti = regexec(&regex, buffer, 0, NULL, 0);
        if( !reti )
        {
    		//Aqui es cuando empareja ya que reti=0

        	sprintf(emparejador, "Empareja");//Meter en la cadena la cadena de palabras Empareja.

            //Comprobamos si se manda el mensaje
			if(mq_send(mq_cliente, emparejador, MAX_SIZE, 0) != 0)
			{
				perror("Error al enviar el mensaje");
				funcionLog("Error al enviar el mensaje");
				exit(-1);
			}
        }
        else if( reti !=0 )
        {
        	//No empareja
    		
        	sprintf(emparejador, "No Empareja");

            // Enviar y comprobar si el mensaje se manda
			if(mq_send(mq_cliente, emparejador, MAX_SIZE, 0) != 0)
			{
				perror("Error al enviar el mensaje");
				funcionLog("Error al enviar el mensaje");
				exit(-1);
			}
        }

        else
        {
        	//Error
            regerror(reti, &regex, emparejador, sizeof(emparejador));
            fprintf(stderr, "Error al evaluar la expresion regular: %s\n", emparejador);
            funcionLog("Error al evaluar la expresion regular");
            exit(1);
        }
		
		if (strncmp(buffer, MSG_STOP, strlen(MSG_STOP))==0)//Si hay un exit escrito en buffer que se salga.
			must_stop = 1;
		else
		{
			printf("Recibido el mensaje: %s\n", buffer);//Imprimimos lo que ha escrito en buffer
			
		}
	
	
      
			
			
	
	} while (!must_stop);//minetras sea exit=0. Para que no se salga del programa.

	
	if(mq_close(mq_server) == (mqd_t)-1){//Cerrar cola del server
		perror("Error al cerrar la cola del servidor");
		funcionLog("Error al cerrar la cola del servidor");
		exit(-1);
	}

	
	if(mq_unlink(server_queue) == (mqd_t)-1){//Eliminar cola del servidor
		perror("Error al eliminar la cola del servidor");
		funcionLog("Error al eliminar la cola del servidor");
		exit(-1);
	}

if(mq_close(mq_cliente) == (mqd_t)-1){//Cerrar cola del cleinte
		perror("Error al cerrar la cola del cliente");
		funcionLog("Error al cerrar la cola del cliente");
		exit(-1);
	}

	
	if(mq_unlink(client_queue) == (mqd_t)-1){//Eliminar cola del cliente
		perror("Error al eliminar la cola del cliente");
		funcionLog("Error al eliminar la cola del cliente");
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

	
	sprintf(nombreFichero,"log-servidor.txt");
	if(fLog==NULL){
		fLog = fopen(nombreFichero,"at");//Anyadir
		if(fLog==NULL){
			perror("Error abriendo el fichero de log");
			exit(1);
		}
	}

	
	t = time(NULL);
	struct tm * p = localtime(&t);//Creamos la struct de tiempo, con la fecha, hora,.. Con localtime volcamos t, definida anteriormente a struct t *p
	strftime(mensajeAEscribir, 1000, "[%Y-%m-%d, %H:%M:%S]", p);//Esta es la estuctura. E mensajeAEscribir realmente es la hora.

	
	sprintf(mensajeAEscribir, "%s ==> %s\n", mensajeAEscribir, mensaje);//Lo guardmaos en mensaje a escirbir. Concatenamos el mensaje a escribir con la hora.
	
	
	resultado = fputs(mensajeAEscribir,fLog);
	if ( resultado < 0)
		perror("Error escribiendo en el fichero de log");

	fclose(fLog);
	fLog=NULL;//Liberamos la direecion de memoria del fichero.
}


void manejador(int num_signal)
{
	char msg[50];

	sprintf(msg,"Recibida la senyal=%d\n", num_signal );
	printf("%s\n", msg);
	funcionLog(msg);

	if(mq_send(mq_cliente, MSG_STOP, MAX_SIZE,20)!=0)
	{
		perror("Error al enviar la senyal de deteccion del programa");
		funcionLog("Error al enviar la senyal de deteccion del programa");
	}

	if(mq_close(mq_server) == (mqd_t)-1){//Cerrar cola del server
		perror("Error al cerrar la cola del servidor");
		funcionLog("Error al cerrar la cola del servidor");
		exit(-1);
	}

	
	if(mq_unlink(server_queue) == (mqd_t)-1){//Eliminar cola del servidor
		perror("Error al eliminar la cola del servidor");
		funcionLog("Error al eliminar la cola del servidor");
		exit(-1);
	}

if(mq_close(mq_cliente) == (mqd_t)-1){//Cerrar cola del cleinte
		perror("Error al cerrar la cola del cliente");
		funcionLog("Error al cerrar la cola del cliente");
		exit(-1);
	}

	
	if(mq_unlink(client_queue) == (mqd_t)-1){//Eliminar cola del cliente
		perror("Error al eliminar la cola del cliente");
		funcionLog("Error al eliminar la cola del cliente");
		exit(-1);
	}
	

	exit(0);
}