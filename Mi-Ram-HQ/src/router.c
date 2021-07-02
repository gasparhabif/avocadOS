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

    //GUARDO LAS TAREAS EN MEMORIA
    t_registro_segmentos* segmento_tareas = guardar_tareas(tareas_cPID_recibidas->cantTareas, tareas_cPID_recibidas->tareas);
    
    log_info(logger, "Tareas en memoria");

    //CREO EL PCB Y LO CARGO
    t_PCB *nuevo_pcb  = malloc(sizeof(t_PCB));
    nuevo_pcb->PID    = tareas_cPID_recibidas->PID;
    nuevo_pcb->tareas = (int) segmento_tareas->base;

    //GUARDO EL PCB
    t_registro_segmentos* segmento_pcb = guardar_pcb(nuevo_pcb);

    log_info(logger, "PCB en memoria");

    //CREO EL REGISTRO DE SEGMENTOS/PAGINAS DEL PROCESO
    t_list* registro_proceso = list_create();

    //CARGO EL REGISTRO DE SEGMENTOS/PAGINAS DEL PROCESO
    list_add(registro_proceso, segmento_tareas);    
    list_add(registro_proceso, segmento_pcb);

    free(nuevo_pcb);
    
    //AÑADO EL PROCESO A LA LISTA DE PROCESOS
    list_add(tabla_procesos, registro_proceso);

    //ENVIAR A DISCORDIADOR EL PUNTERO AL PCB
    int tamanioSerializacion;
    void *paquete = serializarInt((int) segmento_pcb->base, PUNTERO_PCB, &tamanioSerializacion);
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