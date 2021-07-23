#include "proceso2.h"

int main()
{
    system("clear");

    // Iniciar logger
    logger = log_create("logs/i-mongo-store.log", "i-Mongo-Store", 1, LOG_LEVEL_INFO);
    log_info(logger, "Se inició el log");

    // Cargar config
    config = get_store_config("../i-Mongo-Store/cfg/config.cfg");
    log_info(logger, "Se cargó la configuración");

    // Iniciar mutex
    pthread_mutex_init(&fs_libre, NULL);

    // Iniciar FS
    init_paths();

    // Atender signal
    signal(SIGUSR1, sabotaje_handler);

    if (!file_exists(superbloque_file_path))
    {
        log_info(logger, "El FS no existe. Creando FS desde cero...");

        create_dirs();
        log_info(logger, "Se crearon los directorios");
        create_superbloque(BLOCK_SIZE, BLOCKS);
        log_info(logger, "Se creó SuperBloque.ims");
        create_blocks();
        log_info(logger, "Se creó Blocks.ims");

        log_info(logger, "Se creó el FS");
    }

    // Cargar SuperBloque.ims
    superbloque = load_superbloque();
    log_info(logger, "Se cargó SuperBloque.ims");

    // Cargar Blocks.ims
    blocks_file_copy = load_blocks();
    log_info(logger, "Se cargó Blocks.ims");

    // TODO: Sincronizar bloques
    // ...

    // Inicializar servidor
    int server_instance = init_server(config->puerto);

    // Verificar instancia
    if (server_instance == -1)
    {
        log_error(logger, "Error al inicializar servidor");
        return EXIT_FAILURE;
    }

    // Aceptar conexión del Discordiador
    listen(server_instance, 1);
    log_info(logger, "Esperando discordiador en el puerto %d", config->puerto);

    struct sockaddr_in client_dir;
    unsigned int dir_size = sizeof(socklen_t);
    discordiador_cxn = accept(server_instance, (void *)&client_dir, &dir_size);

    if (discordiador_cxn == -1)
    {
        log_error(logger, "Error al aceptar conexión");
        return EXIT_FAILURE;
    }
    log_info(logger, "Se conectó el discordiador en el socket %d", discordiador_cxn);

    // Aceptar conexiones de los tripulantes
    log_info(logger, "Esperando tripulantes en el puerto %d", config->puerto);
    pthread_t tripulantes_cxns_thread;
    pthread_create(&tripulantes_cxns_thread, NULL, (void *)accept_tripulantes, (void *)server_instance);
    pthread_join(tripulantes_cxns_thread, NULL);

    // ...

    free(blocks_file_copy);
    free(superbloque->bitmap->bitarray);
    free(superbloque->bitmap);
    free(superbloque);
    free(config);

    munmap(blocks_file, blocks_file_size);
    munmap(superbloque_file, superbloque_file_size);
    close(blocks_fd);
    close(superbloque_fd);

    return EXIT_SUCCESS;
}