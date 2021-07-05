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

int cant_tareas (t_list* lista_proceso){

    t_registro_segmentos *reg_seg;

    for (int i = 0; i < list_size(lista_proceso); i++)
    {
        reg_seg = list_get(lista_proceso, i);

        if(reg_seg->tipo == TAREAS)
            return (reg_seg->tamanio / sizeof(t_tarea));        
    }

    return 0;
}

int traer_tarea(void *tareas, t_list* lista_proceso, int tid, t_tarea *tarea_buscada){

    //TRAIGO EL REGISTRO DEL TCB
    t_registro_segmentos *reg_tcb = buscar_registro_tcb(lista_proceso, tid);
    
    //ME COPIO EL TCB
    t_TCB *tcb = malloc(sizeof(t_TCB));
    memcpy(tcb, (void*) reg_tcb->base, reg_tcb->tamanio);

    //OBTENGO EL NUMERO DE TAREA A DEVOLVER
    if (tcb->proximaInstruccion == cant_tareas(lista_proceso)){

        tarea_buscada->codigoTarea = FIN_TAREAS;

        return 0;

    }
    else{

        //ME COPIO LA TAREA QUE NECESITO
        memcpy(tarea_buscada, tareas + (tcb->proximaInstruccion * sizeof(t_tarea)), sizeof(t_tarea));
        
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
            free(reg_seg);
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
    int pid;
    t_PCB *pcb_aux = malloc(sizeof(t_PCB));
    memcpy(pcb_aux, pcb, sizeof(t_PCB));
    pid = pcb_aux->PID;
    free(pcb_aux);
    return pid;
}