#include "proceso1.h"

t_log *g_logger;

int main(int argc, char **argv)
{

	t_cpu_conf *config = get_cpu_config("../Discordiador/cfg/config.cfg");

	pid_t p_ram, p_mongo;

	iniciar_logger(g_logger);

	log_info(g_logger, "\nObtuve IP de ram: %s, puerto: %i\nObtuve IP de MONGO: %s, puerto: %i", config->ip_ram, config->puerto_ram, config->ip_mongo, config->puerto_mongo);

	p_ram = fork();

	char *msg = "Prueba";

	if (p_ram == 0)
	{
		//INICIO DEL PROCESO HIJO: 		MI_RAM_HQ

		int socketfd_ram;
		struct sockaddr_in ram_addr;

		log_info(g_logger, "Conectando con RAM...");

		if ((socketfd_ram = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			perror("Socket MI_RAM_HQ ERROR");
		}

		ram_addr.sin_family = AF_INET;
		ram_addr.sin_port = htons(config->puerto_ram);
		ram_addr.sin_addr.s_addr = INADDR_ANY;
		memset(&(ram_addr.sin_zero), '\0', 8);

		if (connect(socketfd_ram, (struct sockaddr *)&ram_addr, sizeof(struct sockaddr)) == -1)
		{
			perror("Connect MI_RAM_HQ ERROR");
		}

		int bEnviadosPorRam = send(socketfd_ram, msg, sizeof(msg), 0);
		log_info(g_logger, "Mande %s, en total %d bytes", msg, bEnviadosPorRam);

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

			int sockfd_mongo;
			struct sockaddr_in mongo_addr;

			log_info(g_logger, "Conectando con MONGO...");

			if ((sockfd_mongo = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			{
				perror("Socket I_MONGO_STORE ERROR");
			}

			mongo_addr.sin_family = AF_INET;
			mongo_addr.sin_port = htons(config->puerto_mongo);
			mongo_addr.sin_addr.s_addr = INADDR_ANY;
			memset(&(mongo_addr.sin_zero), '\0', 8);

			if (connect(sockfd_mongo, (struct sockaddr *)&mongo_addr, sizeof(struct sockaddr)) == -1)
			{
				perror("Connect I_MONGO_STORE ERROR");
			}

			int bEnviadosPorMongo = send(sockfd_mongo, msg, sizeof(msg), 0);
			log_info(g_logger, "Mande %s, en total %d bytes", msg, bEnviadosPorMongo);

			close(sockfd_mongo);

			exit(0);

			//FIN DEL PROCESO HIJO: 	I_MONGO_STORE
		}
		else
		{
			//PADRE

			//Esperar INICIAR_PLANIFICACION por consola

			char *leido;

			leido = readline(">");

			while (strcmp(leido, ""))
			{

				leido = readline(">");
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