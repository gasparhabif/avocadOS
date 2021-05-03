#include "proceso1.h"
/*
void abrir_conexion(void *unaConexion){

	struct d_conexion *dConexion = unaConexion;
	
	struct sockaddr_in server_addr;

	if ((*dConexion->socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Socket ERROR");
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons((int) dConexion->puerto);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(server_addr.sin_zero), '\0', 8);

	if (connect(*dConexion->socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("Connect ERROR");
	}

	return;
}
*/

void abrir_conexion(int puerto){

	int sockfd;

	struct sockaddr_in server_addr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Socket ERROR");
		sockfd = -1;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(puerto);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(server_addr.sin_zero), '\0', 8);

	if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("Connect ERROR");
		sockfd = -1;
	}

	pthread_exit((void *) sockfd);

	return;
}

void enviar_mensaje(void *unMensaje){
	
	struct d_mensaje *msg = unMensaje;

	int bEnviados = send(msg->socket, msg->datos, 4, 0);
	log_info(logger, "\nMande al socket: %d\nEl mensaje %\nEn total %d bytes", msg->socket, *msg->datos, bEnviados);

	return;
}

void recibir_mensaje(int sockfd){

    int bRecibidos;
    char buf[30];
    
    while(escuchando){
        if((bRecibidos = recv(sockfd, buf, sizeof(buf), 0)) == -1){
            perror("ERROR en el recv");
        }

        buf[30] = '\0';

        printf("Recibí: %s", buf);
    }

    return;
}