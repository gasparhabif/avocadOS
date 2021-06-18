#include "proceso3.h"

void* reservar_memoria(int bytes){

    pthread_mutex_lock(&acceso_memoria);

    if(strcmp(config->esquema_memoria, "SEGMENTACION") == 0){
        if(strcmp(config->esquema_memoria, "FF") == 0)
            return reservar_segmento_FF(bytes);
        if(strcmp(config->esquema_memoria, "BF") == 0)
            return reservar_segmento_BF(bytes);
    }
    else if(strcmp(config->esquema_memoria, "PAGINACION") == 0){
            return reservar_pagina(bytes);
    }

    pthread_mutex_unlock(&acceso_memoria);

    return (void*) -1;
}

t_registro_segmentos* guardar_tareas(int cantTareas, t_tarea *tareas_recibidas){
    
    printf("Guardando tareas\n");

    //RESERVO EL SEGMENTO PARA LAS TAREAS 
    void *pos_tareas  = reservar_memoria(cantTareas * sizeof(t_tarea));
    
    //COPIO LAS TAREAS EN LA POSICION RESERVADA
    memcpy(pos_tareas, tareas_recibidas, cantTareas * sizeof(t_tarea));

    //CREO EL REGISTRO PARA LUEGO AÑADIR A LA LISTA DEL PROCESO
    t_registro_segmentos *segmento_tareas = malloc(sizeof(t_registro_segmentos));

    //LLENO EL SEGMENTO DE LAS TAREAS
    segmento_tareas->segmento             = 2;
    segmento_tareas->base                 = pos_tareas;
    segmento_tareas->tamanio              = sizeof(t_PCB);
    segmento_tareas->tipo                 = TAREAS;
    segmento_tareas->tid                  = -1;
    segmento_tareas->usado                = 1;

    return segmento_tareas;
}

t_registro_segmentos* guardar_pcb(t_PCB *pcb_recibido){

    //RESERVO MEMORIA PARA EL PCB
    void *pcb = reservar_memoria(sizeof(t_PCB));
    
    //COPIO EL PCB EN LA POSICION RESERVADA
    memcpy(pcb, pcb_recibido, sizeof(t_PCB));

    //CREO EL REGISTRO PARA LUEGO AÑADIR A LA LISTA DEL PROCESO
    t_registro_segmentos *segmento_pcb = malloc(sizeof(t_registro_segmentos));
    
    //LLENO EL SEGMENTO DEL PCB
    segmento_pcb->segmento    = 1;
    segmento_pcb->base        = pcb;
    segmento_pcb->tamanio     = sizeof(t_PCB);
    segmento_pcb->tipo        = PCB;
    segmento_pcb->tid         = -1;
    segmento_pcb->usado       = 1;

    return segmento_pcb;
}

t_registro_segmentos* guardar_tcb(t_TCB tcb_recibido){

}