#include "proceso3.h"

int main()
{
    system("clear");

    //CREO EL LOGGER
    logger = log_create("../Mi-Ram-HQ/logs/proceso3.log", "Mi-Ram-HQ", 1, LOG_LEVEL_INFO);
    log_info(logger, "Se inicio el log");

    //INICIO DEL CONFIG
    config = get_ram_config("../Mi-Ram-HQ/cfg/config.cfg");
    log_info(logger, "Se cargo la config de la RAM");

    //HAGO EL GRAN MALLOC
    memoria = malloc(config->tamanio_memoria);
    log_info(logger, "%d bytes reservados en %p", config->tamanio_memoria, memoria);

    //SI SE UTILIZA EL ESQUEMA DE SEGMENTACION LE INDICO QUE EN CASO DE RECIBIR
    //SIGUSR1 COMPACTE LA MEMORIA
    signal(SIGUSR1, compactar);

    //INICIO EL MUTEX DE ACCESO A LA MEMORIA
    pthread_mutex_init(&acceso_memoria, NULL);

    //CREO ESTRUCTURAS PARA CONOCER EL ESTADO DE LA MEMORIA:
    //      EN EL CASO DE LA PAGINACION   ES UNA ARRAY DE BITS CON LA CANTIDAD DE FRAMES
    //      EN EL CASO DE LA SEGMENTACION ES UNA LISTA CON EL INICIO Y TAMANIO DE CADA SEGMENTO Y UN BIT QUE INDICA SI EL MISMO ESTA OCUPADO O NO
    if (strcmp(config->esquema_memoria, "SEGMENTACION") == 0)
    {
        tabla_estado_segmentos = list_create();

        //CREO LA LISTA DE PROCESOS
        tabla_procesos = list_create();

        //INDICO EL PRIMER SEGMENTO Y LO PONGO LIBRE
        estado_segmentos *primer_seg = malloc(sizeof(estado_segmentos));
        primer_seg->inicio = (int)memoria;
        primer_seg->limite = config->tamanio_memoria;
        primer_seg->ocupado = 0;

        list_add(tabla_estado_segmentos, primer_seg);
    }
    else if (strcmp(config->esquema_memoria, "PAGINACION") == 0)
    {
        tamanio_paginas = config->tamanio_pagina;
        estado_frames = malloc(config->tamanio_memoria / tamanio_paginas);
    }
    //free(primer_seg);

    //INICIO EL SERVER
    int server_instance;
    if ((server_instance = init_server(config->puerto)) == -1)
    {
        log_error(logger, "Algo malió sal al iniciar el servidor");
        return 1;
    }
    log_info(logger, "Se inicio el server");

    //EMPIEZO A ESCUCHAR Y A RECIBIR
    recibiendo_mensajes = 1;
    aceptando_conexiones = 1;

    //CREO EL HILO QUE ESCUCHA CONEXIONES
    pthread_t accept_connections_thread;
    pthread_create(&accept_connections_thread, NULL, (void *)aceptar_conexiones, (void *)server_instance);
    pthread_join(accept_connections_thread, NULL);

    free(memoria);
    free(config);
}