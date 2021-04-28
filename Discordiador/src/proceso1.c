#include "proceso1.h"

int main(int argc, char **argv){
	t_log *l_logger;
	t_config *l_config;

	char *ip_RAM, *puerto_RAM;
	char *ip_MONGO, *puerto_MONGO;

	pid_t p_ram, p_mongo;

	iniciar_logger(l_logger);
	iniciar_config(l_config);
	leer_config_dConexion(l_config, ip_RAM, puerto_RAM, ip_MONGO, puerto_MONGO);

	p_ram= fork();

	if(p_ram == 0){

		//INICIO DEL PROCESO HIJO: 		MI_RAM_HQ
		log_info(l_logger, "Conectando con RAM");

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
		log_info(l_logger, "Mande %s, en total %d bytes", enviarPorRam, bEnviadosPorRam);

		close(sockfd_ram);

		//FIN DEL PROCESO HIJO: 		MI_RAM_HQ
	}

	else{
		p_mongo = fork();

		if (p_mongo == 0) {

			//INICIO DEL PROCESO HIJO: 	I_MONGO_STORE

			int sockfd_mongo;
			struct sockaddr_in mongo_addr;

			if( (sockfd_mongo = socket(AF_INET, SOCK_STREAM, 0)) == -1){
				perror("Socket I_MONGO_STORE ERROR");
			}

			mongo_addr.sin_family = AF_INET;
			mongo_addr.sin_port = htons(puerto_RAM);
			mongo_addr.sin_addr.s_addr = INADDR_ANY;
			memset(&(mongo_addr.sin_zero), '\0', 8);

			if (connect(sockfd_mongo, (struct sockaddr *)&mongo_addr, sizeof(struct sockaddr)) == -1) {
				perror("Connect I_MONGO_STORE ERROR");
			}

			char *enviarPorMongo = "Soy el discordiador! Me conecte";

			int bEnviadosPorMongo = send(sockfd_mongo, enviarPorMongo, sizeof(enviarPorMongo), 0);
			log_info(l_logger, "Mande %s, en total %d bytes", enviarPorMongo, bEnviadosPorMongo);

			close(sockfd_mongo);

			//FIN DEL PROCESO HIJO: 	I_MONGO_STORE

		}
		else{
	        //PADRE



			//Esperar INICIAR_PLANIFICACION por consola
	    }
	}


	log_destroy(l_logger);
	config_destroy(l_config);

	return EXIT_SUCCESS;
}

void iniciar_logger(t_log *l_logger){
	l_logger = log_create("discordiador.log", "DISCORDIADOR", 1, LOG_LEVEL_INFO);
	log_info(l_logger, "Se inicio el log del discordiador: proceso %d", getpid());
	return;
}

void iniciar_config(t_config *l_config){
	l_config = config_create("discordador.config");
	return;
}

void leer_config_dConexion(t_config *l_config, char *ip_RAM, char *puerto_RAM, char *ip_MONGO, char *puerto_MONGO){
	ip_RAM = leer_config(l_config, "IP_MI_RAM_HQ");
	puerto_RAM = leer_config(l_config, "PUERTO_MI_RAM_HQ");
	ip_MONGO = leer_config(l_config, "IP_I_MONGO_STORE");
	puerto_MONGO = leer_config(l_config, "PUERTO_I_MONGO_STORE");
	return;
}

char* leer_config(t_config *l_config, char* dato){
	char *valor;
	valor = config_get_string_value(l_config, dato);
	return valor;
}
