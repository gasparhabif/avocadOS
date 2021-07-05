#include "proceso3.h"

void *serializar_PCB(t_PCB *unPCB){

    void *pcb_serializado = malloc(sizeof(t_PCB));
    int offset = 0;

    memcpy(pcb_serializado + offset, &(unPCB->PID), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(pcb_serializado + offset, &(unPCB->tareas), sizeof(uint32_t));
    offset += sizeof(uint32_t);

    return pcb_serializado;

}

void *serializar_TCB(t_TCB *unTCB){
    
    void *tcb_serializado = malloc(sizeof(t_TCB));
    int offset = 0;

    memcpy(tcb_serializado + offset, &(unTCB->TID), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(tcb_serializado + offset, &(unTCB->estado), sizeof(char));
    offset += sizeof(char);
    memcpy(tcb_serializado + offset, &(unTCB->posX), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(tcb_serializado + offset, &(unTCB->posY), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(tcb_serializado + offset, &(unTCB->proximaInstruccion), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(tcb_serializado + offset, &(unTCB->puntero_PCB), sizeof(uint32_t));
    offset += sizeof(uint32_t);

    return tcb_serializado;
}

void *serializar_TAREA(t_tarea *unaTarea){
    
    void *tarea_serializada = malloc(sizeof(t_tarea));
    int offset = 0;

    memcpy(tarea_serializada + offset, &(unaTarea->codigoTarea), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(tarea_serializada + offset, &(unaTarea->parametro), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(tarea_serializada + offset, &(unaTarea->posX), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(tarea_serializada + offset, &(unaTarea->posY), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(tarea_serializada + offset, &(unaTarea->duracionTarea), sizeof(uint32_t));
    offset += sizeof(uint32_t);

    return tarea_serializada;
}