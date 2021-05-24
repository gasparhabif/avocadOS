#include "proceso2.h"

int main()
{
    system("clear");

    // Inicializar logger y config
    logger = log_create("logs/i-mongo-store.log", "i-Mongo-Store", 1, LOG_LEVEL_INFO);
    log_info(logger, "Log iniciado");
    config = get_store_config("../i-Mongo-Store/cfg/config.cfg");
    log_info(logger, "Configuración cargada");

    // Inicializar servidor
    int server_instance = init_server(config->puerto);

    // Verificar instancia
    if (server_instance == -1)
    {
        log_info(logger, "Error al inicializar servidor");
        return EXIT_FAILURE;
    }

    // Aceptar conexiones de los tripulantes
    log_info(logger, "Servidor escuchando en puerto %d", config->puerto);
    pthread_t accept_connections_thread;
    pthread_create(&accept_connections_thread, NULL, (void *)accept_connections, (void *)server_instance);
    pthread_join(accept_connections_thread, NULL);

    // ...

    free(config);

    return EXIT_SUCCESS;
}

void accept_connections(void *arg)
{
    int server_instance = (int)arg;

    while (1)
    {
        listen(server_instance, SOMAXCONN);

        // Aceptar conexión
        struct sockaddr_in client_dir;
        unsigned int dir_size = sizeof(socklen_t);
        int client = accept(server_instance, (void *)&client_dir, &dir_size);

        // Verificar conexión aceptada
        if (client == -1)
        {
            log_info(logger, "Error al aceptar conexión");
            break;
        }

        pthread_t connection_thread;
        pthread_create(&connection_thread, NULL, (void *)connection_handler, (void *)client);
        // Acá no ejecuto pthread_join() porque sino no crea nuevos threads hasta que no termine el último
    }
}

void connection_handler(void *arg)
{
    int client = (int)arg;

    log_info(logger, "Se conectó un tripulante en el socket %d", client);

    while (1)
    {
    }
}