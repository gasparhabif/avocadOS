#include "proceso3.h"

int main()
{
    system("clear");

    //CREO EL LOGGER
    logger = log_create("../Mi-Ram-HQ/logs/proceso3.log", "Mi-Ram-HQ", 1, LOG_LEVEL_INFO);
    log_info(logger, "Se inicio el log");

    //INICIO DEL CONFIG
    t_ram_conf *config = get_ram_config("../Mi-Ram-HQ/cfg/config.cfg");
    log_info(logger, "Se cargo la config de la RAM");

    //INICIO EL SERVER
    int server_instance;
    if ((server_instance = init_server(config->puerto)) == -1)
    {
        log_error(logger, "Algo malió sal al iniciar el servidor");
        return 1;
    }
    log_info(logger, "Se inicio el server");
    
    //EMPIEZO A ESCUCHAR Y A RECIBIR
    recibiendo_mensajes  = 1;
    aceptando_conexiones = 1;

    //CREO EL HILO QUE ESCUCHA CONEXIONES
    pthread_t accept_connections_thread;
    pthread_create(&accept_connections_thread, NULL, (void *)aceptar_conexiones, (void *) server_instance);

    //Esto es para que ejecute el thread, sino no ejecuta porque se corta muy rapido el main
    pthread_join(accept_connections_thread, NULL);

    //TODO: cosas raras de memoria

    free(config);
}