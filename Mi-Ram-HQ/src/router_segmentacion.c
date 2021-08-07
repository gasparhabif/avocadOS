#include "proceso3.h"

void comenzar_patota(int client, t_tareas_cPID *tareas_cPID_recibidas)
{
    //for (int i = 0; i < tareas_cPID_recibidas->cantTareas; i++)
    //{
    //    printf("Tarea %d\n", i+1);
    //    printf("Len tarea: %d\n", tareas_cPID_recibidas->tareas[i].tamanio_tarea);
    //    printf("Tarea %s\n", tareas_cPID_recibidas->tareas[i].tarea);
    //}

    log_info(logger, "Una nueva patota aborda la nave");

    int tamanioSerializacion;
    void *paquete;

    pthread_mutex_lock(&m_procesos);

    //GUARDO LAS TAREAS EN MEMORIA
    t_registro_segmentos *segmento_tareas = guardar_tareas(tareas_cPID_recibidas->cantTareas, tareas_cPID_recibidas->tareas);

    if (segmento_tareas->base != (void *)-1)
    {

        log_info(logger, "Tareas en memoria");

        //CREO EL PCB Y LO CARGO
        t_PCB *nuevo_pcb = malloc(sizeof(t_PCB));
        nuevo_pcb->PID = tareas_cPID_recibidas->PID;
        nuevo_pcb->tareas = (int)segmento_tareas->base;

        //GUARDO EL PCB
        t_registro_segmentos *segmento_pcb = guardar_pcb(nuevo_pcb);

        if (segmento_pcb->base != (void *)-1)
        {

            log_info(logger, "PCB en memoria");

            //CREO EL REGISTRO DE SEGMENTOS
            t_list *registro_proceso = list_create();

            //CARGO EL REGISTRO DE SEGMENTOS
            list_add(registro_proceso, segmento_tareas);
            list_add(registro_proceso, segmento_pcb);

            free(nuevo_pcb);

            //AÑADO EL PROCESO A LA LISTA DE PROCESOS
            list_add(tabla_procesos, registro_proceso);

            //CREO EL PAQUETE A ENVIAR
            //printf("Base PCB %d y P: %p\n", (int)segmento_pcb->base, segmento_pcb->base);

            paquete = serializarInt((int)segmento_pcb->base, PUNTERO_PCB, &tamanioSerializacion);
        }
        else
            paquete = serializarInt(-1, PUNTERO_PCB, &tamanioSerializacion);
    }
    else
        paquete = serializarInt(-1, PUNTERO_PCB, &tamanioSerializacion);

    pthread_mutex_unlock(&m_procesos);

    //ENVIAR A DISCORDIADOR EL PUNTERO AL PCB
    send(client, paquete, tamanioSerializacion, 0);
    free(paquete);

    free(tareas_cPID_recibidas);
    //free(segmento_pcb);
    //free(segmento_tareas);
}

void iniciar_tripulante(int client, t_TCBcPID *tcbCpid_recibido, char idMapaTripulante)
{
    /*
    printf("\n-----UN TRIPULANTE CON PID: %d ABORDO LA NAVE-----\n", tcbCpid_recibido->pid);
    printf("TID: %d\n", tcbCpid_recibido->tcb.TID);
    printf("EST: %c\n", tcbCpid_recibido->tcb.estado);
    printf("P_X: %d\n", tcbCpid_recibido->tcb.posX);
    printf("P_Y: %d\n", tcbCpid_recibido->tcb.posY);
    printf("PRI: %d\n", tcbCpid_recibido->tcb.proximaInstruccion);
    printf("PCB: %d\n", tcbCpid_recibido->tcb.puntero_PCB);
    printf("----------------------------------------------------\n\n\n");
    */

    pthread_mutex_lock(&m_procesos);

    t_registro_segmentos *segmento_tareas = guardar_tcb(tcbCpid_recibido->tcb);

    list_add(buscar_lista_proceso(tcbCpid_recibido->pid), segmento_tareas);

    pthread_mutex_unlock(&m_procesos);

    //DIBUJO EL TRIPULANTE EN EL MAPA
    int err;
    err = personaje_crear(level, idMapaTripulante, tcbCpid_recibido->tcb.posX, tcbCpid_recibido->tcb.posY);
    if (err != 0)
        log_error(logger, "Error al crear el personaje en el mapa");
    ASSERT_CREATE(level, idMapaTripulante, err);
    nivel_gui_dibujar(level);

    //LE AVISO AL TRIPULANTE QUE SUS ESTRUCTURAS YA SE ENCUENTRAN EN MEMORIA
    int tamanioSerializacion;
    void *paquete;

    if (segmento_tareas->base == (void *)-1)
        paquete = serializarInt(-1, ESTRUCTURAS_EN_MEMORIA, &tamanioSerializacion);
    else
        paquete = serializarInt(1, ESTRUCTURAS_EN_MEMORIA, &tamanioSerializacion);

    send(client, paquete, tamanioSerializacion, 0);
    free(paquete);
    free(tcbCpid_recibido);

    log_info(logger, "TCB en memoria");
}

