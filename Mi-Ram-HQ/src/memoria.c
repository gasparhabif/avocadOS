#include "proceso3.h"

void* reservar_memoria(int bytes){
    
    void* posicion;

    pthread_mutex_lock(&acceso_memoria);

    if(strcmp(config->esquema_memoria, "SEGMENTACION") == 0){

        posicion = reservar_segmento_FF(bytes);

        /*
        if(strcmp(config->criterio_seleccion, "FF") == 0)
            posicion = reservar_segmento_FF(bytes);
        if(strcmp(config->criterio_seleccion, "BF") == 0)
            posicion = reservar_segmento_BF(bytes);
        */
    }
    else if(strcmp(config->esquema_memoria, "PAGINACION") == 0){
            
    }

    pthread_mutex_unlock(&acceso_memoria);

    return posicion;
}

t_registro_segmentos* guardar_tareas(int cantTareas, t_tarea *tareas_recibidas){
    
    printf("Guardando tareas\n");

    //RESERVO EL SEGMENTO PARA LAS TAREAS 
    void *pos_tareas  = reservar_memoria(cantTareas * sizeof(t_tarea));

    printf("Posicion reservada %p\n", pos_tareas);
    
    //COPIO LAS TAREAS EN LA POSICION RESERVADA
    memcpy(pos_tareas, tareas_recibidas, cantTareas * sizeof(t_tarea));

    printf("Tareas copiadas\n");

    //CREO EL REGISTRO PARA LUEGO AÑADIR A LA LISTA DEL PROCESO
    t_registro_segmentos *segmento_tareas = malloc(sizeof(t_registro_segmentos));

    //LLENO EL SEGMENTO DE LAS TAREAS
    segmento_tareas->segmento = 2;
    segmento_tareas->base     = pos_tareas;
    segmento_tareas->tamanio  = sizeof(t_PCB);
    segmento_tareas->tipo     = TAREAS;
    segmento_tareas->tid      = -1;
    segmento_tareas->usado    = 1;

    printf("Tareas en memoria :)\n");

    return segmento_tareas;
}

t_registro_segmentos* guardar_pcb(t_PCB *pcb_recibido){

    //RESERVO MEMORIA PARA EL PCB
    void *pcb = reservar_memoria(sizeof(t_PCB));
    
    printf("Posicion reservada %p\n", pcb);

    //COPIO EL PCB EN LA POSICION RESERVADA
    memcpy(pcb, pcb_recibido, sizeof(t_PCB));

    printf("PCB copiado\n");

    //CREO EL REGISTRO PARA LUEGO AÑADIR A LA LISTA DEL PROCESO
    t_registro_segmentos *segmento_pcb = malloc(sizeof(t_registro_segmentos));
    
    //LLENO EL SEGMENTO DEL PCB
    segmento_pcb->segmento = 1;
    segmento_pcb->base     = pcb;
    segmento_pcb->tamanio  = sizeof(t_PCB);
    segmento_pcb->tipo     = PCB;
    segmento_pcb->tid      = -1;
    segmento_pcb->usado    = 1;

    printf("PCB en memoria :)\n");

    return segmento_pcb;
}

t_registro_segmentos* guardar_tcb(t_TCB *tcb_recibido){

    //RESERVO MEMORIA PARA EL TCB
    void *tcb = reservar_memoria(sizeof(t_TCB));
    
    printf("Posicion reservada %p\n", tcb);

    //COPIO EL PCB EN LA POSICION RESERVADA
    memcpy(tcb, tcb_recibido, sizeof(t_TCB));

    printf("TCB copiado\n");

    //CREO EL REGISTRO PARA LUEGO AÑADIR A LA LISTA DEL PROCESO
    t_registro_segmentos *segmento_tcb = malloc(sizeof(t_registro_segmentos));
    
    //LLENO EL SEGMENTO DEL PCB
    segmento_pcb->segmento = 0; //TODO
    segmento_pcb->base     = tcb;
    segmento_pcb->tamanio  = sizeof(t_TCB);
    segmento_pcb->tipo     = TCB;
    segmento_pcb->tid      = tcb_recibido->tid;
    segmento_pcb->usado    = 1;

    printf("TCB en memoria :)\n");

    return segmento_tcb;

}