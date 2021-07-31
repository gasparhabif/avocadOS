#include "serializacion.h"

t_TCBcPID *deserializarTCB(t_buffer *buffer)
{

    t_TCBcPID *TCB_recibido = malloc(sizeof(t_TCBcPID));

    void *stream = buffer->stream;

    memcpy(&(TCB_recibido->pid), stream, sizeof(int));
    stream += sizeof(int);
    memcpy(&(TCB_recibido->tcb.TID), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(TCB_recibido->tcb.estado), stream, sizeof(char));
    stream += sizeof(char);
    memcpy(&(TCB_recibido->tcb.posX), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(TCB_recibido->tcb.posY), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(TCB_recibido->tcb.proximaInstruccion), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(TCB_recibido->tcb.puntero_PCB), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);

    return TCB_recibido;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(TCB_recibido);
}

t_tarea *deserializarTarea(t_buffer *buffer)
{

    t_tarea *tarea_recibida = malloc(sizeof(t_tarea));

    void *stream = buffer->stream;

    memcpy(&(tarea_recibida->tamanio_tarea), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);

    if (tarea_recibida->tamanio_tarea != FIN_TAREAS)
    {    
        tarea_recibida->tarea = malloc(tarea_recibida->tamanio_tarea);
        memcpy(tarea_recibida->tarea, stream, tarea_recibida->tamanio_tarea);
    }

    return tarea_recibida;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(tarea_recibida);
}

t_tareas_cPID *deserializarTareas_cPID(t_buffer *buffer)
{
    t_tareas_cPID *tareas_cPID_recibidas = malloc(sizeof(t_tareas_cPID));

    void *stream = buffer->stream;

    memcpy(&(tareas_cPID_recibidas->PID), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(tareas_cPID_recibidas->cantTareas), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);

    tareas_cPID_recibidas->tareas = malloc(tareas_cPID_recibidas->cantTareas * sizeof(t_tarea));

    for (int i = 0; i < tareas_cPID_recibidas->cantTareas; i++)
    {
        memcpy(&(tareas_cPID_recibidas->tareas[i].tamanio_tarea), stream, sizeof(uint32_t));
        stream += sizeof(uint32_t);
        tareas_cPID_recibidas->tareas[i].tarea = malloc(tareas_cPID_recibidas->tareas[i].tamanio_tarea);
        memcpy(tareas_cPID_recibidas->tareas[i].tarea, stream, tareas_cPID_recibidas->tareas[i].tamanio_tarea);
        stream += tareas_cPID_recibidas->tareas[i].tamanio_tarea;
    }

    return tareas_cPID_recibidas;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(tarea_recibida);
}

int *deserializarInt(t_buffer *buffer)
{
    int *numero_recibido = malloc(buffer->size);
    void *stream = buffer->stream;
    memcpy(numero_recibido, stream, buffer->size);
    return numero_recibido;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(tarea_recibida);
}

t_estado *deserializar_ActulizacionEstado(t_buffer *buffer)
{

    t_estado *estado_recibido = malloc(sizeof(t_estado));

    void *stream = buffer->stream;

    memcpy(&(estado_recibido->PID), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(estado_recibido->TID), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(estado_recibido->estado), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);

    return estado_recibido;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(estado_recibido);
}

t_envio_posicion *deserializar_envioPosicion(t_buffer *buffer)
{

    t_envio_posicion *posicion_recibida = malloc(sizeof(t_envio_posicion));

    void *stream = buffer->stream;

    memcpy(&(posicion_recibida->PID), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(posicion_recibida->TID), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(posicion_recibida->pos.posX), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(posicion_recibida->pos.posY), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);

    return posicion_recibida;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(estado_recibido);
}

t_posicion *deserealizar_posicion(t_buffer *buffer)
{

    t_posicion *pos_recibida = malloc(sizeof(t_posicion));

    void *stream = buffer->stream;

    memcpy(&(pos_recibida->posX), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(pos_recibida->posY), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);

    return pos_recibida;
}

