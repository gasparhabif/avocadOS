#include "proceso1.h"

void enviarMensaje(int signal){
	if(signal == SIGUSR1){

		msg = malloc(4);
		scanf("%s", &msg);


		if(userOption == 'm')
			mandarPorMongo = 1;
		else if(userOption == 'r')
			mandarPorRAM = 1;
	}
}


int main(int argc, char **argv)
{
	signal(SIGUSR1, enviarMensaje);
	t_cpu_conf *config = get_cpu_config("../Discordiador/cfg/config.cfg");
	pid_t p_ram, p_mongo;

	iniciar_logger(g_logger);

	log_info(g_logger, "\nObtuve IP de ram: %s, puerto: %i\nObtuve IP de MONGO: %s, puerto: %i", config->ip_ram, config->puerto_ram, config->ip_mongo, config->puerto_mongo);

	p_ram = fork();

	if (p_ram == 0)
	{
		//INICIO DEL PROCESO HIJO: 		MI_RAM_HQ
		
		log_info(g_logger, "Conectando a RAM...");
		int socketfd_ram = abrir_conexion(config->puerto_ram);

		int bEnviadosPorRam = send(socketfd_ram, msg, sizeof(msg), 0);
		log_info(g_logger, "Mande a RAM %s, en total %d bytes", msg, bEnviadosPorRam);

/*
		while(1){
			if(mandarPorRAM){
				int bEnviadosPorRam = send(socketfd_ram, msg, sizeof(msg), 0);
				log_info(g_logger, "Mande a RAM %s, en total %d bytes", msg, bEnviadosPorRam);
				free(msg);
			}
		}
*/

		close(socketfd_ram);

		exit(0);

		//FIN DEL PROCESO HIJO: 		MI_RAM_HQ
	}

	else
	{
		p_mongo = fork();

		if (p_mongo == 0)
		{

			//INICIO DEL PROCESO HIJO: 	I_MONGO_STORE

			log_info(g_logger, "Conectando a MONGO...");
			int sockfd_mongo = abrir_conexion(config->puerto_mongo);

			int bEnviadosPorMongo = send(sockfd_mongo, msg, sizeof(msg), 0);
			log_info(g_logger, "Mande a MONGO %s, en total %d bytes", msg, bEnviadosPorMongo);

/*
			while(1){
				if(mandarPorRAM){
					int bEnviadosPorMongo = send(sockfd_mongo, msg, sizeof(msg), 0);
					log_info(g_logger, "Mande a MONGO %s, en total %d bytes", msg, bEnviadosPorMongo);
					free(msg);
				}
			}
*/

			close(sockfd_mongo);

			exit(0);

			//FIN DEL PROCESO HIJO: 	I_MONGO_STORE
		}
		else
		{
			//PADRE

			printf("Enviar mensaje a:\ni-Mongo-Store [m]\nMi-RAM-HQ[r]\nEXIT[E]\n");
			userOption = scanf("%c", &userOption);

			if(userOption == 'm'){
				kill(p_mongo, SIGUSR1);
			}
			else if(userOption == 'r'){
				kill(p_ram, SIGUSR1);
			}
			else if(userOption == 'E'){
				kill(p_mongo, SIGTERM);
				kill(p_ram, SIGTERM);
				kill(getpid(), SIGTERM);
			}

			exit(0);
		}
	}

	log_destroy(g_logger);
	free(config);

	return EXIT_SUCCESS;
}

void iniciar_logger()
{
	g_logger = log_create("discordiador.log", "DISCORDIADOR", 1, LOG_LEVEL_INFO);
	log_info(g_logger, "Se inicio el log del discordiador: Proceso ID %d", getpid());
	return;
}

int abrir_conexion(char *puerto){
		
		int socketfd;
		struct sockaddr_in server_addr;

		if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			perror("Socket ERROR");
		}

		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(puerto);
		server_addr.sin_addr.s_addr = INADDR_ANY;
		memset(&(server_addr.sin_zero), '\0', 8);

		if (connect(socketfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
		{
			perror("Connect ERROR");
		}

		return socketfd;
}