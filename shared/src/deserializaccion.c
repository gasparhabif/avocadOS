#include "serializacion.h"

void* deserializarTCB(t_paquete* paquete){

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
    //stream += sizeof(uint32_t);

    return TCB_enviar;
}
    //OJOOOO
    //free(TCB_enviar);