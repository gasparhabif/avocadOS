#include "proceso1.h"

int sockfd_tripulante_mongo, sockfd_tripulante_ram;
int duracionMovimientos, duracionEjecucion, duracionBloqueado;
t_posicion pos_actual;
int tid;

void tripulante(void *parametro){

    tid = syscall(SYS_gettid);
    t_TCB *tcb_tripulante = malloc(sizeof(t_TCB));
    t_tarea *tarea_recibida;
    int tamanioSerializacion;
    int finTareas = 0, tareaPendiente = 0;
    int block = 0;

    log_info(logger, "Se inicio el tripulante N°:%d", tid);

    //TRAIGO EL TCB Y LO COMPLETO
    *tcb_tripulante = * (t_TCB *) parametro;
    tcb_tripulante->TID = tid;

    //CARGO MI POSICION
    pos_actual.posX = tcb_tripulante->posX;
    pos_actual.posY = tcb_tripulante->posY;
    log_info(logger, "Tripulante en posicion X:%d Y:%d", pos_actual.posX, pos_actual.posY);

    //ABRO LA CONEXION
    //sockfd_tripulante_mongo = connect_to(config->ip_mongo, config->puerto_mongo);
    sockfd_tripulante_ram   = connect_to(config->ip_ram,   config->puerto_ram);
    if(sockfd_tripulante_mongo == -1 || sockfd_tripulante_ram == -1){
        log_info(logger, "Muerte de tripulante por fallo de conexion");
        return;
    }
    log_info(logger, "El tripulante se conecto con RAM y con Mongo");

    //SERIALIZO Y ENVIO EL TCB
    void* d_Enviar = serializarTCB(tcb_tripulante, &tamanioSerializacion);
    send(sockfd_ram, d_Enviar, tamanioSerializacion, 0);
    free(tcb_tripulante);
    free(d_Enviar);
    log_info(logger, "Se envio el TCB");
    
    //ENVIAR EL TID AL MONGO 

    //CONTROL DE GRADO DE MULTIPROGRAMACION
    sem_wait(&s_multiprogramacion);

    //CAMBIAR A ESTADO READY
    actualizar_estado(sockfd_ram, tid, READY);

    //SOLICITO LA PRIMERA TAREA
    tarea_recibida = solicitar_tarea(&finTareas);

    while(finTareas == 0){

        //PIDO EL SEMAFORO PARA ENTRAR EN EXEC (WAIT) 
        pthread_mutex_lock(&mutex_exec);

        //CAMBIAR A ESTADO EXEC
        actualizar_estado(sockfd_ram, tid, EXEC);

        //SI NO HAY TAREAS PENDIENTES, PIDO UNA TAREA
        if(tareaPendiente == 0)
            tarea_recibida = solicitar_tarea(&finTareas);

        if(finTareas == 0){
            if(ejecutar_tarea(tarea_recibida))
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
        pthread_mutex_unlock(&mutex_exec);

        if(block){
            actualizar_estado(sockfd_tripulante_ram, tid, BLOCKED_IO);
            pthread_mutex_lock(&mutex_block);

            //ACA DEBERÍA MANDARLE LA TAREA AL MONGO NO EN OTRO MOMENTO

            for(int i=0; i<duracionBloqueado; i++){
                retardo_ciclo_cpu();
                duracionBloqueado--;
            }
            block = 0;

            pthread_mutex_unlock(&mutex_block);
            actualizar_estado(sockfd_tripulante_ram, tid, READY);
        }
        else if(!tareaPendiente && finTareas)
            actualizar_estado(sockfd_tripulante_ram, tid, EXIT);
        else
            actualizar_estado(sockfd_tripulante_ram, tid, READY);
    }
        
        
    //SALGO DE LOS ESTADOS: READY, RUNNING, EXEC
    sem_post(&s_multiprogramacion);

    close(sockfd_tripulante_mongo);
    close(sockfd_tripulante_ram);

    return;
}

t_tarea* solicitar_tarea(int *finTareas){
    
    log_info(logger, "Se solicito una tarea a la RAM");

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
        //CALCULO DEL TIEMPO QUE LA TAREA PERMANECERÁ MOVIENDOSE POR EL MAPA, EN EJECUCION Y BLOQUEADA
        if(tarea_recibida->codigoTarea == MOVER_POSICION){
            duracionMovimientos = cantMovimientos(pos_actual.posX, pos_actual.posY, tarea_recibida->posX, tarea_recibida->posY);
            duracionEjecucion   = tarea_recibida->duracionTarea;
            duracionBloqueado   = 0;
        }
        else{
            duracionMovimientos = cantMovimientos(pos_actual.posX, pos_actual.posY, tarea_recibida->posX, tarea_recibida->posY);
            duracionEjecucion   = 1; //ENVIO DE TAREA AL MONGO
            duracionBloqueado   = tarea_recibida->duracionTarea;
        }
    }

    log_info(logger, "Tarea recibida");

    return tarea_recibida;
}

