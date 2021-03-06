#include "proceso3.h"

void comenzar_patota_paginada(int client, t_tareas_cPID *tareas_cPID_recibidas)
{

    // for (int i = 0; i < tareas_cPID_recibidas->cantTareas; i++)
    // {
    //     printf("TAREA %d\n", i + 1);
    //     printf("Tamanio tarea %d\n", tareas_cPID_recibidas->tareas[i].tamanio_tarea);
    //     printf("Tarea: %s\n\n", tareas_cPID_recibidas->tareas[i].tarea);
    // }

    void *paquete;
    int tamanioSerializacion;
    int tamanio_tareas = 0;
    for (int i = 0; i < tareas_cPID_recibidas->cantTareas; i++)
        tamanio_tareas += tareas_cPID_recibidas->tareas[i].tamanio_tarea;

    //printf("Tamanio tareas %d\n", tamanio_tareas);

    pthread_mutex_lock(&acceso_memoria);

    if (solicitar_paginas(tamanio_tareas + sizeof(t_PCB), tareas_cPID_recibidas->PID) == -1)
        paquete = serializarInt(-1, PUNTERO_PCB, &tamanioSerializacion);
    else
    {
        guardar_tareas_pcb_paginacion(tareas_cPID_recibidas);
        paquete = serializarInt(1, PUNTERO_PCB, &tamanioSerializacion);
    }

    pthread_mutex_unlock(&acceso_memoria);

    send(client, paquete, tamanioSerializacion, 0);
    free(paquete);
}

void iniciar_tripulante_paginado(int client, t_TCBcPID *datos_recibidos, char idMapa)
{
    void *paquete;
    int tamanioSerializacion;

    pthread_mutex_lock(&acceso_memoria);

    //printf("Llego un tripulante\n");

    if (solicitar_paginas(sizeof(t_TCB), datos_recibidos->pid) == -1)
        paquete = serializarInt(-1, PUNTERO_PCB, &tamanioSerializacion);
    else
    {
        guardar_tcb_paginacion(datos_recibidos);
        paquete = serializarInt(1, PUNTERO_PCB, &tamanioSerializacion);
    }

    //DIBUJO EL TRIPULANTE EN EL MAPA
    int err;
    err = personaje_crear(level, idMapa, datos_recibidos->tcb.posX, datos_recibidos->tcb.posY);
    if (err != 0)
        log_error(logger, "Error al crear el personaje en el mapa");
    ASSERT_CREATE(level, idMapa, err);
    nivel_gui_dibujar(level);
    pthread_mutex_unlock(&acceso_memoria);

    send(client, paquete, tamanioSerializacion, 0);
    free(paquete);
}

void solicitar_tarea_paginada(int client, t_pidYtid *pidYtid_recibido)
{
    pthread_mutex_lock(&acceso_memoria);

    int err;
    t_list *tabla_proceso = obtener_lista_proceso(pidYtid_recibido->pid, &err);

    //OBTENGO EL NRO DE INSTRUCCION E INCREMENTO EL IP
    int nInstruccion;

    nInstruccion = obtener_numero_instruccion(tabla_proceso, pidYtid_recibido->pid, pidYtid_recibido->tid);

    //BUSCO LA TAREA QUE NECESITO
    //FALTA MODIFICAR ESTO
    t_tarea *tarea_recibida = obtenerTarea(tabla_proceso, pidYtid_recibido->pid, nInstruccion);

    //printf("Se envia la tarea => %s\n", tarea_recibida->tarea);
    //printf("Con un tamanio de %dB\n", tarea_recibida->tamanio_tarea);

    pthread_mutex_unlock(&acceso_memoria);

    //MANDO LA TAREA AL DISCORDIADOR
    int tamanioSerializacion;
    void *tarea_enviar = serializarTarea(tarea_recibida, ENVIAR_PROXIMA_TAREA, &tamanioSerializacion);
    send(client, tarea_enviar, tamanioSerializacion, 0);
    free(tarea_enviar);
    free(tarea_recibida);
}

