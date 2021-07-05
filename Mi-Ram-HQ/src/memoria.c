#include "proceso3.h"

void* reservar_memoria(int bytes){
    
    void* posicion;

    pthread_mutex_lock(&acceso_memoria);

    if(strcmp(config->esquema_memoria, "SEGMENTACION") == 0){
        if(strcmp(config->criterio_seleccion, "FF") == 0)
            posicion = reservar_segmento_FF(bytes);
        if(strcmp(config->criterio_seleccion, "BF") == 0)
            posicion = reservar_segmento_BF(bytes);
    }
    else if(strcmp(config->esquema_memoria, "PAGINACION") == 0){
            
    }

    pthread_mutex_unlock(&acceso_memoria);

    return posicion;
}

void liberar_memoria (int liberar){
        
    pthread_mutex_lock(&acceso_memoria);

    if(strcmp(config->esquema_memoria, "SEGMENTACION") == 0)
        liberar_memoria_segmentacion(liberar);
    else if(strcmp(config->esquema_memoria, "PAGINACION") == 0){}

    pthread_mutex_unlock(&acceso_memoria);
}

t_registro_segmentos* guardar_tareas(int cantTareas, t_tarea *tareas_recibidas){

    //RESERVO EL SEGMENTO PARA LAS TAREAS 
    void *pos_tareas  = reservar_memoria(cantTareas * sizeof(t_tarea));

    //COPIO LAS TAREAS EN LA POSICION RESERVADA
    memcpy(pos_tareas, tareas_recibidas, cantTareas * sizeof(t_tarea));

    //CREO EL REGISTRO PARA LUEGO AÑADIR A LA LISTA DEL PROCESO
    t_registro_segmentos *segmento_tareas = malloc(sizeof(t_registro_segmentos));

    //LLENO EL SEGMENTO DE LAS TAREAS
    segmento_tareas->base     = pos_tareas;
    segmento_tareas->tamanio  = sizeof(t_tarea) * cantTareas;
    segmento_tareas->tipo     = TAREAS;
    segmento_tareas->id       = cantTareas;

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
    segmento_pcb->base     = pcb;
    segmento_pcb->tamanio  = sizeof(t_PCB);
    segmento_pcb->tipo     = PCB;
    segmento_pcb->id       = pcb_recibido->PID;

    return segmento_pcb;
}

t_registro_segmentos* guardar_tcb(t_TCB tcb_recibido){

    //RESERVO MEMORIA PARA EL TCB
    void *tcb = reservar_memoria(sizeof(t_TCB));

    //COPIO EL PCB EN LA POSICION RESERVADA
    memcpy(tcb, &tcb_recibido, sizeof(t_TCB));

    //CREO EL REGISTRO PARA LUEGO AÑADIR A LA LISTA DEL PROCESO
    t_registro_segmentos *segmento_tcb = malloc(sizeof(t_registro_segmentos));
    
    //LLENO EL SEGMENTO DEL PCB
    segmento_tcb->base     = tcb;
    segmento_tcb->tamanio  = sizeof(t_TCB);
    segmento_tcb->tipo     = TCB;
    segmento_tcb->id       = tcb_recibido.TID;

    return segmento_tcb;

}