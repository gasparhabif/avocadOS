#include "proceso1.h"

void tripulante(void *tcb){

    int tid = syscall(SYS_gettid);
    t_TCB *tcb_tripulante;
    int sockfd_tripulante_mongo, sockfd_tripulante_ram;

    log_info(logger, "Se inicio el tripulante N°: %d", tid);

    //TRAIGO EL TCB
    tcb_tripulante = tcb;
    tcb_tripulante->TID = tid;

    //ABRO LA CONEXION
    sockfd_tripulante_mongo = connect_to(config->ip_mongo, config->puerto_mongo);
    sockfd_tripulante_ram   = connect_to(config->ip_ram, config->puerto_ram);
/*
    //SERIALIZO Y ENVIO EL TCB
    void* d_Enviar = serializarTCB(tcb_tripulante);
    send(sockfd_ram, d_Enviar, sizeof(d_Enviar), 0);
*/

/*
    if(strcmp(config->algoritmo, "FIFO") == 0){
        
        pthread_mutex_lock(&mutex);

        while(1){ //hay tareas por ejecutar?

            //CAMBIAR A ESTADO EXEC & PEDIR TAREA

            //RECIBIR TAREA
            t_tarea *tarea_recibida = (t_tarea*) recibir_paquete(sockfd_tripulante_ram);
            
            //REALIZAR TAREA
            ejecutar_tarea(tarea_recibida, sockfd_tripulante_mongo);

            //LÓGICA DE SABOTAJES
        }
        
        //LIBERO EL SEMAFORO
        pthread_mutex_unlock(&mutex);     
    }
    else if(strcmp(config->algoritmo, "RR") == 0){
        //TODO
    }
    
//    close(sockfd_tripulante_mongo);
//    close(sockfd_tripulante_ram);
*/
    return;

}

int ejecutar_tarea (t_tarea *unaTarea, int sockfd_mongo){

    // RETURN VALUE
    // 0: fallo la ejecucion de la tarea
    // 1: se ejecuto correctamente

    //void* d_enviar = serializar_tareas_mongo(unaTarea->codigoTarea, unaTarea->parametro);

    //send(sockfd_mongo, d_enviar, sizeof(d_enviar), 0);

    //recibi

    return 1;

}