void mover_tripulante_paginada(t_envio_posicion *datos_recibidos, char idMapa)
{

    //printf("Recibo la posicion -> %d|%d\n", datos_recibidos->pos.posX, datos_recibidos->pos.posY);

    pthread_mutex_lock(&acceso_memoria);

    void *elementos_proceso = recuperar_elementos_proceso(datos_recibidos->PID);
    int err;
    t_list *tabla_proceso = obtener_lista_proceso(datos_recibidos->PID, &err);
    t_tabla_paginas_proceso *elemento_proceso;
    void *tcbSerializado = malloc(sizeof(t_TCB));
    t_TCB *tcb;

    for (int i = 0; i < list_size(tabla_proceso); i++)
    {
        elemento_proceso = list_get(tabla_proceso, i);

        if (elemento_proceso->tipo == TCB && elemento_proceso->id == datos_recibidos->TID)
        {
            memcpy(tcbSerializado, elementos_proceso + elemento_proceso->offset, elemento_proceso->tamanio);

            tcb = deserializar_TCB(tcbSerializado);
            free(tcbSerializado);

            tcb->posX = datos_recibidos->pos.posX;
            tcb->posY = datos_recibidos->pos.posY;

            tcbSerializado = serializar_pTCB(tcb);
            free(tcb);

            memcpy(elementos_proceso + elemento_proceso->offset, tcbSerializado, elemento_proceso->tamanio);

            guardar_elementos_proceso(datos_recibidos->PID, elementos_proceso);
            free(elementos_proceso);
        }
    }

    //MODIFICO LA POSICION DEL TRIPULANTE EN EL MAPA
    item_mover(level, idMapa, datos_recibidos->pos.posX, datos_recibidos->pos.posY);
    nivel_gui_dibujar(level);

    pthread_mutex_unlock(&acceso_memoria);
}

void actualizar_estado_paginada(t_estado *datos_recibidos)
{
    pthread_mutex_lock(&acceso_memoria);

    void *elementos_proceso = recuperar_elementos_proceso(datos_recibidos->PID);
    int err;
    t_list *tabla_proceso = obtener_lista_proceso(datos_recibidos->PID, &err);
    t_tabla_paginas_proceso *elemento_proceso;
    void *tcbSerializado = malloc(sizeof(t_TCB));
    t_TCB *tcb;

    for (int i = 0; i < list_size(tabla_proceso); i++)
    {
        elemento_proceso = list_get(tabla_proceso, i);

        if (elemento_proceso->tipo == TCB && elemento_proceso->id == datos_recibidos->TID)
        {
            //int bytes_ocupados = elemento_proceso->offset + elemento_proceso->tamanio;
            // for (int j = elemento_proceso->offset; j < bytes_ocupados; j += config->tamanio_pagina)
            // {
            //     t_estado_frame *frame = list_get(estado_frames, j);
            //     frame->ultimo_acceso = obtener_timestamp();
            //     printf("EL ULTIMO ACCESO AL FRAME %d ES DE: %d\n", j, frame->ultimo_acceso);
            //     list_replace(estado_frames, j, frame);
            // }

            memcpy(tcbSerializado, elementos_proceso + elemento_proceso->offset, elemento_proceso->tamanio);

            tcb = deserializar_TCB(tcbSerializado);
            free(tcbSerializado);

            tcb->estado = datos_recibidos->estado;

            tcbSerializado = serializar_pTCB(tcb);
            free(tcb);

            memcpy(elementos_proceso + elemento_proceso->offset, tcbSerializado, elemento_proceso->tamanio);

            guardar_elementos_proceso(datos_recibidos->PID, elementos_proceso);

            free(elementos_proceso);
        }
    }

    pthread_mutex_unlock(&acceso_memoria);
}

