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

    // Iniciar paths
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

    // Sincronizar bloques
    pthread_t blocks_sync_thread;
    pthread_create(&blocks_sync_thread, NULL, (void *)sync_blocks_handler, NULL);
    pthread_detach(blocks_sync_thread);

    // Si existen, cargar recursos
    if (file_exists(oxigeno_file_path))
    {
        last_oxigeno = load_recurso(oxigeno_file_path);
    }
    if (file_exists(comida_file_path))
    {
        last_comida = load_recurso(comida_file_path);
    }
    if (file_exists(basura_file_path))
    {
        last_basura = load_recurso(basura_file_path);
    }

    // Inicializar servidor
    int server_instance = init_server(config->puerto);

    // Verificar instancia
    if (server_instance == -1)
    {
        log_error(logger, "Error al inicializar servidor");
        return EXIT_FAILURE;
    }

    // Aceptar conexión del Discordiador para sabotajes
    listen(server_instance, 1);
    log_info(logger, "Esperando Discordiador en el puerto %d para informar sabotajes", config->puerto);

    struct sockaddr_in client_dir;
    unsigned int dir_size = sizeof(socklen_t);
    discordiador_cxn_sabotajes = accept(server_instance, (void *)&client_dir, &dir_size);

    if (discordiador_cxn_sabotajes == -1)
    {
        log_error(logger, "Error al aceptar conexión");
        return EXIT_FAILURE;
    }
    log_info(logger, "Se conectó el discordiador en el socket %d para informar sabotajes", discordiador_cxn_sabotajes);

    // Aceptar conexión del Discordiador para bitácoras
    listen(server_instance, 1);
    log_info(logger, "Esperando Discordiador en el puerto %d para traficar bitácoras", config->puerto);

    // struct sockaddr_in client_dir;
    // unsigned int dir_size = sizeof(socklen_t);
    discordiador_cxn_bitacoras = accept(server_instance, (void *)&client_dir, &dir_size);

    if (discordiador_cxn_bitacoras == -1)
    {
        log_error(logger, "Error al aceptar conexión");
        return EXIT_FAILURE;
    }
    log_info(logger, "Se conectó el Discordiador en el socket %d para traficar bitácoras", discordiador_cxn_bitacoras);

    pthread_t discordiador_cxn_thread;
    pthread_create(&discordiador_cxn_thread, NULL, (void *)discordiador_cxn_handler_bitacoras, NULL);
    pthread_detach(discordiador_cxn_thread);

    // Aceptar conexiones de los tripulantes
    log_info(logger, "Esperando tripulantes en el puerto %d", config->puerto);
    pthread_t tripulantes_cxns_thread;
    pthread_create(&tripulantes_cxns_thread, NULL, (void *)accept_tripulantes, (void *)server_instance);
    pthread_join(tripulantes_cxns_thread, NULL);

    // ...

    liberar_recurso(last_oxigeno);
    liberar_recurso(last_comida);
    liberar_recurso(last_basura);

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

void sync_blocks_handler()
{
    while (1)
    {
        sleep(config->tiempo_sincronizacion);
        pthread_mutex_lock(&fs_libre);
        sync_blocks();
        log_info(logger, "Se sincronizaron los bloques");
        pthread_mutex_unlock(&fs_libre);
    }
}
