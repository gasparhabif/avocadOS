#include "proceso1.h"

t_log *logger;

int main(int argc, char **argv)
{
	logger = log_create("discordiador.log", "DISCORDIADOR", 1, LOG_LEVEL_INFO);
	log_info(logger, "Se inicio el log del discordiador: Proceso ID %d", getpid());

	t_cpu_conf *config = get_cpu_config("../Discordiador/cfg/config.cfg");
	log_info(logger, "\nObtuve IP de ram: %s, puerto: %i\nObtuve IP de MONGO: %s, puerto: %i", config->ip_ram, config->puerto_ram, config->ip_mongo, config->puerto_mongo);

	pthread_t hiloRAM, hiloMONGO;
	
	sockfd_mongo = malloc(sizeof(int));
	sockfd_ram = malloc(sizeof(int));

	struct d_conexion d_conexion_ram;
	d_conexion_ram.puerto = (void *) config->puerto_ram;
	d_conexion_ram.socket = sockfd_ram;

	log_info(logger, "Conectando a RAM...");
	pthread_create(&hiloRAM, NULL, (void*) abrir_conexion, (void *) &d_conexion_ram);

	struct d_conexion d_conexion_mongo;
	d_conexion_mongo.puerto = (void *) config->puerto_mongo;
	d_conexion_mongo.socket = sockfd_mongo;

	log_info(logger, "Conectando a MONGO...");
	pthread_create(&hiloMONGO, NULL, (void*) abrir_conexion, (void *) &d_conexion_mongo);

	pthread_join(hiloRAM, NULL);
	pthread_join(hiloMONGO, NULL);

	pthread_t hiloEnviarMensaje;
	char userOption = '0';
	struct d_mensaje *msg;

	while(userOption != 'E'){
	
		msg = malloc(sizeof(struct d_mensaje));
		printf("Mensaje a enviar: ");
		scanf("%s", msg->datos);
		printf("Enviar mensaje a:\ni-Mongo-Store [m]\nMi-RAM-HQ [r]\nEXIT [E]\n");
		scanf("%c", &userOption);

		if(userOption == 'm'){
			msg->socket = sockfd_mongo;

			pthread_create(&hiloEnviarMensaje, NULL, (void*) enviar_mensaje, (void *) &msg);
			pthread_join(hiloEnviarMensaje, NULL);
		}
		else if(userOption == 'r'){
			msg->socket = sockfd_ram;

			pthread_create(&hiloEnviarMensaje, NULL, (void*) enviar_mensaje, (void *) &msg);
			pthread_join(hiloEnviarMensaje, NULL);
		}

		free(msg);
	}

	close(*sockfd_ram);
	close(*sockfd_mongo);
	log_destroy(logger);
	free(config);

	return EXIT_SUCCESS;
}

void abrir_conexion(void *unaConexion){

	struct d_conexion *dConexion = unaConexion;
	struct sockaddr_in server_addr;

	if ((*dConexion->socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Socket ERROR");
	}

	log_info(logger, "Socket: %d y Puerto: %d", dConexion->socket, dConexion->puerto);

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

void enviar_mensaje(void *unMensaje){
	
	struct d_mensaje *msg = unMensaje;

	int bEnviados = send(*msg->socket, msg->datos, 4, 0);
	log_info(logger, "Mande al socket: %d\nEl mensaje %\nEn total %d bytes", *msg->socket, msg->datos, bEnviados);

}
