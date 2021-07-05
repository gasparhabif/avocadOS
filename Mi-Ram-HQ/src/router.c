#include "proceso3.h"

void comenzar_patota(int client, t_tareas_cPID *tareas_cPID_recibidas)
{
    /*
    printf("\n-----UNA NUEVA PATOTA ABORDA LA NAVE-----\n");
    printf("INICIO DE PATOTA CON PID: %d\n", tareas_cPID_recibidas->PID);
    printf("CANT TAREAS: %d\n", tareas_cPID_recibidas->cantTareas);

    for (int i = 0; i < tareas_cPID_recibidas->cantTareas; i++)
    {
        printf("TAREA: %d\n",  i + 1);
        printf("CODT: %d\n",   tareas_cPID_recibidas->tareas[i].codigoTarea);
        printf("PARA: %d\n",   tareas_cPID_recibidas->tareas[i].parametro);
        printf("POSX: %d\n",   tareas_cPID_recibidas->tareas[i].posX);
        printf("POSY: %d\n",   tareas_cPID_recibidas->tareas[i].posY);
        printf("DURA: %d\n\n", tareas_cPID_recibidas->tareas[i].duracionTarea);
    }
    printf("-----------------------------------------\n\n\n");
    */

    log_info(logger, "Una nueva patota aborda la nave");

    int tamanioSerializacion;
    void *paquete;

    //GUARDO LAS TAREAS EN MEMORIA
    t_registro_segmentos* segmento_tareas = guardar_tareas(tareas_cPID_recibidas->cantTareas, tareas_cPID_recibidas->tareas);
    
    if(segmento_tareas->base != (void *) -1){

        log_info(logger, "Tareas en memoria");

        //CREO EL PCB Y LO CARGO
        t_PCB *nuevo_pcb  = malloc(sizeof(t_PCB));
        nuevo_pcb->PID    = tareas_cPID_recibidas->PID;
        nuevo_pcb->tareas = (int) segmento_tareas->base;

        //GUARDO EL PCB
        t_registro_segmentos* segmento_pcb = guardar_pcb(nuevo_pcb);

        if(segmento_pcb->base != (void *) -1){

            log_info(logger, "PCB en memoria");

            //CREO EL REGISTRO DE SEGMENTOS
            t_list* registro_proceso = list_create();

            //CARGO EL REGISTRO DE SEGMENTOS
            list_add(registro_proceso, segmento_tareas);    
            list_add(registro_proceso, segmento_pcb);

            free(nuevo_pcb);
            
            //AÑADO EL PROCESO A LA LISTA DE PROCESOS
            list_add(tabla_procesos, registro_proceso);

            //CREO EL PAQUETE A ENVIAR
            paquete = serializarInt((int) segmento_pcb->base, PUNTERO_PCB, &tamanioSerializacion);
        }
        else
            paquete = serializarInt(-1, PUNTERO_PCB, &tamanioSerializacion);
    }
    else
        paquete = serializarInt(-1, PUNTERO_PCB, &tamanioSerializacion);

    //ENVIAR A DISCORDIADOR EL PUNTERO AL PCB      
    send(client, paquete, tamanioSerializacion, 0);
    free(paquete);

    free(tareas_cPID_recibidas);
    //free(segmento_pcb);
    //free(segmento_tareas);
}

void iniciar_tripulante(int client, t_TCBcPID *tcbCpid_recibido)
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

    t_registro_segmentos* segmento_tareas = guardar_tcb(tcbCpid_recibido->tcb);

    list_add(buscar_lista_proceso(tcbCpid_recibido->pid), segmento_tareas);

    log_info(logger, "TCB en memoria");

    //LE AVISO AL TRIPULANTE QUE SUS ESTRUCTURAS YA SE ENCUENTRAN EN MEMORIA
    int tamanioSerializacion;
    void *paquete = serializarInt(1, ESTRUCTURAS_EN_MEMORIA, &tamanioSerializacion);
    send(client, paquete, tamanioSerializacion, 0);
    free(paquete);
    free(tcbCpid_recibido);

}

void solicitar_tarea(int client, t_pidYtid *pid_tid_recibidos)
{

    t_list* lista_proceso  = buscar_lista_proceso(pid_tid_recibidos->pid);
   
    t_PCB *pcb_proceso     = buscar_pcb_proceso(lista_proceso, pid_tid_recibidos->pid);
    
    t_tarea *tarea_buscada = malloc(sizeof(t_tarea));
    traer_tarea((void *) pcb_proceso->tareas, lista_proceso, pid_tid_recibidos->tid, tarea_buscada);

    //printf("codigoTarea: %d\n",   tarea_buscada->codigoTarea);
    //printf("parametro: %d\n",     tarea_buscada->parametro);
    //printf("posX: %d\n",          tarea_buscada->posX);
    //printf("posY: %d\n",          tarea_buscada->posY);
    //printf("duracionTarea: %d\n", tarea_buscada->duracionTarea);
/*
    t_tarea *tareaDePrueba = malloc(sizeof(t_tarea));
    tareaDePrueba->codigoTarea = MOVER_POSICION;
    tareaDePrueba->parametro = 3;
    tareaDePrueba->posX = 1;
    tareaDePrueba->posY = 4;
    tareaDePrueba->duracionTarea = 5;
*/
    int bEnviar;
    void *d_enviar = serializarTarea(tarea_buscada, &bEnviar);
    send(client, d_enviar, bEnviar, 0);
    free(d_enviar);

    free(tarea_buscada);
    free(pid_tid_recibidos);
}