int ejecutar_tarea (t_tarea *unaTarea){

    int tiempoMovimientos = 0;
    int tiempoEjecutando  = 0;

    if(strcmp(config->algoritmo, "FIFO") == 0){
        tiempoMovimientos = ejecutar_tiempos_CPU(duracionMovimientos, 0);
        tiempoEjecutando  = ejecutar_tiempos_CPU(duracionEjecucion,  0);        
    }
    else if (strcmp(config->algoritmo, "RR") == 0)
    {
         tiempoMovimientos = ejecutar_tiempos_CPU(duracionMovimientos, 0);
         if(tiempoMovimientos < config->quantum)
            tiempoEjecutando  = ejecutar_tiempos_CPU(duracionEjecucion, tiempoMovimientos); 
    }
    
    mover_tripulante(tiempoMovimientos, unaTarea->posX, unaTarea->posY);
    for(int i=0; i<tiempoEjecutando; i++){
            retardo_ciclo_cpu();
            duracionEjecucion--;
    }

    if(unaTarea->codigoTarea != MOVER_POSICION && duracionMovimientos == 0 && duracionEjecucion == 0){    
        int bEnviados;
        void* d_enviar = serializarTarea(unaTarea, &bEnviados);
        send(sockfd_mongo, d_enviar, bEnviados, 0);

        return 1;
    }

    return 0;

}

int ejecutar_tiempos_CPU(int duracionEjecucion, int tEjecutado){

    int tiempoAEjecutar = 0;

    if(duracionEjecucion != 0)
    {
        if(strcmp(config->algoritmo, "FIFO") == 0)
            tiempoAEjecutar = duracionEjecucion;
        else if(strcmp(config->algoritmo, "RR") == 0)
        {
            if(duracionEjecucion > config->quantum)
                tiempoAEjecutar = config->quantum - tEjecutado;
            else
                tiempoAEjecutar = duracionEjecucion - tEjecutado;
        }
    }

    return tiempoAEjecutar;
}

void actualizar_estado(int socket, uint32_t tid, char nuevoEstado){

    int bEnviar;
    void *d_enviar = serializar_ActulizacionEstado(tid, nuevoEstado, &bEnviar);
    send(socket, d_enviar, bEnviar, 0);
    free(d_enviar);

    log_info(logger, "Tripulante en estado: %c", nuevoEstado);

    return;
}

void mover_tripulante(int cantMovimientos, u_int32_t posX, u_int32_t posY){
    
    for(int i = 0; i < cantMovimientos; i++){
        
        retardo_ciclo_cpu();

        mover_una_posicion(posX, posY);

        int bEnviar;
        void *d_enviar = serializar_envioPosicion(tid, posX, posY, &bEnviar);
        send(sockfd_tripulante_ram,   d_enviar, bEnviar, 0);
        send(sockfd_tripulante_mongo, d_enviar, bEnviar, 0);
        free(d_enviar);

        duracionMovimientos--;

        log_info(logger, "Tripulante en posicion X:%d Y:%d", posX, posY);
    }
}

void mover_una_posicion(u_int32_t posX, u_int32_t posY){
    
    int movX = pos_actual.posX - posX;
    int movY = pos_actual.posY - posY;

    if(movX != 0){
        //REALIZAR UN MOVIMIENTO EN X
        //PUEDE SER PARA ATRAS O PARA ADELANTE
        if (movX > 0)
            pos_actual.posX++;
        else
            pos_actual.posX--;
    }
    else{
        //REALIZAR UN MOVIMIENTO EN Y
        //PUEDE SER PARA ATRAS O PARA ADELANTE
        if (movY > 0)
            pos_actual.posY++;
        else
            pos_actual.posY--;
    }
}

int cantMovimientos(int xInicial, int yInicial, int xFinal, int yFinal){
    return (xFinal-xInicial) + (yFinal-yInicial);
}

void retardo_ciclo_cpu(){
    sleep(config->retardo_ciclo_cpu);
    return;
}