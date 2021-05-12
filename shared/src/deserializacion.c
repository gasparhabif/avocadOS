#include "serializacion.h"

t_TCB* deserializarTCB(t_paquete* paquete){

    t_TCB *TCB_enviar = malloc(sizeof(t_TCB));

    void* stream = paquete->buffer->stream;

    memcpy(&(TCB_enviar->TID), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(TCB_enviar->estado), stream, sizeof(char));
    stream += sizeof(char);
    memcpy(&(TCB_enviar->posX), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(TCB_enviar->posY), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(TCB_enviar->proximaInstruccion), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(TCB_enviar->puntero_PCB), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);

    return TCB_enviar;

    //OJOOOO
    //free(TCB_enviar);
}

t_PCB* deserializarPCB(t_paquete* paquete){

    t_PCB *PCB_enviar = malloc(sizeof(t_PCB));
    void* stream = paquete->buffer->stream;

    memcpy(&(PCB_enviar->PID), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(PCB_enviar->tareas), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);

    return PCB_enviar;

    //OJOOOO
    //free(PCB_enviar);
}

t_tareas* deserializarTareas(t_paquete* paquete){

    t_tareas *tareas_enviar;

    int cantTareasRecibidas = paquete->buffer->size;
    void* stream = paquete->buffer->stream;

    tareas_enviar = malloc(cantTareasRecibidas * sizeof(t_tareas));

    for(int i=0; i<cantTareasRecibidas; i++){
        memcpy(&(tareas_enviar[i].codigoTarea), stream, sizeof(u_int8_t));
        stream += sizeof(u_int8_t);
        memcpy(&(tareas_enviar[i].parametro), stream, sizeof(u_int32_t));
        stream += sizeof(u_int32_t);
        memcpy(&(tareas_enviar[i].posX), stream, sizeof(u_int32_t));
        stream += sizeof(u_int32_t);
        memcpy(&(tareas_enviar[i].posY), stream, sizeof(u_int32_t));
        stream += sizeof(u_int32_t);
        memcpy(&(tareas_enviar[i].duracionTarea), stream, sizeof(u_int32_t));
        stream += sizeof(u_int32_t);
    }

    return tareas_enviar;

    //OJOOOO
    //free(tareas_enviar);
}