#include "proceso1.h"

void tripulante(t_parametros_tripulantes *parametro)
{
    int duracionMovimientos, duracionEjecucion, duracionBloqueado;
    int tid;
    tid = syscall(SYS_gettid);
    t_TCB tcb_tripulante;
    t_tarea_descomprimida *tarea_recibida;
    int len_tarea;
    char *nom_tarea = NULL;
    int tamanioSerializacion;
    int finTareas = 0, tareaPendiente = 0;
    int block = 0;

    log_info(logger, "Se inicio el tripulante N°:%d", tid);

    //TRAIGO LOS PARAMETROS
    t_parametros_tripulantes *parametros_recibidos = malloc(sizeof(t_parametros_tripulantes));
    parametros_recibidos = parametro;

    //TRAIGO EL TCB Y LO COMPLETO
    tcb_tripulante = parametros_recibidos->tcbTripulante;
    tcb_tripulante.TID = tid;

    /*
    printf("---------------TCB----------------\n");
    printf("TID:    %d\n", tcb_tripulante.TID);
    printf("ESTADO: %d\n", tcb_tripulante.estado);
    printf("POSX:   %d\n", tcb_tripulante.posX);
    printf("POSY:   %d\n", tcb_tripulante.posY);
    printf("---------------TCB----------------\n\n");
    */

    //CREO EL TCB AUXILIAR CON EL QUE VOY A TRABAJAR EN LA PLANIFICACION, SABOTAJES Y MAS
    t_admin_tripulantes *admin = malloc(sizeof(t_admin_tripulantes));
    admin = parametros_recibidos->admin;

    admin->tid = tid;
    admin->estado = NEW;
    admin->posX = tcb_tripulante.posX;
    admin->posY = tcb_tripulante.posY;
    admin->debeMorir = 0;

    pthread_mutex_init(&(admin->pausar_tripulante), NULL);

    if (!planificando)
        pthread_mutex_trylock(&admin->pausar_tripulante);

    /*
    printf("---------------ADMIN----------------\n");
    printf("PID:    %d\n", admin->pid);
    printf("TID:    %d\n", admin->tid);
    printf("ESTADO: %d\n", admin->estado);
    printf("POSX:   %d\n", admin->posX);
    printf("POSY:   %d\n", admin->posY);
    printf("MUERTE: %d\n", admin->debeMorir);
    printf("---------------ADMIN----------------\n");
    */

    //DEVUELVO LA MEMORIA DE LOS PARAMETROS RECIBIDOS
    free(parametros_recibidos);

    //LOGEO EL INICIO
    log_info(logger, "Tripulante en posicion X:%d Y:%d", admin->posX, admin->posY);

    //ABRO LA CONEXION CON LA RAM
    admin->sockfd_tripulante_ram = connect_to(config->ip_ram, config->puerto_ram);
    if (admin->sockfd_tripulante_ram == -1)
    {
        log_info(logger, "Tripulante abandona la nave por fallo de conexion con Mi-RAM-HQ");
        return;
    }
    log_info(logger, "El tripulante se conecto con Mi-RAM-HQ");

    //SERIALIZO Y ENVIO EL TCB
    void *d_Enviar = serializarTCB(admin->pid, tcb_tripulante, &tamanioSerializacion);
    send(admin->sockfd_tripulante_ram, d_Enviar, tamanioSerializacion, 0);
    free(d_Enviar);
    log_info(logger, "Se envio el TCB a la RAM");

    //ESPERAR A QUE SE CREEN TODAS LAS ESTRUCTURAS DE LA MEMORIA
    if (*((int *) recibir_paquete(admin->sockfd_tripulante_ram)) < 0)
    {
        log_info(logger, "La memoria no pudo guardar mis estructuras, voy a abandonar la nave");
        return;
    }

/*
    admin->sockfd_tripulante_mongo = connect_to(config->ip_mongo, config->puerto_mongo);
    if (admin->sockfd_tripulante_mongo == -1)
    {
        log_info(logger, "Tripulante abandona la nave por fallo de conexion con i-Mongo-Store");
        return;
    }
    log_info(logger, "El tripulante se conecto con i-Mongo-Store");
*/

    //printf("Posición de TID %d: %d|%d\n", admin->tid, admin->posX, admin->posY);

    //ENVIO EL TID Y LA POSICION INICIAL AL MONGO
    void *enviar_tidYpos = serializar_posicionCtid(admin->tid, admin->posX, admin->posY, &tamanioSerializacion);
    send(admin->sockfd_tripulante_mongo, enviar_tidYpos, tamanioSerializacion, 0);
    free(enviar_tidYpos);
    log_info(logger, "Se envio el TID y la posicion al i-mongo-store");

    //CAMBIAR A ESTADO READY
    actualizar_estado(admin, READY);

    while (finTareas == 0)
    {
        //pthread_mutex_trylock(&admin->pausar_tripulante);

        if (pthread_mutex_trylock(&admin->pausar_tripulante) != 0)
            pthread_mutex_lock(&admin->pausar_tripulante);

        //PIDO EL SEMAFORO PARA ENTRAR EN EXEC (WAIT)
        sem_wait(&s_multiprocesamiento);

        //CAMBIAR A ESTADO EXEC
        actualizar_estado(admin, EXEC);

        if (admin->debeMorir)
            finTareas = 1;

        //SI NO HAY TAREAS PENDIENTES, PIDO UNA TAREA
        if (tareaPendiente == 0)
        {
            //ANTES DE SOLICITAR UNA TAREA CHEQUEO SI EL TRIPULANTE DEBE MORIR
            //if (admin->debeMorir)
            //    finTareas = 1;
            //else
                tarea_recibida = solicitar_tarea(admin, &finTareas, &duracionMovimientos, &duracionEjecucion, &duracionBloqueado, &len_tarea, &nom_tarea);
        }

        //SI LA TAREA RECIBIDA NO ES LA ULTIMA (o no se solicito el fin del tripulante)
        if (finTareas == 0)
        {
            //EJECUTO LA TAREA, SI DEBO BLOCKEAR EL TRIPULANTE LA VARIABLE BLOCK VALDRA 1
            block = ejecutar_tarea(admin, tarea_recibida, &duracionMovimientos, &duracionEjecucion);

            //CHEQUEO SI QUEDO ALGO POR EJECUTAR DE ESTA TAREA
            if (duracionMovimientos == 0 && duracionEjecucion == 0 && duracionBloqueado == 0)
                tareaPendiente = 0;
            else
                tareaPendiente = 1;
        }
        else
            tareaPendiente = 0;

        //LIBERO EL SEMAFORO DE EXEC (SIGNAL)
        sem_post(&s_multiprocesamiento);

        if (block)
        {
            block = 0;
            actualizar_estado(admin, BLOCKED_IO);
            pthread_mutex_lock(&mutex_block);

            //ENVIO LA TAREA AL MONGO
            int bEnviar;
            void *d_enviar = serializar_ejecutarTarea(tarea_recibida->codigoTarea, len_tarea, nom_tarea, tarea_recibida->parametro, &bEnviar);
            send(admin->sockfd_tripulante_mongo, d_enviar, bEnviar, 0);
            free(d_enviar);

            for (int i = duracionBloqueado; i > 0; i--)
            {
                log_info(logger, "Ciclo bloqueado");
                retardo_ciclo_cpu();
            }
            duracionBloqueado = 0;
            tareaPendiente = 0;

            pthread_mutex_unlock(&mutex_block);
            actualizar_estado(admin, READY);
        }
        else if (!tareaPendiente && finTareas)
        {
            actualizar_estado(admin, EXIT);
        }
        else if (!tareaPendiente && !finTareas)
        {
            int bEnviar;
            void *d_enviar = serializar_ejecutarTarea(tarea_recibida->codigoTarea, len_tarea, nom_tarea, tarea_recibida->parametro, &bEnviar);
            send(admin->sockfd_tripulante_mongo, d_enviar, bEnviar, 0);
            free(d_enviar);
        }
        else{
            actualizar_estado(admin, READY);
        }

        if (admin->debeMorir)
            finTareas = 1;

        pthread_mutex_unlock(&admin->pausar_tripulante);
    }

    if (admin->estado != 'X')
        actualizar_estado(admin, EXIT);    
    
    //DOY EL AVISO AL MONGO QUE FINALICÉ MIS TAREAS
    int bEnviar;
    void *dEnviar;
    dEnviar = serializarInt(1, FIN_TAREAS, &bEnviar);
    send(admin->sockfd_tripulante_mongo, dEnviar, bEnviar, 0);
    free(dEnviar);

    //DOY EL AVISO A RAM QUE DEVUELVA MI MEMORIA
    dEnviar = serializar_pidYtid(admin->pid, admin->tid, ELIMINAR_TRIPULANTE, &bEnviar);
    send(admin->sockfd_tripulante_ram, dEnviar, bEnviar, 0);
    free(dEnviar);

    matarTripulante(admin->tid);

    //close(admin->sockfd_tripulante_mongo);
    close(admin->sockfd_tripulante_ram);

    free(admin);

    log_info(logger, "Termine mis tareas en la nave, adios");

    pthread_exit(0);

    return;
}