void solicitar_tripulantes_paginada(int client)
{

    pthread_mutex_lock(&acceso_memoria);

    int cantTotalTripulantes = 0;
    t_list *tablaUnProceso;
    void *elementos_proceso;
    int n = 0;
    t_tabla_paginas_proceso *elemento_proceso;
    void *tcbSerializado = malloc(sizeof(t_TCB));
    t_TCB *tcb;

    for (int i = 0; i < list_size(tabla_procesos); i++)
    {
        tablaUnProceso = list_get(tabla_procesos, i);
        cantTotalTripulantes += cant_tripulantes_paginacion(tablaUnProceso);
    }

    t_TCBmostrar *tcbs_enviar = malloc(cantTotalTripulantes * sizeof(t_TCBmostrar));

    for (int i = 0; i < list_size(tabla_procesos); i++)
    {
        tablaUnProceso = list_get(tabla_procesos, i);

        elementos_proceso = recuperar_elementos_proceso(obtener_pid_pag(tablaUnProceso));

        for (int j = 0; j < list_size(tablaUnProceso); j++)
        {
            elemento_proceso = list_get(tablaUnProceso, j);

            if (elemento_proceso->tipo == TCB)
            {
                memcpy(tcbSerializado, elementos_proceso + elemento_proceso->offset, elemento_proceso->tamanio);

                tcb = deserializar_TCB(tcbSerializado);

                tcbs_enviar[n].PID = obtener_pid_pag(tablaUnProceso);
                tcbs_enviar[n].TID = tcb->TID;
                tcbs_enviar[n].estado = tcb->estado;
                tcbs_enviar[n].posX = tcb->posX;
                tcbs_enviar[n].posY = tcb->posY;
                tcbs_enviar[n].proximaInstruccion = tcb->proximaInstruccion;
                n++;
            }
        }
    }

    pthread_mutex_unlock(&acceso_memoria);

    int tamanioSerializacion;
    void *d_enviar = serializar_tcbMostrar(cantTotalTripulantes, tcbs_enviar, &tamanioSerializacion);
    send(client, d_enviar, tamanioSerializacion, 0);
    free(tcbs_enviar);
    free(d_enviar);
}