t_pidYtid *deserealizar_pidYtid(t_buffer *buffer)
{

    t_pidYtid *pid_tid_recibidos = malloc(sizeof(t_pidYtid));

    void *stream = buffer->stream;

    memcpy(&(pid_tid_recibidos->pid), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(pid_tid_recibidos->tid), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);

    return pid_tid_recibidos;
}

t_ListaTripulantes *deserealizarTCBmostrar(t_buffer *buffer)
{

    t_ListaTripulantes *listaTripulantes_recibida = malloc(sizeof(t_ListaTripulantes));

    void *stream = buffer->stream;

    memcpy(&(listaTripulantes_recibida->cantTripulantes), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);

    listaTripulantes_recibida->tripulantes = malloc(sizeof(t_TCBmostrar) * listaTripulantes_recibida->cantTripulantes);

    for (int i = 0; i < listaTripulantes_recibida->cantTripulantes; i++)
    {
        memcpy(&(listaTripulantes_recibida->tripulantes[i].PID), stream, sizeof(uint32_t));
        stream += sizeof(uint32_t);
        memcpy(&(listaTripulantes_recibida->tripulantes[i].TID), stream, sizeof(uint32_t));
        stream += sizeof(uint32_t);
        memcpy(&(listaTripulantes_recibida->tripulantes[i].estado), stream, sizeof(char));
        stream += sizeof(char);
        memcpy(&(listaTripulantes_recibida->tripulantes[i].posX), stream, sizeof(uint32_t));
        stream += sizeof(uint32_t);
        memcpy(&(listaTripulantes_recibida->tripulantes[i].posY), stream, sizeof(uint32_t));
        stream += sizeof(uint32_t);
        memcpy(&(listaTripulantes_recibida->tripulantes[i].proximaInstruccion), stream, sizeof(uint32_t));
        stream += sizeof(uint32_t);
    }

    return listaTripulantes_recibida;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(tarea_recibida);
}

t_bitacora *deserializar_bitacora(t_buffer *buffer)
{

    t_bitacora *bitacoraRecibida = malloc(sizeof(t_bitacora));

    void *stream = buffer->stream;

    memcpy(&(bitacoraRecibida->cantAcciones), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(bitacoraRecibida->tamanioAcciones), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);

    bitacoraRecibida->acciones = malloc(bitacoraRecibida->tamanioAcciones);

    for (int i = 0; i < bitacoraRecibida->cantAcciones; i++)
    {
        memcpy(&(bitacoraRecibida->acciones[i].tamanioAccion), stream, sizeof(uint32_t));
        stream += sizeof(uint32_t);
        memcpy(&(bitacoraRecibida->acciones[i].accion), stream, bitacoraRecibida->acciones[i].tamanioAccion);
        stream += bitacoraRecibida->acciones[i].tamanioAccion;
    }

    return bitacoraRecibida;
}

t_ejecutar_tarea *deserealizar_ejecutarTarea(t_buffer *buffer)
{

    t_ejecutar_tarea *ejecutarTarea_recibido = malloc(sizeof(t_ejecutar_tarea));

    void *stream = buffer->stream;

    memcpy(&(ejecutarTarea_recibido->codigoTarea), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(ejecutarTarea_recibido->parametro), stream, sizeof(uint32_t));

    return ejecutarTarea_recibido;
}

t_tidCposicion *deserializar_posicionCtid(t_buffer *buffer)
{
    t_tidCposicion *tidCpos_recibido = malloc(sizeof(t_tidCposicion));

    void *stream = buffer->stream;

    memcpy(&(tidCpos_recibido->TID), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(tidCpos_recibido->pos.posX), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(tidCpos_recibido->pos.posY), stream, sizeof(uint32_t));

    return tidCpos_recibido;
}

char *deserializar_bitacora_tripulante(t_buffer *buffer)
{
    uint32_t tamanioBitacora;

    void *stream = buffer->stream;

    memcpy(&tamanioBitacora, stream, sizeof(uint32_t));
    char *bitacora_recibida = malloc(tamanioBitacora);
    memcpy(bitacora_recibida, stream + sizeof(uint32_t), tamanioBitacora);

    return bitacora_recibida;
}