t_tarea_descomprimida *solicitar_tarea(t_admin_tripulantes *admin, int *finTareas, int *duracionMovimientos, int *duracionEjecucion, int *duracionBloqueado, int *len_tarea, char **nom_tarea)
{
    //PEDIR TAREA
    int tamanioSerializacion;
    void *solicitud_tarea = serializar_pidYtid(admin->pid, admin->tid, SOLICITAR_TAREA, &tamanioSerializacion);
    send(admin->sockfd_tripulante_ram, solicitud_tarea, tamanioSerializacion, 0);
    free(solicitud_tarea);

    //printf("Solicitando tarea...\n");

    //RECIBIR TAREA
    //t_tarea *tarea_recibida = malloc(sizeof(t_tarea));
    t_tarea *tarea_comprimida = (t_tarea *) recibir_paquete(admin->sockfd_tripulante_ram);

    //printf("Tamanio: %d\n", tarea_comprimida->tamanio_tarea);
    //printf("Tarea: %s\n", tarea_comprimida->tarea);

    t_tarea_descomprimida *tarea_recibida = NULL;

    //CHEQUEO QUE LA TAREA RECIBIDA SEA LA ULTIMA
    if (tarea_comprimida->tamanio_tarea == FIN_TAREAS){
        *finTareas = 1;
    }
    else
    {
        char* nombre_tarea;
        tarea_recibida = descomprimir_tarea(tarea_comprimida, len_tarea, &nombre_tarea);
        (*len_tarea)++;
        *nom_tarea = malloc(*len_tarea);
        strcpy(*nom_tarea, nombre_tarea);

        //printf("Tarea LEN=> %d\n", *len_tarea);
        //if(*len_tarea != 0)
        //    printf("Tarea CPY=> %s\n", *nom_tarea);

        //AVISO AL MONGO QUE INICIO UNA TAREA PARA INCLUIRLA EN LA BITACORA
        void *comenzar_tarea = serializar_string(nombre_tarea, *len_tarea, &tamanioSerializacion);
        send(admin->sockfd_tripulante_mongo, comenzar_tarea, tamanioSerializacion, 0);
        free(comenzar_tarea);

        //CALCULO DEL TIEMPO QUE LA TAREA PERMANECERÁ MOVIENDOSE POR EL MAPA, EN EJECUCION Y BLOQUEADA
        if (tarea_recibida->codigoTarea == MOVER_POSICION)
        {
            *duracionBloqueado = 0;
            *duracionEjecucion = tarea_recibida->duracionTarea;
        }
        else
        {
            *duracionEjecucion = 1; //ENVIO DE TAREA AL MONGO
            *duracionBloqueado = tarea_recibida->duracionTarea;
        }
        *duracionMovimientos = cantMovimientos(admin->posX, admin->posY, tarea_recibida->posX, tarea_recibida->posY);
    }

    log_info(logger, "Tarea recibida:\nDuracion movimientos: %d\nDuracion ejecucion:   %d\nDuracion bloqueado:   %d\n", *duracionMovimientos, *duracionEjecucion, *duracionBloqueado);

    return tarea_recibida;
}