void solicitar_tarea(int client, t_pidYtid *pid_tid_recibidos)
{

    pthread_mutex_lock(&m_procesos);

    t_list *lista_proceso = buscar_lista_proceso(pid_tid_recibidos->pid);

    t_PCB *pcb_proceso = buscar_pcb_proceso(lista_proceso, pid_tid_recibidos->pid);

    t_tarea *tarea_buscada = malloc(sizeof(t_tarea));
    traer_tarea((void *)pcb_proceso->tareas, lista_proceso, pid_tid_recibidos->tid, tarea_buscada);

    pthread_mutex_unlock(&m_procesos);

    //log_info(logger, "Tarea => %s", tarea_buscada->tarea);
    //log_info(logger, "Tamanio %d", tarea_buscada->tamanio_tarea);

    int bEnviar;
    void *d_enviar = serializarTarea(tarea_buscada, ENVIAR_PROXIMA_TAREA, &bEnviar);
    send(client, d_enviar, bEnviar, 0);
    free(d_enviar);

    free(tarea_buscada);
    free(pid_tid_recibidos);
}

void mover_tripulante(t_envio_posicion *pos_recibida, char idMapaTripulante)
{

    pthread_mutex_lock(&m_procesos);

    //ENCUENTRO LA LISTA DEL PROCESO
    t_list *lista_proceso = buscar_lista_proceso(pos_recibida->PID);

    //BUSCO EL REGISTRO DEL SEGMENTO DEL TCB
    t_registro_segmentos *seg_tcb = buscar_registro_tcb(lista_proceso, pos_recibida->TID);

    //CREO UN TCB PARA TRAERLO DE MEMORIA Y TRABAJARLO
    t_TCB *tcb = malloc(sizeof(t_TCB));

    //ME COPIO EL TCB DE MEMORIA
    memcpy(tcb, seg_tcb->base, sizeof(t_TCB));

    //MODIFICO LA POSICION
    tcb->posX = pos_recibida->pos.posX;
    tcb->posY = pos_recibida->pos.posY;

    //LLEVO EL TCB NUEVAMENTE A MEMORIA
    memcpy(seg_tcb->base, tcb, sizeof(t_TCB));

    pthread_mutex_unlock(&m_procesos);

    //MODIFICO LA POSICION DEL TRIPULANTE EN EL MAPA
    item_mover(level, idMapaTripulante, pos_recibida->pos.posX, pos_recibida->pos.posY);
    nivel_gui_dibujar(level);

    log_info(logger, "Se movio al tripulante a la posicion %d|%d", pos_recibida->pos.posX, pos_recibida->pos.posY);

    //LIBERO LA MEMORIA
    free(pos_recibida);
    free(tcb);

    return;
}

void actualizar_estado(t_estado *estadoRecibido)
{

    pthread_mutex_lock(&m_procesos);

    //ENCUENTRO LA LISTA DEL PROCESO
    t_list *lista_proceso = buscar_lista_proceso(estadoRecibido->PID);

    //BUSCO EL REGISTRO DEL SEGMENTO DEL TCB
    t_registro_segmentos *seg_tcb = buscar_registro_tcb(lista_proceso, estadoRecibido->TID);

    //CREO UN TCB PARA TRAERLO DE MEMORIA Y TRABAJARLO
    t_TCB *tcb = malloc(sizeof(t_TCB));

    //ME COPIO EL TCB DE MEMORIA
    memcpy(tcb, seg_tcb->base, sizeof(t_TCB));

    //MODIFICO EL ESTADO
    tcb->estado = estadoRecibido->estado;

    //LLEVO EL TCB NUEVAMENTE A MEMORIA
    memcpy(seg_tcb->base, tcb, sizeof(t_TCB));

    pthread_mutex_unlock(&m_procesos);

    //LIBERO LA MEMORIA
    free(estadoRecibido);
    free(tcb);

    return;
}

