// valgrind --tool=memcheck --leak-check=yes --show-possibly-lost=no --show-reachable=no --num-callers=20 ./proceso1

#include "proceso1.h"

int main(int argc, char **argv)
{
	system("clear");

	//CREO EL LOGGER Y CONFIG
	logger = log_create("logs/discordiador.log", "DISCORDIADOR", 0, LOG_LEVEL_INFO);
	log_info(logger, "Se inicio el log del discordiador");

	config = get_cpu_config("../Discordiador/cfg/config.cfg");
	log_info(logger, "Se cargo la config del Discordador");

	//INICIALIZO VARIABLES CLAVE
	patota_id              = 1;
	ejecutandoTripulantes  = 1;
	ejecutandoPlanificador = 1;
	escuchandoSabotajes    = 1;
	planificando           = 1;
	sabotaje               = 0;

	pthread_mutex_init(&mutex_block, NULL);
	sem_init(&s_multiprocesamiento, 0, config->grado_multitarea);

	exec    = list_create();
	ready   = list_create();
	bloq    = list_create();
	bloq_IO = list_create();

	pthread_mutex_init(&m_listaExec,  NULL);
	pthread_mutex_init(&m_listaReady, NULL);
	pthread_mutex_init(&m_listaBlock, NULL);
	pthread_mutex_init(&m_listaBlockIO, NULL);
	
	pthread_mutex_init(&pause_block, NULL);
	sem_init(&pause_exec, 0, config->grado_multitarea);

	//REALIZO LA CONEXION CON RAM Y MONGO
	log_info(logger, "Conectando a RAM...");
	sockfd_ram = connect_to(config->ip_ram, config->puerto_ram);

	log_info(logger, "Conectando a MONGO...");
	//sockfd_mongo = connect_to(config->ip_mongo, config->puerto_mongo);

	//EN CASO DE QUE LA CONEXION HAYA FALLADO
	/*char reconectOP;

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
	*/

	log_info(logger, "Conexión establecida con RAM y con Mongo!");

	//EMPIEZO A ESCUCHAR SABOTAJES QUE PUEDEN LLEGAR DESDE EL MONGO
	//pthread_t thread_sabotajes;
	//pthread_create(&thread_sabotajes, NULL, (void *)sabotajes, NULL);
	//pthread_detach(thread_sabotajes);

	//LECTURA DE CONSOLA
	void (*comando[7])(char **) = {INICIAR_PATOTA, LISTAR_TRIPULANTES, EXPULSAR_TRIPULANTE, INICIAR_PLANIFICACION, PAUSAR_PLANIFICACION, OBTENER_BITACORA, IMPRIMIR_SEGMENTOS};

	char *leido;
	char **parametros;
	leido = readline(">");
	while (strcmp(leido, "EXIT") && strcmp(leido, "exit"))
	{
		add_history(leido);

		parametros = string_split(leido, " ");

		string_to_upper(parametros[0]);

		if (strcmp(parametros[0],      "INICIAR_PATOTA") == 0)
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
		else if (strcmp(parametros[0], "IMPRMIR_SEGMENTOS") == 0)
			comando[6](parametros);
		else if (strcmp(parametros[0], "CLEAR") == 0)
			system("clear");
		else
		{
			if (strcmp(leido, ""))
				printf("No reconozco ese comando\n");
		}

		free(leido);
		free(parametros);
		leido = readline(">");
	}
	free(leido);

	ejecutandoTripulantes = 0;
	ejecutandoPlanificador = 0;
	escuchandoSabotajes = 0;

	sem_destroy(&s_multiprocesamiento);
	system("clear");
	close(sockfd_ram);
	close(sockfd_mongo);
	log_destroy(logger);
	free(config);

	return EXIT_SUCCESS;
}