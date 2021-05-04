#include "proceso1.h"

int main(int argc, char **argv)
{
	system("clear");

	logger = log_create("discordiador.log", "DISCORDIADOR", 1, LOG_LEVEL_INFO);
	log_info(logger, "Se inicio el log del discordiador: Proceso ID %d", getpid());

	t_cpu_conf *config = get_cpu_config("../Discordiador/cfg/config.cfg");
	log_info(logger, "\nObtuve IP de ram: %s, puerto: %i\nObtuve IP de MONGO: %s, puerto: %i", config->ip_ram, config->puerto_ram, config->ip_mongo, config->puerto_mongo);

	pthread_t abrir_conexion_ram, abrir_conexion_mongo;
	
	sockfd_mongo = malloc(sizeof(int));
	sockfd_ram = malloc(sizeof(int));

	log_info(logger, "Conectando a RAM...");
	pthread_create(&abrir_conexion_ram, NULL, (void*) abrir_conexion, (void *) config->puerto_ram);

	log_info(logger, "Conectando a MONGO...");
	pthread_create(&abrir_conexion_mongo, NULL, (void*) abrir_conexion, (void *) config->puerto_mongo);

	pthread_join(abrir_conexion_ram, &sockfd_ram);
	pthread_join(abrir_conexion_mongo, &sockfd_mongo);

	char reconectOP;
	system("clear");

	while((int) sockfd_ram == -1 || (int) sockfd_mongo == -1){
		system("clear");
		printf("Error de conexion ¿desea reconectar? [s|n]\n");
		reconectOP = getchar();

		if(reconectOP == 's'){

			if((int) sockfd_ram == -1){
				pthread_create(&abrir_conexion_ram, NULL, (void*) abrir_conexion, (void *) config->puerto_ram);
				pthread_join(abrir_conexion_ram, &sockfd_ram);
			}

			if((int) sockfd_mongo == -1){
				pthread_create(&abrir_conexion_mongo, NULL, (void*) abrir_conexion, (void *) config->puerto_mongo);
				pthread_join(abrir_conexion_mongo, &sockfd_mongo);
			}

		} else if(reconectOP == 'n'){
			exit(1);
		}
	}

	log_info(logger, "Conexión establecida con RAM y con Mongo!");

	escuchando = 1;
	pthread_t escuchar_ram, escuchar_mongo;
	pthread_create(&escuchar_ram,   NULL, (void*) recibir_mensaje, sockfd_ram);
	pthread_create(&escuchar_mongo, NULL, (void*) recibir_mensaje, sockfd_mongo);

	pthread_t hiloEnviarMensaje;
	char userOption = '\0';
	struct d_mensaje msg;
	system("clear");

	while(userOption != 'E'){

		msg.datos = malloc(MAX_DATA_SIZE);
		puts("Mensaje a enviar:");
		scanf("%s", msg.datos);

		fflush(stdin);

		puts("Enviar mensaje a:");
		puts("\t[m] i-Mongo-Store");
		puts("\t[r] Mi-RAM-HQ");
		puts("\t[E] EXIT");
		scanf(" %c", &userOption);

		if(userOption == 'm'){
			msg.socket = (int) sockfd_mongo;

			pthread_create(&hiloEnviarMensaje, NULL, (void*) enviar_mensaje, (void *) &msg);
			pthread_join(hiloEnviarMensaje, NULL);
		}
		else if(userOption == 'r'){
			msg.socket = (int) sockfd_ram;

			pthread_create(&hiloEnviarMensaje, NULL, (void*) enviar_mensaje, (void *) &msg);
			pthread_join(hiloEnviarMensaje, NULL);
		}

		free(msg.datos);
		system("clear");
	}

	escuchando=0;
	close((int) sockfd_ram);
	close((int) sockfd_mongo);
	log_destroy(logger);
	free(config);

	return EXIT_SUCCESS;
}