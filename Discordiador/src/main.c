#include "proceso1.h"

int main(int argc, char **argv)
{
	system("clear");

	//CREO EL LOGGER Y CONFIG
	logger = log_create("logs/discordiador.log", "DISCORDIADOR", 1, LOG_LEVEL_INFO);
	log_info(logger, "Se inicio el log del discordiador: Proceso ID %d", getpid());

	config = get_cpu_config("../Discordiador/cfg/config.cfg");
	log_info(logger, "\nObtuve IP de ram: %s, puerto: %i\nObtuve IP de MONGO: %s, puerto: %i", config->ip_ram, config->puerto_ram, config->ip_mongo, config->puerto_mongo);

	//INICIALIZO EL PID
	patota_id = 1;

<<<<<<< HEAD
=======
	//REALIZO LA CONEXION CON RAM Y MONGO
	log_info(logger, "Conectando a RAM...");
	sockfd_ram = connect_to(config->ip_ram, config->puerto_ram);

	log_info(logger, "Conectando a MONGO...");
	sockfd_mongo = connect_to(config->ip_mongo, config->puerto_mongo);

>>>>>>> mi-ram-hq
	//LECTURA DE CONSOLA
	void (*comando[6])(char **) = {INICIAR_PATOTA, LISTAR_TRIPULANTES, EXPULSAR_TRIPULANTE, INICIAR_PLANIFICACION, PAUSAR_PLANIFICACION, OBTENER_BITACORA};

	char *leido;
	char **parametros;
	leido = readline(">");

	while (strcmp(leido, "exit"))
	{

		parametros = string_split(leido, " ");

		if (strcmp(parametros[0], "INICIAR_PATOTA") == 0)
			comando[0](parametros);
		else if (strcmp(parametros[0], "LISTAR_TRIPULANTES") == 0)
			comando[1](parametros);
		else if (strcmp(parametros[0], "EXPULSAR_TRIPULANTE") == 0)
			comando[2](parametros);
		else if (strcmp(parametros[0], "INICIAR_PLANIFICACION") == 0)
			comando[3](parametros);
		else if (strcmp(parametros[0], "PAUSAR_PLANIFICACION") == 0)
			comando[4](parametros);
		else if (strcmp(parametros[0], "OBTENER_BITACORA") == 0)
			comando[5](parametros);
		else
		{
			if (strcmp(leido, ""))
				printf("No reconozco ese comando\n");
			//PLANIFICADOR MANUAL, solo para pruebas
			else
				turno = atoi(leido);
		}

		free(leido);
		free(parametros);
		leido = readline(">");
	}
	free(leido);

<<<<<<< HEAD
	//REALIZO LA CONEXION CON RAM Y MONGO
	log_info(logger, "Conectando a RAM...");
	sockfd_ram = connect_to(config->ip_ram, config->puerto_ram);

	log_info(logger, "Conectando a MONGO...");
	sockfd_mongo = connect_to(config->ip_mongo, config->puerto_mongo);

=======
>>>>>>> mi-ram-hq
	char reconectOP;
	system("clear");

	while (sockfd_ram == -1 || sockfd_mongo == -1)
	{
		system("clear");
		printf("Error de conexion ¿desea reconectar? [s|n]\n");
		fflush(stdin);
		reconectOP = getchar();

		if (reconectOP == 's')
		{

			if (sockfd_ram == -1)
				sockfd_ram = connect_to(config->ip_ram, config->puerto_ram);
			if (sockfd_mongo == -1)
				sockfd_mongo = connect_to(config->ip_mongo, config->puerto_mongo);
		}
		else if (reconectOP == 'n')
		{
			exit(1);
			system("clear");
		}
	}

	log_info(logger, "Conexión establecida con RAM y con Mongo!");

	//MANDO MENSAJES
	char userOption = '\0';
	char *msg;
	system("clear");

	while (userOption != 'E')
	{

		msg = malloc(MAX_DATA_SIZE);
		puts("Mensaje a enviar:");
		scanf("%s", msg);

		fflush(stdin);

		puts("Enviar mensaje a:");
		puts("\t[m] i-Mongo-Store");
		puts("\t[r] Mi-RAM-HQ");
		puts("\t[E] EXIT");
		scanf(" %c", &userOption);

		if (userOption == 'm')
		{
			//enviar_mensaje(sockfd_mongo, msg);
		}
		else if (userOption == 'r')
		{
			//enviar_mensaje(sockfd_ram, msg);
		}

		free(msg);
		system("clear");
	}

	system("clear");
	escuchando = 0;
	close(sockfd_ram);
	close(sockfd_mongo);
	log_destroy(logger);
	free(config);

	return EXIT_SUCCESS;
}