int ejecutar_tarea(t_admin_tripulantes *admin, t_tarea_descomprimida *unaTarea, int *duracionMovimientos, int *duracionEjecucion)
{
    int tiempoMovimientos = 0;
    int tiempoEjecutando = 0;

    if (strcmp(config->algoritmo, "FIFO") == 0)
    {
        tiempoMovimientos = ejecutar_tiempos_CPU(*duracionMovimientos, 0);
        tiempoEjecutando = ejecutar_tiempos_CPU(*duracionEjecucion, 0);
    }
    else if (strcmp(config->algoritmo, "RR") == 0)
    {
        tiempoMovimientos = ejecutar_tiempos_CPU(*duracionMovimientos, 0);
        if (tiempoMovimientos < config->quantum)
        {
            tiempoEjecutando = ejecutar_tiempos_CPU(*duracionEjecucion, tiempoMovimientos);
            log_info(logger, "Tiempo ejecutado: %d", tiempoEjecutando);
        }
    }

    mover_tripulante(admin, unaTarea->posX, unaTarea->posY, tiempoMovimientos, duracionMovimientos);

    for (int i = 0; i < tiempoEjecutando; i++)
    {

        retardo_ciclo_cpu();
        log_info(logger, "Ciclo ejecucion");
        (*duracionEjecucion)--;
    }

    if (unaTarea->codigoTarea != MOVER_POSICION && *duracionMovimientos == 0 && *duracionEjecucion == 0)
        return 1;

    return 0;
}

