#include "proceso1.h"

int abrir_conexion(int puerto){

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
		log_info(logger, "Error en connect %d", sockfd);
		perror("Connect ERROR");
		sockfd = -1;
	}

	return sockfd;
}

void enviar_mensaje(int socket, char *msg){

	int bEnviados = send(socket, msg, sizeof(msg), 0);
	log_info(logger, "\nMande al socket: %d\nEl mensaje %s\nEn total %d bytes", socket, msg, bEnviados);

	return;
}

void recibir_mensaje(int sockfd){

    int bRecibidos;
    char buf[MAX_DATA_SIZE];
    
    while(escuchando){
        if((bRecibidos = recv(sockfd, buf, sizeof(buf), 0)) == -1){
            perror("ERROR en el recv");
        }

        buf[30] = '\0';

        printf("Recibí: %s", buf);
    }

    return;
}