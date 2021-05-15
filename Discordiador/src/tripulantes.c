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

    //ABRO LA CONEXION
    sockfd_tripulante_mongo = connect_to(config->ip_mongo, config->puerto_mongo);
    sockfd_tripulante_ram   = connect_to(config->ip_ram, config->puerto_ram);

    //SERIALIZO Y ENVIO EL TCB
    void* d_Enviar = serializarTCB(tcb_tripulante);
    send(sockfd_ram, d_Enviar, sizeof(d_Enviar), 0);
    
    while(ejecutandoTripulantes){
        if(turno == tid){
            //CAMBIAR A ESTADO EXEC
            
            //PEDIR TAREA


            //RECIBIR TAREA
            t_tarea *tarea_recibida = (t_tarea*) recibir_paquete(sockfd_tripulante_ram);

            //REALIZAR TAREA
            ejecutar_tarea(tarea_recibida, sockfd_tripulante_mongo);
        }
    }
    
    close(sockfd_tripulante_mongo);
    close(sockfd_tripulante_ram);

    return;

}

int ejecutar_tarea (t_tarea *unaTarea, int sockfd_mongo){

    //void* d_enviar = serializar_tareas_mongo(unaTarea->codigoTarea, unaTarea->parametro);

    //send(sockfd_mongo, d_enviar, sizeof(d_enviar), 0);

    return 1;

}