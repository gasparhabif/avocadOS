#include "proceso3.h"

void comenzar_patota_paginada(int client, t_tareas_cPID *tareas_cPID_recibidas)
{
    int   tamanio_tareas = sizeof(t_tarea) * tareas_cPID_recibidas->cantTareas;
    void *paquete;
    int   tamanioSerializacion;

    
    pthread_mutex_lock(&acceso_memoria);

    if (solicitar_paginas(tamanio_tareas + sizeof(t_PCB), tareas_cPID_recibidas->PID) == -1)
        paquete = serializarInt(-1, PUNTERO_PCB, &tamanioSerializacion);
    else
    {
        guardar_tareas_pcb_paginacion(tareas_cPID_recibidas);

        //guardar_pcb_paginacion(tareas_cPID_recibidas->PID, /*DIRECCION TAREAS*/);

        paquete = serializarInt(1, PUNTERO_PCB, &tamanioSerializacion);
    }

    pthread_mutex_unlock(&acceso_memoria);

    send(client, paquete, tamanioSerializacion, 0);
    free(paquete);
}

void iniciar_tripulante_paginado(int client, t_TCBcPID *datos_recibidos)
{
    void *paquete;
    int tamanioSerializacion;

    pthread_mutex_lock(&acceso_memoria);

    if (solicitar_paginas(sizeof(t_TCB), datos_recibidos->pid) == -1)
        paquete = serializarInt(-1, PUNTERO_PCB, &tamanioSerializacion);
    else
    {
        guardar_tcb_paginacion(datos_recibidos);
        paquete = serializarInt(1, PUNTERO_PCB, &tamanioSerializacion);
    }

    pthread_mutex_unlock(&acceso_memoria);

    send(client, paquete, tamanioSerializacion, 0);
    free(paquete);
}

void solicitar_tarea_paginada(int client, t_pidYtid *pidYtid_recibido)
{
    pthread_mutex_lock(&acceso_memoria);

    int err;
    t_list *tabla_proceso    = obtener_lista_proceso(pidYtid_recibido->pid, &err);

    //OBTENGO EL NRO DE INSTRUCCION E INCREMENTO EL IP
    int nInstruccion         = obtener_numero_instruccion(tabla_proceso, pidYtid_recibido->pid, pidYtid_recibido->tid);
    
    //BUSCO LA TAREA QUE NECESITO
    t_tarea *tarea_recibida  = obtenerTarea(tabla_proceso, pidYtid_recibido->pid, nInstruccion);

    pthread_mutex_unlock(&acceso_memoria);

    //MANDO LA TAREA AL DISCORDIADOR
    int tamanioSerializacion;
    void *tarea_enviar = serializarTarea(tarea_recibida, &tamanioSerializacion);
    send(client, tarea_enviar, tamanioSerializacion, 0);
    free(tarea_enviar);
    free(tarea_recibida);
}

void mover_tripulante_paginada(t_envio_posicion *datos_recibidos)
{

    printf("Recibo la posicion -> %d|%d\n", datos_recibidos->pos.posX, datos_recibidos->pos.posY);

    pthread_mutex_lock(&acceso_memoria);

    void *elementos_proceso = recuperar_elementos_proceso(datos_recibidos->PID);
    int err;
    t_list *tabla_proceso    = obtener_lista_proceso(datos_recibidos->PID, &err);
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

            printf("Cambio una posicion\n");

            tcbSerializado = serializar_pTCB(tcb);
            free(tcb);

            memcpy(elementos_proceso + elemento_proceso->offset, tcbSerializado, elemento_proceso->tamanio);

            guardar_elementos_proceso(datos_recibidos->PID, elementos_proceso);

        }
    }

    pthread_mutex_unlock(&acceso_memoria);
}

void actualizar_estado_paginada(t_estado *datos_recibidos)
{
    pthread_mutex_lock(&acceso_memoria);

    void *elementos_proceso = recuperar_elementos_proceso(datos_recibidos->PID);
    int err;
    t_list *tabla_proceso    = obtener_lista_proceso(datos_recibidos->PID, &err);
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

            tcb->estado = datos_recibidos->estado;

            tcbSerializado = serializar_pTCB(tcb);
            free(tcb);

            memcpy(elementos_proceso + elemento_proceso->offset, tcbSerializado, elemento_proceso->tamanio);

            guardar_elementos_proceso(datos_recibidos->PID, elementos_proceso);

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

                tcbs_enviar[n].PID                = obtener_pid_pag(tablaUnProceso);
                tcbs_enviar[n].TID                = tcb->TID;
                tcbs_enviar[n].estado             = tcb->estado;
                tcbs_enviar[n].posX               = tcb->posX;
                tcbs_enviar[n].posY               = tcb->posY;
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

void eliminar_tripulante_paginado(t_pidYtid *datos_recibidos)
{

    pthread_mutex_lock(&acceso_memoria);

    if(cant_tripulantes_proceso(datos_recibidos->pid) == 1){
        //ELIMINAR PCB Y TAREAS (ADEMAS DEL TCB)
        
        t_pagina_proceso *pag_proceso;
            
        for (int i = 0; i < list_size(tabla_paginas); i++)
        {
            pag_proceso = list_get(tabla_paginas, i);

            if(pag_proceso->pid == datos_recibidos->pid){

                //LIBERO LA MEMORIA Y ELIMINO LAS PAGINAS DEL PROCESO DE LA TABLA
                for (int j = 0; j < list_size(pag_proceso->paginas); j++){
                    estado_frames[((int) list_get(pag_proceso->paginas, j) - (int) memoria) / tamanio_paginas] = 0;
                    free(list_remove(pag_proceso->paginas, j));
                }

                //ELIMINO LA TABLA DE PAGINAS DEL PROCEO
                free(list_remove(tabla_paginas, i));
            }
        }

        //ELIMINO EL PROCESO DE LA TABLA DE PROCESO
        t_list *tabla_un_proceso;

        for (int i = 0; i < list_size(tabla_procesos); i++)
        {
            tabla_un_proceso = list_get(tabla_procesos, i);

            if(obtener_pid_pag(tabla_un_proceso) == datos_recibidos->pid){
                for(int j=0; j<list_size(tabla_un_proceso); j++)
                    free(list_remove(tabla_un_proceso, j));

                
                list_remove(tabla_procesos, i);
            }
        }       
    }
    else{
        //ELIMINAR TCB SOLICITADO

        
    }

    pthread_mutex_unlock(&acceso_memoria);
}
