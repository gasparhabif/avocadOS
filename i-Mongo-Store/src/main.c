#include "proceso2.h"

int main()
{
    system("clear");

    // Inicializar logger y config
    logger = log_create("logs/i-mongo-store.log", "i-Mongo-Store", 1, LOG_LEVEL_INFO);
    log_info(logger, "Log iniciado");
    config = get_store_config("../i-Mongo-Store/cfg/config.cfg");
    log_info(logger, "Configuración cargada");

    // Inicializar FS
    superbloque = malloc(sizeof(t_superbloque));
    init_fs();

    // Inicializar servidor
    int server_instance = init_server(config->puerto);

    // Verificar instancia
    if (server_instance == -1)
    {
        log_error(logger, "Error al inicializar servidor");
        return EXIT_FAILURE;
    }

    log_info(logger, "Servidor escuchando en puerto %d", config->puerto);

    // Aceptar conexión del Discordiador
    pthread_t discordiador_cnx_thread;
    pthread_create(&discordiador_cnx_thread, NULL, (void *)discordiador_cxn_handler, (void *)server_instance);

    // Aceptar conexiones de los tripulantes
    pthread_t tripulantes_cxns_thread;
    pthread_create(&tripulantes_cxns_thread, NULL, (void *)accept_tripulantes, (void *)server_instance);
    pthread_join(tripulantes_cxns_thread, NULL);

    // ...

    free(superbloque);
    free(config);

    return EXIT_SUCCESS;
}

// INICIAR_PATOTA 3 ./unasTareas.txt 2|5 7|9