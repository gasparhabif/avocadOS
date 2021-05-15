#include "serializacion.h"

void* serializarTCB(t_TCB *unTCB){

    //CREO EL BUFFER
    t_buffer *buffer = malloc(sizeof(t_buffer));

    //CARGO EL SIZE DEL BUFFER
    buffer->size = sizeof(uint32_t) * 5 + sizeof(char);

    //CARGO EL STREAM DEL BUFFER
    void *stream = malloc(buffer -> size);
    int offset = 0;

    memcpy(stream + offset, &unTCB->TID, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, &unTCB->estado, sizeof(char));
    offset += sizeof(char);
    memcpy(stream + offset, &unTCB->posX, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, &unTCB->posY, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, &unTCB->proximaInstruccion, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, &unTCB->puntero_PCB, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    buffer->stream = stream;

    //CREAMOS EL PAQUETE
    t_paquete* paquete = malloc(sizeof(t_paquete));

    paquete->codigo_operacion = INICIAR_TRIPULANTE;
    paquete->buffer = buffer;

    //CREO EL STREAM A ENVIAR
    void* a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
    offset = 0;

    memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);

    return a_enviar;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(a_enviar);
}

void* serializarTarea(t_tarea unaTarea){

 
    //CREO EL BUFFER
    t_buffer *buffer = malloc(sizeof(t_buffer));

    //CARGO EL SIZE DEL BUFFER
    buffer->size = sizeof(t_tarea);

    //CARGO EL STREAM DEL BUFFER
    void *stream = malloc(sizeof(t_tarea));
    int offset = 0;

    memcpy(stream + offset, &unaTarea.codigoTarea, sizeof(u_int8_t));
    offset += sizeof(u_int8_t);
    memcpy(stream + offset, &unaTarea.parametro, sizeof(u_int32_t));
    offset += sizeof(u_int32_t);
    memcpy(stream + offset, &unaTarea.posX, sizeof(u_int32_t));
    offset += sizeof(u_int32_t);
    memcpy(stream + offset, &unaTarea.posY, sizeof(u_int32_t));
    offset += sizeof(u_int32_t);
    memcpy(stream + offset, &unaTarea.duracionTarea, sizeof(u_int32_t));
    offset += sizeof(u_int32_t);

    buffer->stream = stream;

    //CREAMOS EL PAQUETE
    t_paquete* paquete = malloc(sizeof(t_paquete));

    paquete->codigo_operacion = ENVIAR_PROXIMA_TAREA;
    paquete->buffer = buffer;

    //CREO EL STREAM A ENVIAR
    void* a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
    offset = 0;

    memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);

    return a_enviar;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(a_enviar);

}


void* serializarTareas_cPID(t_tarea *unasTareas, int patotaID, int *tamanioSerializacion){

    int cantTareas = sizeof(unasTareas) / sizeof(t_tarea);

    //CREO EL BUFFER Y LE RESERVO LA MEMORIA
    t_buffer *buffer = malloc(sizeof(t_buffer));

    //CARGO EL SIZE DEL BUFFER
    buffer->size = sizeof(unasTareas) + sizeof(uint32_t) + sizeof(uint8_t);

    *tamanioSerializacion = buffer->size;

    //CARGO EL STREAM DEL BUFFER
    void *stream = malloc(buffer -> size);
    int offset = 0;

    memcpy(stream + offset, &patotaID, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, &cantTareas, sizeof(uint8_t));
    offset += sizeof(uint32_t);
    for(int i = 0; i < cantTareas; i++){
        memcpy(stream + offset, &unasTareas[i].codigoTarea, sizeof(u_int8_t));
        offset += sizeof(u_int8_t);
        memcpy(stream + offset, &unasTareas[i].parametro, sizeof(u_int32_t));
        offset += sizeof(u_int32_t);
        memcpy(stream + offset, &unasTareas[i].posX, sizeof(u_int32_t));
        offset += sizeof(u_int32_t);
        memcpy(stream + offset, &unasTareas[i].posY, sizeof(u_int32_t));
        offset += sizeof(u_int32_t);
        memcpy(stream + offset, &unasTareas[i].duracionTarea, sizeof(u_int32_t));
        offset += sizeof(u_int32_t);
    }

    buffer->stream = stream;

    //CREAMOS EL PAQUETE
    t_paquete* paquete = malloc(sizeof(t_paquete));

    paquete->codigo_operacion = COMENZAR_PATOTA;
    paquete->buffer = buffer;

    //CREO EL STREAM A ENVIAR
    void* a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
    offset = 0;

    memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(a_enviar + offset, &(paquete->buffer->stream), paquete->buffer->size);

    //CARGO EL TAMANIO TOTAL DE LA SERIALIZACION
    *tamanioSerializacion += sizeof(uint32_t) + sizeof(uint8_t);

    //DEVUELVO LA MEMORIA UTILIZADA
    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);

    return a_enviar;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(a_enviar);
    
}

void* serializarInt(uint32_t valor, uint32_t CODIGO_OPERACION){
    
    //CREO EL BUFFER
    t_buffer *buffer = malloc(sizeof(t_buffer));

    //CARGO EL SIZE DEL BUFFER
    buffer->size = sizeof(uint32_t);

    //CARGO EL STREAM DEL BUFFER
    void *stream = malloc(buffer -> size);

    memcpy(stream, &valor, sizeof(uint32_t));

    buffer->stream = stream;

    //CREAMOS EL PAQUETE
    t_paquete* paquete = malloc(sizeof(t_paquete));

    paquete->codigo_operacion = CODIGO_OPERACION;
    paquete->buffer = buffer;

    //CREO EL STREAM A ENVIAR
    void* a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
    int offset = 0;

    memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);

    return a_enviar;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(a_enviar);
}