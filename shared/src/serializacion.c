#include "serializacion.h"

void* serializarTCB(t_TCB unTCB){

    //CREO EL BUFFER
    t_buffer *buffer = malloc(sizeof(t_buffer));

    //CARGO EL SIZE DEL BUFFER
    buffer->size = sizeof(uint32_t) * 5 + sizeof(char);

    //CARGO EL STREAM DEL BUFFER
    void *stream = malloc(buffer -> size);
    int offset = 0;

    memcpy(stream + offset, &unTCB.TID, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, &unTCB.estado, sizeof(char));
    offset += sizeof(char);
    memcpy(stream + offset, &unTCB.posX, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, &unTCB.posY, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, &unTCB.proximaInstruccion, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, &unTCB.puntero_PCB, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    buffer->stream = stream;

    //CREAMOS EL PAQUETE
    t_paquete* paquete = malloc(sizeof(t_paquete));

    paquete->codigo_operacion = TCB;
    paquete->buffer = buffer;

    //CREO EL STREAM A ENVIAR
    void* a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
    offset = 0;

    memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

    //LIBERO MEMORIA
    free(a_enviar);
    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);

    return a_enviar;
}
