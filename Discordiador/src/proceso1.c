#include "proceso1.h"

t_log *g_logger;
t_config *g_config;

int main(int argc, char **argv){

	char *ip_RAM, *puerto_RAM;
	char *ip_MONGO, *puerto_MONGO;

	pid_t p_ram, p_mongo;

	iniciar_logger(g_logger);
	iniciar_config(g_config);

	//leer_config_dConexion(ip_RAM, puerto_RAM, ip_MONGO, puerto_MONGO);
	ip_RAM = leer_config("IP_MI_RAM_HQ");
	puerto_RAM = leer_config("PUERTO_MI_RAM_HQ");
	ip_MONGO = leer_config("IP_I_MONGO_STORE");
	puerto_MONGO = leer_config("PUERTO_I_MONGO_STORE");

	log_info(g_logger, "\nObtuve IP de ram: %s, puerto: %s\nObtuve IP de MONGO: %s, puerto: %s", ip_RAM, puerto_RAM, ip_MONGO, puerto_MONGO);

	p_ram= fork();

	if(p_ram == 0){

		//INICIO DEL PROCESO HIJO: 		MI_RAM_HQ
		log_info(g_logger, "Conectando con RAM...");

		int sockfd_ram;
		struct sockaddr_in ram_addr;

		if( (sockfd_ram = socket(AF_INET, SOCK_STREAM, 0)) == -1){
				perror("Socket MI_RAM_HQ ERROR");
		}

		ram_addr.sin_family = AF_INET;
		ram_addr.sin_port = htons(puerto_RAM);
		ram_addr.sin_addr.s_addr = INADDR_ANY;
		memset(&(ram_addr.sin_zero), '\0', 8);

		if (connect(sockfd_ram, (struct sockaddr *)&ram_addr, sizeof(struct sockaddr)) == -1) {
			perror("Connect MI_RAM_HQ ERROR");
		}

		char *enviarPorRam = "Soy el discordiador! Me conecte";

		int bEnviadosPorRam = send(sockfd_ram, enviarPorRam, sizeof(enviarPorRam), 0);
		log_info(g_logger, "Mande %s, en total %d bytes", enviarPorRam, bEnviadosPorRam);

		close(sockfd_ram);

		exit(0);

		//FIN DEL PROCESO HIJO: 		MI_RAM_HQ
	}

	else{
		p_mongo = fork();

		if (p_mongo == 0) {

			//INICIO DEL PROCESO HIJO: 	I_MONGO_STORE

			log_info(g_logger, "Conectando con MONGO...");

			int sockfd_mongo;
			struct sockaddr_in mongo_addr;

			if( (sockfd_mongo = socket(AF_INET, SOCK_STREAM, 0)) == -1){
				perror("Socket I_MONGO_STORE ERROR");
			}

			mongo_addr.sin_family = AF_INET;
			mongo_addr.sin_port = htons(puerto_MONGO);
			mongo_addr.sin_addr.s_addr = INADDR_ANY;
			memset(&(mongo_addr.sin_zero), '\0', 8);

			if (connect(sockfd_mongo, (struct sockaddr *)&mongo_addr, sizeof(struct sockaddr)) == -1) {
				perror("Connect I_MONGO_STORE ERROR");
			}

			char *enviarPorMongo = "Soy el discordiador! Me conecte";

			int bEnviadosPorMongo = send(sockfd_mongo, enviarPorMongo, sizeof(enviarPorMongo), 0);
			log_info(g_logger, "Mande %s, en total %d bytes", enviarPorMongo, bEnviadosPorMongo);

			close(sockfd_mongo);

			exit(0);

			//FIN DEL PROCESO HIJO: 	I_MONGO_STORE

		}
		else{
	        //PADRE

			//Esperar INICIAR_PLANIFICACION por consola

			char * leido;

			leido = readline(">");

			while(strcmp(leido, "")){


				leido = readline(">");
			}

			exit(0);
	    }
	}

	log_destroy(g_logger);
	config_destroy(g_config);



	return EXIT_SUCCESS;
}

void iniciar_logger(){
	g_logger = log_create("discordiador.log", "DISCORDIADOR", 1, LOG_LEVEL_INFO);
	log_info(g_logger, "Se inicio el log del discordiador: proceso %d", getpid());
	return;
}

void iniciar_config(){
	g_config = config_create("discordiador.config");
	return;
}

void leer_config_dConexion(char *ip_RAM, char *puerto_RAM, char *ip_MONGO, char *puerto_MONGO){
	ip_RAM = leer_config("IP_MI_RAM_HQ");
	puerto_RAM = leer_config("PUERTO_MI_RAM_HQ");
	ip_MONGO = leer_config("IP_I_MONGO_STORE");
	puerto_MONGO = leer_config("PUERTO_I_MONGO_STORE");
	return;
}

char* leer_config(char* dato){
	char *valor;
	valor = config_get_string_value(g_config, dato);
	return valor;
}