void mover_tripulante(t_envio_posicion *pos_recibida){

    //ENCUENTRO LA LISTA DEL PROCESO
    t_list* lista_proceso  = buscar_lista_proceso(pos_recibida->PID);

    //BUSCO EL REGISTRO DEL SEGMENTO DEL TCB
    t_registro_segmentos* seg_tcb = buscar_registro_tcb(lista_proceso, pos_recibida->TID);

    //CREO UN TCB PARA TRAERLO DE MEMORIA Y TRABAJARLO
    t_TCB *tcb = malloc(sizeof(t_TCB));

    //ME COPIO EL TCB DE MEMORIA
    memcpy(tcb, seg_tcb->base, sizeof(t_TCB));

    //MODIFICO LA POSICION
    tcb->posX = pos_recibida->pos.posX;
    tcb->posY = pos_recibida->pos.posY;

    //LLEVO EL TCB NUEVAMENTE A MEMORIA
    memcpy(seg_tcb->base, tcb, sizeof(t_TCB));

    //LIBERO LA MEMORIA
    free(pos_recibida);
    free(tcb);

    return;
}

void actualizar_estado(t_estado *estadoRecibido){

    //ENCUENTRO LA LISTA DEL PROCESO
    t_list* lista_proceso  = buscar_lista_proceso(estadoRecibido->PID);

    //BUSCO EL REGISTRO DEL SEGMENTO DEL TCB
    t_registro_segmentos* seg_tcb = buscar_registro_tcb(lista_proceso, estadoRecibido->TID);

    //CREO UN TCB PARA TRAERLO DE MEMORIA Y TRABAJARLO
    t_TCB *tcb = malloc(sizeof(t_TCB));

    //ME COPIO EL TCB DE MEMORIA
    memcpy(tcb, seg_tcb->base, sizeof(t_TCB));

    //MODIFICO EL ESTADO
    tcb->estado = estadoRecibido->estado;

    //LLEVO EL TCB NUEVAMENTE A MEMORIA
    memcpy(seg_tcb->base, tcb, sizeof(t_TCB));

    //LIBERO LA MEMORIA
    free(estadoRecibido);
    free(tcb);

    return;

}

void eliminar_tripulante(t_pidYtid *tibCpid_recibido){

    //ENCUENTRO LA LISTA DEL PROCESO
    t_list* lista_proceso = buscar_lista_proceso(tibCpid_recibido->pid);

    //SI QUEDA UN SOLO TRIPULANTE ELIMINO LAS TAREAS Y EL PCB
    //SI QUEDA MAS DE UN TRIPULANTE, ELIMINO UNICAMENTE EL TCB
    if(cant_tripulantes(lista_proceso) == 1){

        int baseAEliminar;

        baseAEliminar = eliminar_pcbOtareas(lista_proceso, TAREAS);
        liberar_memoria(baseAEliminar);

        baseAEliminar = eliminar_pcbOtareas(lista_proceso, PCB);
        liberar_memoria(baseAEliminar);

    }
        
    //ELIMINO LA ENTRADA DE LA TABLA DE SEGMENTOS DEL PROCESO Y OBTENGO LA DIRECCION PARA BORRAR LA MEMORIA
    int baseTCB = eliminar_tcb(lista_proceso, tibCpid_recibido->tid);

    //LIBERO EL SEGMENTO EN LA MEMORIA
    liberar_memoria(baseTCB);

    //LIBERO LA MEMORIA
    free(tibCpid_recibido);

}

void solicitar_tripulantes(int client){

    int cantTotalTripulantes = 0;
    t_list *tablaUnProceso;
    int n = 0;

    //OBTENGO LA CANTIDAD TOTAL DE TRIPULANTES Y RESERVO UN ESPACIO PARA COPIARLOS
    for(int i=0; i < list_size(tabla_procesos); i++){
        t_list *tablaUnProceso = list_get(tabla_procesos, i);
        cantTotalTripulantes += cant_tripulantes(tablaUnProceso);
    }

    t_TCBmostrar *tcbs_enviar = malloc(cantTotalTripulantes * sizeof(t_TCBmostrar));

    //BUSCO TODOS LOS TRIPULANTES EN LA NAVE
    t_registro_segmentos *reg_seg = malloc(sizeof(t_registro_segmentos)); 
    t_TCB                *tcb_aux = malloc(sizeof(t_TCB));

    //RECORRO LA LISTA DE PROCESOS
    for(int i=0; i < list_size(tabla_procesos); i++){
        tablaUnProceso = list_get(tabla_procesos, i);

        //RECORRO LOS SEGMENTO DE CADA PROCESO
        for(int j=0; j < list_size(tablaUnProceso); j++){
            reg_seg = list_get(tablaUnProceso, j);

            //SI ENCUNETRO UN SEGMENTO QUE SEA EL TCB
            if(reg_seg->tipo == TCB){

                //ME COPIO EL TCB EN UN ESPACIO DE MEMORIA AUXILIAR (PARA TRABAJARLO)
                memcpy(tcb_aux, (void *) reg_seg->base, sizeof(t_TCB));

                //COPIO LOS DATOS QUE NECESITO
                tcbs_enviar[n].PID                = obtener_PID((void *) tcb_aux->puntero_PCB);
                tcbs_enviar[n].TID                = tcb_aux->TID;
                tcbs_enviar[n].estado             = tcb_aux->estado;
                tcbs_enviar[n].posX               = tcb_aux->posX;
                tcbs_enviar[n].posY               = tcb_aux->posY;
                tcbs_enviar[n].proximaInstruccion = tcb_aux->proximaInstruccion;
                n++;
            }
        }
    }

    printf("Encontre todos los tripulantes\n");

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