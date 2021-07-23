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

void *serializar_TAREA(t_tarea *unaTarea)
{

    void *tarea_serializada = malloc(sizeof(t_tarea));
    int offset = 0;

    memcpy(tarea_serializada + offset, &(unaTarea->codigoTarea), sizeof(uint8_t));
    offset += sizeof(uint8_t);
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

    printf("Tcb Deserializado TID: %d\n", tcb_deserializado->TID);
    printf("Tcb Deserializado Prox inst: %d\n", tcb_deserializado->proximaInstruccion);

    return tcb_deserializado;
}

t_tarea *deserializar_TAREA(void *unaTarea)
{

    t_tarea *tarea_deserializada = malloc(sizeof(t_tarea));
    int offset = 0;

    memcpy(&(tarea_deserializada->codigoTarea), unaTarea + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(&(tarea_deserializada->parametro), unaTarea + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&(tarea_deserializada->posX), unaTarea + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&(tarea_deserializada->posY), unaTarea + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(&(tarea_deserializada->duracionTarea), unaTarea + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    return tarea_deserializada;
}