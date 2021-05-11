#include "proceso1.h"

void tripulante(void *tcb){

    t_log *tripulante_logger;
    char *logger_dir;
    int tid = syscall(SYS_gettid);
    int logger_dir_size = sizeof("logs/tripulante.log") + strlen(string_itoa(tid)) + 1;
    t_TCB *tcb_tripulante;
    int sockfd_tripulante_mongo, sockfd_tripulante_ram;

    //CREO EL LOGGER
    logger_dir = malloc(logger_dir_size);

    strcat(logger_dir, "logs/tripulante");
    strcat(logger_dir, string_itoa(tid));
    strcat(logger_dir, ".log");
    logger_dir[logger_dir_size] = '\0';

    tripulante_logger = log_create(logger_dir, "TRIPULANTE", 1, LOG_LEVEL_INFO);
    log_info(tripulante_logger, "Se inicio el tripulante N°: %d", tid);

    free(logger_dir);

    //TRAIGO EL TCB
    tcb_tripulante = tcb;

    tcb_tripulante->TID = tid;
/*
    //ABRO LA CONEXION
    sockfd_tripulante_mongo = abrir_conexion(config->puerto_mongo);
    sockfd_tripulante_ram   = abrir_conexion(config->puerto_ram);

    //TODO: serializar tcb y mandarlo
*/
    return;

}