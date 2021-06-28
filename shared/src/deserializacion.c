#include "serializacion.h"

t_TCB *deserializarTCB(t_buffer *buffer)
{

    t_TCB *TCB_recibido = malloc(sizeof(t_TCB));

    void *stream = buffer->stream;

    memcpy(&(TCB_recibido->TID), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(TCB_recibido->estado), stream, sizeof(char));
    stream += sizeof(char);
    memcpy(&(TCB_recibido->posX), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(TCB_recibido->posY), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(TCB_recibido->proximaInstruccion), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(TCB_recibido->puntero_PCB), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);

    return TCB_recibido;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(TCB_recibido);
}

t_tarea *deserializarTarea(t_buffer *buffer)
{

    t_tarea *tarea_recibida = malloc(sizeof(t_tarea));

    void *stream = buffer->stream;

    memcpy(&(tarea_recibida->codigoTarea), stream, sizeof(u_int8_t));
    stream += sizeof(u_int8_t);
    memcpy(&(tarea_recibida->parametro), stream, sizeof(u_int32_t));
    stream += sizeof(u_int32_t);
    memcpy(&(tarea_recibida->posX), stream, sizeof(u_int32_t));
    stream += sizeof(u_int32_t);
    memcpy(&(tarea_recibida->posY), stream, sizeof(u_int32_t));
    stream += sizeof(u_int32_t);
    memcpy(&(tarea_recibida->duracionTarea), stream, sizeof(u_int32_t));
    stream += sizeof(u_int32_t);

    return tarea_recibida;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(tarea_recibida);
}

t_tareas_cPID *deserializarTareas_cPID(t_buffer *buffer)
{
    t_tareas_cPID *tareas_cPID_recibidas = malloc(sizeof(t_tareas_cPID));

    void *stream = buffer->stream;

    memcpy(&(tareas_cPID_recibidas->PID), stream, sizeof(u_int32_t));
    stream += sizeof(u_int32_t);
    memcpy(&(tareas_cPID_recibidas->cantTareas), stream, sizeof(u_int8_t));
    stream += sizeof(u_int8_t);

    tareas_cPID_recibidas->tareas = malloc(tareas_cPID_recibidas->cantTareas * sizeof(t_tarea));

    for (int i = 0; i < tareas_cPID_recibidas->cantTareas; i++)
    {
        memcpy(&(tareas_cPID_recibidas->tareas[i].codigoTarea), stream, sizeof(u_int8_t));
        stream += sizeof(u_int8_t);
        memcpy(&(tareas_cPID_recibidas->tareas[i].parametro), stream, sizeof(u_int32_t));
        stream += sizeof(u_int32_t);
        memcpy(&(tareas_cPID_recibidas->tareas[i].posX), stream, sizeof(u_int32_t));
        stream += sizeof(u_int32_t);
        memcpy(&(tareas_cPID_recibidas->tareas[i].posY), stream, sizeof(u_int32_t));
        stream += sizeof(u_int32_t);
        memcpy(&(tareas_cPID_recibidas->tareas[i].duracionTarea), stream, sizeof(u_int32_t));
        stream += sizeof(u_int32_t);
    }

    return tareas_cPID_recibidas;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(tarea_recibida);
}

int *deserializarInt(t_buffer *buffer)
{

    int *numero_recibido = malloc(sizeof(int));

    void *stream = buffer->stream;

    memcpy(&(numero_recibido), stream, sizeof(int));

    return numero_recibido;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(tarea_recibida);
}

t_estado *deserializar_ActulizacionEstado(t_buffer *buffer)
{

    t_estado *estado_recibido = malloc(sizeof(t_estado));

    void *stream = buffer->stream;

    memcpy(&(estado_recibido->TID), stream, sizeof(u_int32_t));
    stream += sizeof(u_int32_t);
    memcpy(&(estado_recibido->estado), stream, sizeof(u_int32_t));
    stream += sizeof(u_int32_t);

    return estado_recibido;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(estado_recibido);
}

t_envio_posicion *deserializar_envioPosicion(t_buffer *buffer)
{

    t_envio_posicion *posicion_recibida = malloc(sizeof(t_envio_posicion));

    void *stream = buffer->stream;

    memcpy(&(posicion_recibida->TID), stream, sizeof(u_int32_t));
    stream += sizeof(u_int32_t);
    memcpy(&(posicion_recibida->pos.posX), stream, sizeof(u_int32_t));
    stream += sizeof(u_int32_t);
    memcpy(&(posicion_recibida->pos.posY), stream, sizeof(u_int32_t));
    stream += sizeof(u_int32_t);

    return posicion_recibida;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(estado_recibido);
}

t_posicion *deserealizar_posicion(t_buffer *buffer){

    t_posicion *pos_recibida = malloc(sizeof(t_posicion));

    void *stream = buffer->stream;

    memcpy(&(pos_recibida->posX), stream, sizeof(u_int32_t));
    stream += sizeof(u_int32_t);
    memcpy(&(pos_recibida->posX), stream, sizeof(u_int32_t));
    stream += sizeof(u_int32_t);

    return pos_recibida;

}