void eliminar_tripulante(t_pidYtid *pidCtid_recibido, char idMapaTripulante)
{

    pthread_mutex_lock(&m_procesos);

    //ENCUENTRO LA LISTA DEL PROCESO
    t_list *unProceso = buscar_lista_proceso(pidCtid_recibido->pid);

    //SI QUEDA UN SOLO TRIPULANTE ELIMINO LAS TAREAS Y EL PCB
    //SI QUEDA MAS DE UN TRIPULANTE, ELIMINO UNICAMENTE EL TCB
    if (cant_tripulantes(unProceso) == 1)
    {

        int baseAEliminar;

        //ELIMINO LA ENTRADA DE LA TABLA DE SEGMENTOS DEL PROCESO Y OBTENGO LA DIRECCION PARA BORRAR LA MEMORIA
        baseAEliminar = eliminar_tcb(unProceso, pidCtid_recibido->tid);

        //LIBERO EL SEGMENTO EN LA MEMORIA
        liberar_memoria(baseAEliminar);
        log_info(logger, "Se elimino el trpulante N°%d", pidCtid_recibido->tid);

        //ELIMINO LAS TAREAS DE LA MEMORIA
        baseAEliminar = eliminar_pcbOtareas(unProceso, TAREAS);
        liberar_memoria(baseAEliminar);
        log_info(logger, "Se eliminaron las tareas de la patota N°%d", pidCtid_recibido->pid);

        //ELIMINO EL PCB DE LA MEMORIA
        baseAEliminar = eliminar_pcbOtareas(unProceso, PCB);
        liberar_memoria(baseAEliminar);
        log_info(logger, "Se elimino el PCB de la patota N°%d", pidCtid_recibido->pid);

        //ELIMINO EL PROCESO DE LA TABLA DE PROCESOS
        eliminar_proceso(pidCtid_recibido->pid);
    }
    else
    {
        //ELIMINO LA ENTRADA DE LA TABLA DE SEGMENTOS DEL PROCESO Y OBTENGO LA DIRECCION PARA BORRAR LA MEMORIA
        int baseTCB = eliminar_tcb(unProceso, pidCtid_recibido->tid);

        //LIBERO EL SEGMENTO EN LA MEMORIA
        liberar_memoria(baseTCB);
        log_info(logger, "Se elimino el trpulante N°%d", pidCtid_recibido->tid);
    }

    pthread_mutex_unlock(&m_procesos);

    //ELIMINO AL TRIPULANTE DEL MAPA
    item_borrar(level, idMapaTripulante);
    nivel_gui_dibujar(level);

    //LIBERO LA MEMORIA
    free(pidCtid_recibido);
}

void solicitar_tripulantes(int client)
{

    int cantTotalTripulantes = 0;
    t_list *tablaUnProceso;
    int n = 0;

    pthread_mutex_lock(&m_procesos);

    //OBTENGO LA CANTIDAD TOTAL DE TRIPULANTES Y RESERVO UN ESPACIO PARA COPIARLOS
    for (int i = 0; i < list_size(tabla_procesos); i++)
    {
        t_list *tablaUnProceso = list_get(tabla_procesos, i);
        cantTotalTripulantes += cant_tripulantes(tablaUnProceso);
    }

    t_TCBmostrar *tcbs_enviar = malloc(cantTotalTripulantes * sizeof(t_TCBmostrar));

    //BUSCO TODOS LOS TRIPULANTES EN LA NAVE
    t_registro_segmentos *reg_seg = malloc(sizeof(t_registro_segmentos));
    t_TCB *tcb_aux = malloc(sizeof(t_TCB));

    //RECORRO LA LISTA DE PROCESOS
    for (int i = 0; i < list_size(tabla_procesos); i++)
    {
        tablaUnProceso = list_get(tabla_procesos, i);

        //RECORRO LOS SEGMENTO DE CADA PROCESO
        for (int j = 0; j < list_size(tablaUnProceso); j++)
        {
            reg_seg = list_get(tablaUnProceso, j);

            //SI ENCUNETRO UN SEGMENTO QUE SEA EL TCB
            if (reg_seg->tipo == TCB)
            {

                //ME COPIO EL TCB EN UN ESPACIO DE MEMORIA AUXILIAR (PARA TRABAJARLO)
                memcpy(tcb_aux, (void *)reg_seg->base, sizeof(t_TCB));

                //COPIO LOS DATOS QUE NECESITO
                tcbs_enviar[n].PID = obtener_PID((void *)tcb_aux->puntero_PCB);
                tcbs_enviar[n].TID = tcb_aux->TID;
                tcbs_enviar[n].estado = tcb_aux->estado;
                tcbs_enviar[n].posX = tcb_aux->posX;
                tcbs_enviar[n].posY = tcb_aux->posY;
                tcbs_enviar[n].proximaInstruccion = tcb_aux->proximaInstruccion;
                n++;
            }
        }
    }

    pthread_mutex_unlock(&m_procesos);

    //DEVUELVO MEMORIA QUE NO NECESITO
    //free(reg_seg);
    //free(tcb_aux);

    //LE DEVUELVO AL DISCODIADOR LA INFORMACION SOLICITADA
    int tamanioSerializacion;
    void *d_enviar = serializar_tcbMostrar(cantTotalTripulantes, tcbs_enviar, &tamanioSerializacion);
    send(client, d_enviar, tamanioSerializacion, 0);
    free(tcbs_enviar);
    free(d_enviar);

    return;
}