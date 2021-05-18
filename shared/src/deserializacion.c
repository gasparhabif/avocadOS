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
}

int *deserializarInt(t_buffer *buffer)
{

    int *posicion_recibida = malloc(sizeof(int));

    void *stream = buffer->stream;

    memcpy(&(posicion_recibida), stream, sizeof(int));

    return posicion_recibida;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(tarea_recibida);
}