void eliminar_tripulante_paginado(t_pidYtid *datos_recibidos, char idMapa)
{

    pthread_mutex_lock(&acceso_memoria);
    if (cant_tripulantes_proceso(datos_recibidos->pid) == 1)
    {
        //ELIMINAR PCB Y TAREAS (ADEMAS DEL TCB)
        // log_error(logger, "Se elimina PATOTA %d, TRIPULANTE %d", datos_recibidos->pid, datos_recibidos->tid);

        t_pagina_proceso *pag_proceso;

        for (int i = 0; i < list_size(tabla_paginas); i++)
        {
            pag_proceso = list_get(tabla_paginas, i);

            if (pag_proceso->pid == datos_recibidos->pid)
            {

                int tamanio_lista = list_size(pag_proceso->paginas);
                // log_error(logger, "Tamanio de la lista: %d", tamanio_lista);
                //LIBERO LA MEMORIA Y ELIMINO LAS PAGINAS DEL PROCESO DE LA TABLA
                for (int j = 0; j < tamanio_lista; j++)
                {
                    int indice = ((int)list_get(pag_proceso->paginas, 0) - (int)memoria) / tamanio_paginas;
                    // log_error(logger, "El indice es: %d para el marco: %d con pid: %d\n", indice, indice, pag_proceso->pid);
                    t_estado_frame *frame = list_get(estado_frames, indice);
                    frame->pag_proc = 0;
                    frame->pid = 0;
                    frame->ocupado = 0;
                    frame->ultimo_acceso = 0;
                    list_replace(estado_frames, indice, frame);

                    list_remove(pag_proceso->paginas, 0);
                }

                //ELIMINO LA TABLA DE PAGINAS DEL PROCEO
                list_remove(tabla_paginas, i);
            }
        }

        //ELIMINO EL PROCESO DE LA TABLA DE PROCESO
        t_list *tabla_un_proceso;

        for (int i = 0; i < list_size(tabla_procesos); i++)
        {
            tabla_un_proceso = list_get(tabla_procesos, i);

            if (obtener_pid_pag(tabla_un_proceso) == datos_recibidos->pid)
            {
                list_remove(tabla_procesos, i);
            }
        }
    }
    else
    {
        //ELIMINAR TCB SOLICITADO UNICAMENTE

        // log_error(logger, "Se elimina unicamente el tripulante %d de la patota %d", datos_recibidos->tid, datos_recibidos->pid);

        int bProceso = bytes_ocupados_pid(datos_recibidos->pid);
        void *elementos_proceso_cTCB = recuperar_elementos_proceso(datos_recibidos->pid);
        void *elementos_proceso_sTCB = malloc(bProceso - sizeof(t_TCB));
        int err;
        t_list *tabla_proceso = obtener_lista_proceso(datos_recibidos->pid, &err);
        t_list *lista_paginas_proceso = obtener_paginas_proceso(datos_recibidos->pid, &err)->paginas;
        t_tabla_paginas_proceso *elemento_del_proceso;
        int index;
        int paginasNecesarias = (bProceso - sizeof(t_TCB)) / tamanio_paginas;

        // log_info(logger, "(bProceso(%d) - sizeof(t_TCB)(%d)) / tamanio_paginas(%d)", bProceso, sizeof(t_TCB), tamanio_paginas);

        if (((bProceso - sizeof(t_TCB)) % tamanio_paginas) != 0)
            paginasNecesarias++;

        // log_info(logger, "Supuestas paginas necesarias: %d para el tripulante %d en el proceso %d", paginasNecesarias, datos_recibidos->tid, datos_recibidos->pid);

        for (int i = 0; i < list_size(tabla_proceso); i++)
        {
            elemento_del_proceso = list_get(tabla_proceso, i);

            if (elemento_del_proceso->tipo == TCB && elemento_del_proceso->id == datos_recibidos->tid)
            {

                //void* tcbSerializado = malloc(sizeof(t_TCB));
                //memcpy(tcbSerializado, elementos_proceso_cTCB + elemento_del_proceso->offset, sizeof(t_TCB));
                //t_TCB *TCB = deserializar_TCB(tcbSerializado);
                //printf("%d\n", TCB->TID);
                //printf("%c\n", TCB->estado);
                //printf("%d\n", TCB->posX);
                //printf("%d\n", TCB->posY);
                //printf("%d\n", TCB->proximaInstruccion);
                //printf("%d\n", TCB->puntero_PCB);

                //ME FIJO SI HAY QUE DEVOLVER PAGINAS O NO
                // log_error(logger, "La condicion del if es pagsNecesarias %d < tamanio lista %d", paginasNecesarias, list_size(lista_paginas_proceso));
                if (paginasNecesarias < list_size(lista_paginas_proceso))
                {
                    // log_error(logger, "Estoy entrando al If que estamos chequando");
                    for (int j = list_size(lista_paginas_proceso) - 1; j > paginasNecesarias - 1; j--)
                    {
                        int indice = ((int)list_get(lista_paginas_proceso, j) - (int)memoria) / tamanio_paginas;
                        // t_estado_frame *frame = list_get(estado_frames, j);
                        t_estado_frame *frame = list_get(estado_frames, indice);
                        // log_error(logger, "2do indice es: %d para el marco: %d con pid: %d\n", indice, indice, datos_recibidos->pid);
                        frame->pag_proc = 0;
                        frame->pid = 0;
                        frame->ocupado = 0;
                        frame->ultimo_acceso = 0;
                        list_replace(estado_frames, indice, frame);
                        list_remove(lista_paginas_proceso, j);
                        // log_error(logger, "Liberando la p??gina: %d", indice);
                    }
                }

                //CAMBIAR OFFSETS A PARTIR DE ACA
                index = i;

                //ELIMINO EL TCB DE LA MEMORIA
                int offsetTCB = elemento_del_proceso->offset + elemento_del_proceso->tamanio;

                memcpy(elementos_proceso_sTCB, elementos_proceso_cTCB, elemento_del_proceso->offset);

                if ((bProceso - offsetTCB) != 0)
                {
                    memcpy(elementos_proceso_sTCB + elemento_del_proceso->offset, elementos_proceso_cTCB + offsetTCB, bProceso - offsetTCB);
                }

                // void *tcb_serializado = malloc(sizeof(t_TCB));
                // memcpy(tcb_serializado, elementos_proceso_sTCB + 48, 24);
                // t_TCB *unTCB = deserializar_TCB(tcb_serializado);
                // free(tcb_serializado);

                //ELIMINAR TCB DE LA LISTA DEL PROCESO
                list_remove(tabla_proceso, i);
            }
        }

        //CAMBIO LOS OFFSETS NECESARIOS
        for (int i = index; i < list_size(tabla_proceso); i++)
        {
            elemento_del_proceso = list_get(tabla_proceso, i);
            elemento_del_proceso->offset -= sizeof(t_TCB);
            list_replace(tabla_proceso, i, elemento_del_proceso);

            //printf("ID: %d\n", elemento_del_proceso->id);
            //printf("TIPO: %d\n", elemento_del_proceso->tipo);
            //printf("OFFSET: %d\n", elemento_del_proceso->offset);
            //printf("TAMANIO: %d\n\n", elemento_del_proceso->tamanio);
        }

        guardar_elementos_proceso(datos_recibidos->pid, elementos_proceso_sTCB);

        log_info(logger, "El tripulante %d abandono la nave", datos_recibidos->tid);
    }

    //ELIMINO AL TRIPULANTE DEL MAPA
    item_borrar(level, idMapa);
    nivel_gui_dibujar(level);
    pthread_mutex_unlock(&acceso_memoria);
}
