#include "proceso3.h"

void *serializar_PCB(t_PCB *unPCB)
{

    void *pcb_serializado = malloc(sizeof(t_PCB));
    int offset = 0;

    memcpy(pcb_serializado + offset, &(unPCB->PID), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(pcb_serializado + offset, &(unPCB->tareas), sizeof(uint32_t));
    offset += sizeof(uint32_t);

    return pcb_serializado;
}

void *serializar_TCB(t_TCB unTCB)
{

    void *tcb_serializado = malloc(sizeof(t_TCB));
    int offset = 0;

    memcpy(tcb_serializado + offset, &(unTCB.TID), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(tcb_serializado + offset, &(unTCB.estado), sizeof(char));
    offset += sizeof(char);
    memcpy(tcb_serializado + offset, &(unTCB.posX), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(tcb_serializado + offset, &(unTCB.posY), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(tcb_serializado + offset, &(unTCB.proximaInstruccion), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(tcb_serializado + offset, &(unTCB.puntero_PCB), sizeof(uint32_t));
    offset += sizeof(uint32_t);

    return tcb_serializado;
}

void *serializar_pTCB(t_TCB *unTCB)
{

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

void *serializar_TAREA(char *unaTarea, int len_tarea)
{
    void *tarea_serializada = malloc(len_tarea);
    memcpy(tarea_serializada, unaTarea, len_tarea);
    return tarea_serializada;
}

t_PCB *deserializar_PCB(void *unPCB)
{

    t_PCB *pcb_deserializado = malloc(sizeof(t_PCB));
    int offset = 0;

    memcpy(&(pcb_deserializado->PID), unPCB + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&(pcb_deserializado->tareas), unPCB + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    return pcb_deserializado;
}

t_TCB *deserializar_TCB(void *unTCB)
{

    t_TCB *tcb_deserializado = malloc(sizeof(t_TCB));
    int offset = 0;

    memcpy(&(tcb_deserializado->TID), unTCB + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&(tcb_deserializado->estado), unTCB + offset, sizeof(char));
    offset += sizeof(char);
    memcpy(&(tcb_deserializado->posX), unTCB + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&(tcb_deserializado->posY), unTCB + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&(tcb_deserializado->proximaInstruccion), unTCB + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&(tcb_deserializado->puntero_PCB), unTCB + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    return tcb_deserializado;
}

t_tarea *deserializar_TAREA(void *unaTarea, int len_tarea)
{
    t_tarea *tarea_deserializada = malloc(sizeof(t_tarea));
    memcpy(&(tarea_deserializada->tamanio_tarea), &len_tarea, sizeof(uint32_t));
    tarea_deserializada->tamanio_tarea++;
    tarea_deserializada->tarea = malloc(tarea_deserializada->tamanio_tarea);
    memcpy(tarea_deserializada->tarea, unaTarea, tarea_deserializada->tamanio_tarea-1);
    tarea_deserializada->tarea[tarea_deserializada->tamanio_tarea-1] = '\0';

    return tarea_deserializada;
}