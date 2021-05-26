//TODO: actualizar posiciones

#include "proceso1.h"

void tripulante(void *parametro){

    int tid = syscall(SYS_gettid);
    t_TCB *tcb_tripulante = malloc(sizeof(t_TCB));
    int sockfd_tripulante_mongo, sockfd_tripulante_ram;
    t_tarea *tarea_recibida;
    int tamanioSerializacion;
    int finTareas = 0, tareaPendiente = 0;
    int duracionEjecucion, duracionBloqueado;
    int block = 0;
    t_posicion pos_actual;

    //PARA QUE NO CORTE LA EJECUCION EN EL WHILE
    tarea_recibida->codigoTarea = 1;

    log_info(logger, "Se inicio el tripulante N°: %d", tid);

    //TRAIGO EL TCB Y LO COMPLETO
    *tcb_tripulante = * (t_TCB *) parametro;
    tcb_tripulante->TID = tid;

    //printf("TID: %d\n",   tcb_tripulante->TID);
    //printf("EST: %c\n",   tcb_tripulante->estado);
    //printf("P_X: %d\n",   tcb_tripulante->posX);
    //printf("P_Y: %d\n",   tcb_tripulante->posY);
    //printf("P_I: %d\n",   tcb_tripulante->proximaInstruccion);
    //printf("P_P: %d\n\n", tcb_tripulante->puntero_PCB);

    //CARGO MI POSICION
    pos_actual.posX = tcb_tripulante->posX;
    pos_actual.posY = tcb_tripulante->posY;

    //ABRO LA CONEXION
    sockfd_tripulante_mongo = connect_to(config->ip_mongo, config->puerto_mongo);
    sockfd_tripulante_ram   = connect_to(config->ip_ram,   config->puerto_ram);

    //SERIALIZO Y ENVIO EL TCB
    void* d_Enviar = serializarTCB(tcb_tripulante, &tamanioSerializacion);
    send(sockfd_ram, d_Enviar, tamanioSerializacion, 0);
    free(tcb_tripulante);
    free(d_Enviar);

    //CONTROL DE GRADO DE MULTIPROGRAMACION
    sem_wait(&s_multiprogramacion);

    //CAMBIAR A ESTADO READY
    actualizar_estado(sockfd_ram, tid, READY);
 
    while(finTareas == 0){

        //PIDO EL SEMAFORO PARA ENTRAR EN EXEC (WAIT) 
        pthread_mutex_lock(&mutex);

        //CAMBIAR A ESTADO EXEC
        actualizar_estado(sockfd_ram, tid, EXEC);

        //SI NO HAY TAREAS PENDIENTES, PIDO UNA TAREA
        if(tareaPendiente == 0)
            tarea_recibida = solicitar_tarea(sockfd_tripulante_ram, tid, &duracionEjecucion, &duracionBloqueado, &finTareas, pos_actual);

        if(finTareas == 0){
            if(ejecutar_tarea(tarea_recibida, &duracionEjecucion, &duracionBloqueado, sockfd_tripulante_mongo))
                block = 1;

            //CHEQUEO SI QUEDO ALGO POR EJECUTAR DE ESTA TAREA
            if (duracionEjecucion==0 && duracionBloqueado==0)
                tareaPendiente = 0;
            else
                tareaPendiente = 1;
        }
        else
            tareaPendiente = 0;

        //LIBERO EL SEMAFORO (SIGNAL)
        pthread_mutex_unlock(&mutex);

        if(block){
            actualizar_estado(sockfd_tripulante_ram, tid, BLOCKED);
            for(int i=0; i<duracionBloqueado; i++){
                sleep(config->retardo_ciclo_cpu);
                duracionBloqueado--;
            }
            block = 0;
            actualizar_estado(sockfd_tripulante_ram, tid, READY);
        }
        else if(tareaPendiente)
            actualizar_estado(sockfd_tripulante_ram, tid, READY);
        else if (!tareaPendiente)
            actualizar_estado(sockfd_tripulante_ram, tid, EXIT);
    }
        
        
    //SALGO DE LOS ESTADOS: READY, RUNNING, EXEC
    sem_post(&s_multiprogramacion);

    close(sockfd_tripulante_mongo);
    close(sockfd_tripulante_ram);

    return;
}

