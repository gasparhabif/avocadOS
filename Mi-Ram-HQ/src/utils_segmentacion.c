#include "proceso3.h"

t_list* buscar_lista_proceso (int pid){
    
    t_list* registro_proceso;
    t_registro_segmentos *reg_seg;

    for(int i=0; i<list_size(tabla_procesos); i++){
        registro_proceso = list_get(tabla_procesos, i);
        
        for(int j=0; j<list_size(registro_proceso); j++){

            reg_seg = list_get(registro_proceso, j);

            if(reg_seg->tipo == PCB)
                if (reg_seg->id == pid)
                    return registro_proceso;
        }
    }

    return (t_list *) -1;

}

t_PCB* buscar_pcb_proceso(t_list* lista_proceso, int pid){

    t_registro_segmentos *reg_seg;
    t_PCB *pcb = malloc(sizeof(t_PCB));

    for (int i = 0; i < list_size(lista_proceso); i++)
    {
        reg_seg = list_get(lista_proceso, i);

        if(reg_seg->tipo == PCB && reg_seg->id == pid){
            
            memcpy(pcb, (void*) reg_seg->base, reg_seg->tamanio);
            
            return pcb;
            
        }
        
    }

    return (t_PCB *) -1;
}

t_registro_segmentos *buscar_registro_tcb (t_list* lista_proceso, int tid){
    
    t_registro_segmentos *reg_seg;

    for (int i = 0; i < list_size(lista_proceso); i++)
    {
        reg_seg = list_get(lista_proceso, i);

        if(reg_seg->tipo == TCB && reg_seg->id == tid)
            return reg_seg;
        
    }

    return (t_registro_segmentos *) -1;
}

t_registro_segmentos *buscar_registro_tareas (t_list* lista_proceso){
    
    t_registro_segmentos *reg_tareas;

    for (int i = 0; i < list_size(lista_proceso); i++)
    {
        reg_tareas = list_get(lista_proceso, i);

        if(reg_tareas->tipo == TAREAS)
            return reg_tareas;
        
    }

    return (t_registro_segmentos *) -1;
}

int cant_tareas (t_list* lista_proceso){

    t_registro_segmentos *reg_seg;

    for (int i = 0; i < list_size(lista_proceso); i++)
    {
        reg_seg = list_get(lista_proceso, i);

        if(reg_seg->tipo == TAREAS)
            return reg_seg->id;        
    }

    return 0;
}

int traer_tarea(void *tareas, t_list* lista_proceso, int tid, t_tarea *tarea_buscada){

    //TRAIGO EL REGISTRO DEL TCB Y LAS TAREAS
    t_registro_segmentos *reg_tcb    = buscar_registro_tcb   (lista_proceso, tid);
    t_registro_segmentos *reg_tareas = buscar_registro_tareas(lista_proceso);

    //ME COPIO EL TCB
    t_TCB *tcb = malloc(sizeof(t_TCB));
    memcpy(tcb, (void*) reg_tcb->base, reg_tcb->tamanio);

    //OBTENGO EL NUMERO DE TAREA A DEVOLVER
    if (tcb->proximaInstruccion == cant_tareas(lista_proceso)){

        tarea_buscada->tamanio_tarea = FIN_TAREAS;
        return 0;

    }
    else{

        int offsetTarea = 0;
        for (int i = 0; i < tcb->proximaInstruccion; i++){
            offsetTarea += (int) list_get(reg_tareas->tamanio_tareas, i);
        }

        //ME COPIO LA TAREA QUE NECESITO
        tarea_buscada->tamanio_tarea = (int) list_get(reg_tareas->tamanio_tareas, tcb->proximaInstruccion);
        tarea_buscada->tarea = malloc(tarea_buscada->tamanio_tarea);
        memcpy(tarea_buscada->tarea, tareas + offsetTarea, tarea_buscada->tamanio_tarea);
        
        //SUMO UNO A LA PROXIMA INSTRUCCION
        tcb->proximaInstruccion += 1;
        memcpy((void*) reg_tcb->base, tcb, reg_tcb->tamanio);
        
        return 1;
    }

}

int cant_tripulantes (t_list *lista_proceso){

    int cantTripulantes = 0;
    t_registro_segmentos *reg_seg = malloc(sizeof(t_registro_segmentos));

    for (int i = 0; i < list_size(lista_proceso); i++)
    {
        reg_seg = list_get(lista_proceso, i);

        if(reg_seg->tipo == TCB)
            cantTripulantes++;
    }

    //free(reg_seg);

    return cantTripulantes;    
}

int eliminar_tcb(t_list *lista_proceso, int tid){

    t_registro_segmentos *reg_seg = malloc(sizeof(t_registro_segmentos));
    int base;

    for (int i = 0; i < list_size(lista_proceso); i++)
    {
        reg_seg = list_get(lista_proceso, i);

        if(reg_seg->tipo == TCB && reg_seg->id == tid){
            base = (int) reg_seg->base;
            //free(reg_seg);
            list_remove(lista_proceso, i);
            return base;
        }

    }

    free(reg_seg);

    return -1;

}

int eliminar_pcbOtareas(t_list *lista_proceso, int queBorrar){

    t_registro_segmentos *reg_seg = malloc(sizeof(t_registro_segmentos));
    int base;

    for (int i = 0; i < list_size(lista_proceso); i++)
    {
        reg_seg = list_get(lista_proceso, i);

        if(reg_seg->tipo == queBorrar){
            base = (int) reg_seg->base;
            free(reg_seg);
            list_remove(lista_proceso, i);
            return base;
        }
    }

    free(reg_seg);

    return -1;
}

int obtener_PID(void *pcb){
    printf("PCB: %p\n", pcb);
    int pid;
    t_PCB *pcb_aux = malloc(sizeof(t_PCB));
    memcpy(pcb_aux, pcb, sizeof(t_PCB));
    pid = pcb_aux->PID;
    free(pcb_aux);
    return pid;
}

void eliminar_proceso(int pid){
    
    t_list* registro_proceso;

    for(int i=0; i<list_size(tabla_procesos); i++){
        registro_proceso = list_get(tabla_procesos, i);

        if(obtener_PIDproceso(registro_proceso) == pid){
            list_remove(tabla_procesos, i);
        }
    }
}

int obtener_PIDproceso(t_list *lista_proceso){

    t_registro_segmentos *reg_seg = malloc(sizeof(t_registro_segmentos));
    t_PCB *pcb = malloc(sizeof(t_PCB));

    for (int i = 0; i < list_size(lista_proceso); i++)
    {
        reg_seg = list_get(lista_proceso, i);

        if(reg_seg->tipo == PCB){
            memcpy(pcb, (void *) reg_seg->base, sizeof(t_PCB));
            //free(reg_seg);
            return pcb->PID ;
        }
    }

    //free(reg_seg);
    //free(pcb);

    return -1;
}