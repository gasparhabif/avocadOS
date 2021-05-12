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

    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);

    return a_enviar;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(a_enviar);
}

void* serializarPCB(t_PCB unPCB){

    //CREO EL BUFFER
    t_buffer *buffer = malloc(sizeof(t_buffer));

    //CARGO EL SIZE DEL BUFFER
    buffer->size = sizeof(uint32_t) * 2;

    //CARGO EL STREAM DEL BUFFER
    void *stream = malloc(buffer -> size);
    int offset = 0;

    memcpy(stream + offset, &unPCB.PID, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, &unPCB.tareas, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    buffer->stream = stream;

    //CREAMOS EL PAQUETE
    t_paquete* paquete = malloc(sizeof(t_paquete));

    paquete->codigo_operacion = PCB;
    paquete->buffer = buffer;

    //CREO EL STREAM A ENVIAR
    void* a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
    offset = 0;

    memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(a_enviar + offset, &(paquete->buffer->stream), paquete->buffer->size);

    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);

    return a_enviar;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(a_enviar);
    
}


void* serializarTareas(t_tareas *unasTareas){

    // PARA SERIALIZAR TAREAS EL SIZE DEL BUFFER VA A SER LA CANTIDAD DE TAREAS, LUEGO EN EL
    // SIZE DEL STREAM SERÁ LA CANTIDAD DE TAREAS MULTIPLICADO POR EL SIZEOF DE T_TAREA.
    // DICHO DE OTRA FORMA, EN ESTA SERIALIZACION UTILIZO EL SIZE DEL BUFFER COMO CANTTAREAS.

    int cantTareas = sizeof(unasTareas) / sizeof(t_tareas);

    //CREO EL BUFFER
    t_buffer *buffer = malloc(sizeof(t_buffer));

    //CARGO EL SIZE DEL BUFFER
    buffer->size = cantTareas;

    //CARGO EL STREAM DEL BUFFER
    void *stream = malloc(sizeof(unasTareas));
    int offset = 0;

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

    paquete->codigo_operacion = TAREAS;
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


void* serializarTareas_cPID(t_tareas *unasTareas, int patotaID){
    
    // PARA SERIALIZAR TAREAS EL SIZE DEL BUFFER VA A SER LA CANTIDAD DE TAREAS, LUEGO EN EL
    // SIZE DEL STREAM SERÁ LA CANTIDAD DE TAREAS MULTIPLICADO POR EL SIZEOF DE T_TAREA.
    // DICHO DE OTRA FORMA, EN ESTA SERIALIZACION UTILIZO EL SIZE DEL BUFFER COMO CANTTAREAS.
    // ADEMAS DEL SIZE HAY UN CAMPO PARA ENVIAR EL PID (patota ID).

    int cantTareas = sizeof(unasTareas) / sizeof(t_tareas);

    //CREO EL BUFFER
    t_buffer_PID *buffer_cPID = malloc(sizeof(t_buffer_PID));

    //CARGO EL SIZE DEL BUFFER
    buffer_cPID->size = cantTareas;

    //CARGO EL PATOTA ID
    buffer_cPID->PID = patotaID;

    //CARGO EL STREAM DEL BUFFER
    void *stream = malloc(sizeof(unasTareas));
    int offset = 0;

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

    buffer_cPID->stream = stream;

    //CREAMOS EL PAQUETE
    t_paquete_PID* paquete = malloc(sizeof(t_paquete));

    paquete->codigo_operacion = TAREAS_cPID;
    paquete->buffer = buffer_cPID;

    //CREO EL STREAM A ENVIAR
    void* a_enviar = malloc(buffer_cPID->size + sizeof(uint8_t) + sizeof(uint32_t) * 2);
    offset = 0;

    memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(a_enviar + offset, &(paquete->buffer->PID), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);

    return a_enviar;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(a_enviar);
}

//DESEREALIZACION
/*
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

*/