int ejecutar_tiempos_CPU(int duracionEjecucion, int tEjecutado)
{
    int tiempoAEjecutar = 0;

    if (duracionEjecucion != 0)
    {
        if (strcmp(config->algoritmo, "FIFO") == 0)
            tiempoAEjecutar = duracionEjecucion;
        else if (strcmp(config->algoritmo, "RR") == 0)
        {
            if (tEjecutado == 0)
            {
                if (duracionEjecucion > config->quantum)
                    tiempoAEjecutar = config->quantum;
                else
                    tiempoAEjecutar = duracionEjecucion;
            }
            else
            {
                if (duracionEjecucion > config->quantum)
                    tiempoAEjecutar = config->quantum - tEjecutado;
                else
                {
                    if ((duracionEjecucion + tEjecutado) <= config->quantum)
                        tiempoAEjecutar = duracionEjecucion;
                    else
                        tiempoAEjecutar = duracionEjecucion - tEjecutado;
                }
            }
        }
    }

    return tiempoAEjecutar;
}

void actualizar_estado(t_admin_tripulantes *admin, char nuevoEstado)
{

    //ENVIO A LA RAM EL NUEVO ESTADO
    int bEnviar;
    void *d_enviar = serializar_ActulizacionEstado(admin->pid, admin->tid, nuevoEstado, &bEnviar);
    send(admin->sockfd_tripulante_ram, d_enviar, bEnviar, 0);
    free(d_enviar);

    //ELIMINO EL TRIPULANTE DE LA LISTA EN LA QUE SE ENCUENTRE
    switch (admin->estado)
    {
    case EXEC:
        pthread_mutex_lock(&m_listaExec);
        //log_info(logger, "Sale de EXEC: %d", admin->tid);
        eliminarTripulante(exec, admin->tid);
        pthread_mutex_unlock(&m_listaExec);
        break;
    case READY:
        pthread_mutex_lock(&m_listaReady);
        //log_info(logger, "Sale de READY: %d", admin->tid);
        eliminarTripulante(ready, admin->tid);
        pthread_mutex_unlock(&m_listaReady);
        break;
    case BLOCKED_IO:
        pthread_mutex_lock(&m_listaBlockIO);
        //log_info(logger, "Sale de BLOCK_IO: %d", admin->tid);
        eliminarTripulante(bloq_IO, admin->tid);
        pthread_mutex_unlock(&m_listaBlockIO);
        break;
    }

    admin->estado = nuevoEstado;

    //AGREGO EL TRIPULANTE A LA NUEVA LISTA
    switch (nuevoEstado)
    {
    case EXEC:
        pthread_mutex_lock(&m_listaExec);
        //log_info(logger, "Entra a EXEC: %d", admin->tid);
        list_add(exec, admin);
        pthread_mutex_unlock(&m_listaExec);
        break;
    case READY:
        pthread_mutex_lock(&m_listaReady);
        //log_info(logger, "Entra a READY: %d", admin->tid);
        list_add(ready, admin);
        pthread_mutex_unlock(&m_listaReady);
        break;
    case BLOCKED_IO:
        pthread_mutex_lock(&m_listaBlockIO);
        //log_info(logger, "Entra a BLOCK_IO: %d", admin->tid);
        list_add(bloq_IO, admin);
        pthread_mutex_unlock(&m_listaBlockIO);
        break;
    default:
        break;
    }

    log_info(logger, "Tripulante en estado: %c", nuevoEstado);

    return;
}

