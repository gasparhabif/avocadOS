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

/*
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer));
	
	while(escuchando){
		//RECIBO EL CODIGO DE OPERACION
		recv(unSocket, &(paquete->codigo_operacion), sizeof(uint8_t), 0)) == -1)
		//RECIBO EL TAMAÑO DEL STREAM
		recv(unSocket, &(paquete->buffer->size), sizeof(uint32_t), 0);
		//RESERVO LA MEMORIA PARA RECIBIR AL STREAM
		paquete->buffer->stream = malloc(paquete->buffer->size);
		//RECIBO EL STREAM
		recv(unSocket, paquete->buffer->stream, paquete->buffer->size, 0);

		void *dRecibidos;

		switch(paquete->codigo_operacion){
			case TCB:
				dRecibidos = desearilizarTCB(paquete);
				break;
			case PCB:
				deserializarPCB(paquete);
				break;
			case TAREAS:
				deseralizarTareas(paquete);
				break;
			default:
				log_info(logger, "Recibi un paquete desconocido");
				break;
		}

		free(paquete->buffer->stream);
    	free(paquete->buffer);
    	free(paquete);		
	}

	return dRecibidos;
*/
    return;
}