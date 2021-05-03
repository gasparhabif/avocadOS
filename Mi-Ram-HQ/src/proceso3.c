#include "proceso3.h"

int main()
{
    iniciar_logger();

    t_ram_conf *config = get_ram_config("../Mi-Ram-HQ/cfg/config.cfg");
    log_info(logger, "Se cargo la config de la RAM");

    int server_instance;
    if ((server_instance = init_server(config->puerto)) == -1)
    {
        log_error(logger, "Algo malió sal al iniciar el servidor");
        return 1;
    }

    log_info(logger, "Servidor iniciado");
    listen(server_instance, 100);

    struct sockaddr_in client_dir;
    unsigned int dir_size = sizeof(socklen_t);
    int client;

    if ((client = accept(server_instance, (void *)&client_dir, &dir_size)) == -1)
    {
        log_error(logger, "Algo malió sal al aceptar al cliente");
        return 1;
    }
    
    log_info(logger, "Recibí una conexión en el socket %d", client);
    int connection_discordiador = connect_to("127.0.0.1", 5000);
    char *buffer = malloc(100);

    while (1)
    {
        int msg_size = recv(client, buffer, 100, 0);

        if (msg_size <= 0)
        {
            log_error(logger, "Algo malió sal al recibir el mensaje");
            return 1;
        }
        buffer[msg_size] = '\0';
        log_info(logger, "Recibí \"%s\" con peso de %d bytes", buffer, msg_size);

        if(connection_discordiador == -1)
            log_error(logger, "Algo malió sal al conectar con Discordiador");
        else
        {
            send(connection_discordiador, "OK", 2, 0);
            log_info(logger, "Envie mensaje de confirmación de recepción al Discordiador");
        }
    }

    free(buffer);
    free(config);
}

void iniciar_logger()
{
    logger = log_create("../Mi-Ram-HQ/logs/proceso3.log", "Mi-Ram-HQ", 1, LOG_LEVEL_INFO);
	log_info(logger, "Se inicio el log del Proceso 3 [Mi-Ram-HQ]");
}