void mover_tripulante(t_admin_tripulantes *admin, uint32_t posX, uint32_t posY, int movimientosPosibles, int *duracionMovimientos)
{

    for (int i = 0; i < movimientosPosibles; i++)
    {

        if (sabotaje)
            retardo_ciclo_en_emergencia();
        else
            retardo_ciclo_cpu();

        mover_una_posicion(admin, posX, posY);

        int bEnviar;
        void *d_enviar = serializar_envioPosicion(admin->pid, admin->tid, admin->posX, admin->posY, &bEnviar);
        send(admin->sockfd_tripulante_ram, d_enviar, bEnviar, 0);
        send(admin->sockfd_tripulante_mongo, d_enviar, bEnviar, 0);
        free(d_enviar);

        (*duracionMovimientos) -= 1;

        log_info(logger, "Tripulante en posicion X:%d Y:%d", admin->posX, admin->posY);
    }
}

void mover_una_posicion(t_admin_tripulantes *admin, uint32_t posX, uint32_t posY)
{

    int movX = posX - admin->posX;
    int movY = posY - admin->posY;

    if (movX != 0)
    {
        //REALIZAR UN MOVIMIENTO EN X
        //PUEDE SER PARA ATRAS O PARA ADELANTE
        if (movX > 0)
            admin->posX++;
        else
            admin->posX--;
    }
    else
    {
        //REALIZAR UN MOVIMIENTO EN Y
        //PUEDE SER PARA ATRAS O PARA ADELANTE
        if (movY > 0)
            admin->posY++;
        else if (movY < 0)
            admin->posY--;
    }
}

int cantMovimientos(int xInicial, int yInicial, int xFinal, int yFinal)
{
    return fabs((xFinal - xInicial)) + fabs((yFinal - yInicial));
}

void retardo_ciclo_cpu()
{
    sem_wait(&pause_exec);
    sleep(config->retardo_ciclo_cpu);
    sem_post(&pause_exec);

    return;
}

void retardo_ciclo_IO()
{
    pthread_mutex_lock(&pause_block);
    sleep(config->retardo_ciclo_cpu);
    pthread_mutex_unlock(&pause_block);

    return;
}

void retardo_ciclo_en_emergencia()
{
    sleep(config->retardo_ciclo_cpu);
}