t_tarea* solicitar_tarea(int sockfd_tripulante_ram, uint32_t tid, int *duracionEjecucion, int*duracionBloqueado, int *finTareas, t_posicion pos_actual){
    
    //PEDIR TAREA
    int tamanioSerializacion;
    void *solicitud_tarea = serializarInt(tid, SOLICITAR_TAREA, &tamanioSerializacion);
    send(sockfd_ram, solicitud_tarea, tamanioSerializacion, 0);
    free(solicitud_tarea);

    //RECIBIR TAREA
    t_tarea *tarea_recibida;
    tarea_recibida = (t_tarea*) recibir_paquete(sockfd_tripulante_ram);

    //CHEQUEO QUE LA TAREA RECIBIDA SEA LA ULTIMA 
    if(tarea_recibida->codigoTarea == FIN_TAREAS)
        *finTareas = 1;
    else{
        //CALCULO DEL TIEMPO QUE LA TAREA PERMANECERÁ EN EJECUCION Y BLOQUEADA
        if(tarea_recibida->codigoTarea == MOVER_POSICION){
            *duracionEjecucion = tarea_recibida->duracionTarea + cantMovimientos(pos_actual.posX, pos_actual.posY, tarea_recibida->posX, tarea_recibida->posY);
            *duracionBloqueado = 0;
        }
        else{
            *duracionEjecucion = 1 + cantMovimientos(pos_actual.posX, pos_actual.posY, tarea_recibida->posX, tarea_recibida->posY);
            *duracionBloqueado = tarea_recibida->duracionTarea;
        }
    }

    return tarea_recibida;
}

int ejecutar_tarea (t_tarea *unaTarea, int *duracionEjecucion, int *duracionBloqueado, int sockfd_mongo){

    int tiempoEjecutado = ejecutar_tiempos_CPU(duracionEjecucion);

    if(unaTarea->codigoTarea != MOVER_POSICION){
        if((strcmp(config->algoritmo, "RR") == 0 && tiempoEjecutado < config->quantum) || strcmp(config->algoritmo, "FIFO") == 0){
            
            int bEnviados;
            void* d_enviar = serializarTarea(unaTarea, &bEnviados);
            send(sockfd_mongo, d_enviar, bEnviados, 0);

            return 1;
        }
    }

    return 0;

}

int ejecutar_tiempos_CPU(int *duracionEjecucion){

    int tiempoAEjecutar = 0;

    if(*duracionEjecucion != 0)
    {
        if(strcmp(config->algoritmo, "FIFO") == 0)
            tiempoAEjecutar = *duracionEjecucion;
        else if(strcmp(config->algoritmo, "RR") == 0)
        {
            if(*duracionEjecucion > config->quantum)
                tiempoAEjecutar = config->quantum;
            else
                tiempoAEjecutar = *duracionEjecucion;
        }

        for(int i=0; i<tiempoAEjecutar; i++){
            sleep(config->retardo_ciclo_cpu);
            *duracionEjecucion -= 1;
        }
    }

    return tiempoAEjecutar;
}

void actualizar_estado(int socket, uint32_t tid, char nuevoEstado){

    int bEnviar;
    void *d_enviar = serializar_ActulizacionEstado(tid, nuevoEstado, &bEnviar);
    send(socket, d_enviar, bEnviar, 0);
    free(d_enviar);

    return;
}

void mover_tripulante(int socket_ram, int socket_mongo, uint32_t tid, u_int32_t posX, u_int32_t posY){

}

int cantMovimientos(int xInicial, int yInicial, int xFinal, int yFinal){
    return (xFinal-xInicial) + (yFinal-yInicial);
}