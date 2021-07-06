#include "serializacion.h"

void *serializarTCB(uint32_t pid, t_TCB unTCB, int *tamanioSerializacion)
{

    //CREO EL BUFFER
    t_buffer *buffer = malloc(sizeof(t_buffer));

    //CARGO EL SIZE DEL BUFFER
    buffer->size = sizeof(t_TCB) + sizeof(int);

    //CARGO EL TAMAÑO SE LA SERIALIZACION (PARA QUE EL SEND SE PUEDA REALIZAR CORRECTAMENTE)
    *tamanioSerializacion = buffer->size + sizeof(uint32_t) + sizeof(uint8_t);

    //CARGO EL STREAM DEL BUFFER
    void *stream = malloc(buffer->size);
    int offset = 0;

    memcpy(stream + offset, &pid, sizeof(uint32_t));
    offset += sizeof(uint32_t);
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
    t_paquete *paquete = malloc(sizeof(t_paquete));

    paquete->codigo_operacion = INICIAR_TRIPULANTE;
    paquete->buffer = buffer;

    //CREO EL STREAM A ENVIAR
    void *a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
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

void *serializarTarea(t_tarea *unaTarea, int *tamanioSerializacion)
{

    //CREO EL BUFFER
    t_buffer *buffer = malloc(sizeof(t_buffer));

    //CARGO EL SIZE DEL BUFFER
    buffer->size = sizeof(t_tarea);

    //CARGO EL TAMAÑO SE LA SERIALIZACION (PARA QUE EL SEND SE PUEDA REALIZAR CORRECTAMENTE)
    *tamanioSerializacion = buffer->size + sizeof(uint32_t) + sizeof(uint8_t);

    //CARGO EL STREAM DEL BUFFER
    void *stream = malloc(sizeof(t_tarea));
    int offset = 0;

    memcpy(stream + offset, &(unaTarea->codigoTarea), sizeof(u_int8_t));
    offset += sizeof(u_int8_t);
    memcpy(stream + offset, &(unaTarea->parametro), sizeof(u_int32_t));
    offset += sizeof(u_int32_t);
    memcpy(stream + offset, &(unaTarea->posX), sizeof(u_int32_t));
    offset += sizeof(u_int32_t);
    memcpy(stream + offset, &(unaTarea->posY), sizeof(u_int32_t));
    offset += sizeof(u_int32_t);
    memcpy(stream + offset, &(unaTarea->duracionTarea), sizeof(u_int32_t));
    offset += sizeof(u_int32_t);

    buffer->stream = stream;

    //CREAMOS EL PAQUETE
    t_paquete *paquete = malloc(sizeof(t_paquete));

    paquete->codigo_operacion = ENVIAR_PROXIMA_TAREA;
    paquete->buffer = buffer;

    //CREO EL STREAM A ENVIAR
    void *a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
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

void *serializarTareas_cPID(t_tarea *unasTareas, int patotaID, int *tamanioSerializacion, int cantTareas)
{
    //CREO EL BUFFER Y LE RESERVO LA MEMORIA
    t_buffer *buffer = malloc(sizeof(t_buffer));

    //CARGO EL SIZE DEL BUFFER
    // Cantidad de Tareas * Tamaño Tareas + PID + Cant Tareas
    buffer->size = (sizeof(t_tarea) * cantTareas) + sizeof(uint32_t) + sizeof(uint8_t);

    *tamanioSerializacion = buffer->size + sizeof(uint32_t) + sizeof(uint8_t);

    //CARGO EL STREAM DEL BUFFER
    buffer->stream = malloc(buffer->size);

    int offset = 0;

    memcpy(buffer->stream + offset, &patotaID, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(buffer->stream + offset, &cantTareas, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    for (int i = 0; i < cantTareas; i++)
    {
        memcpy(buffer->stream + offset, &unasTareas[i].codigoTarea, sizeof(uint8_t));
        offset += sizeof(uint8_t);
        memcpy(buffer->stream + offset, &(unasTareas[i].parametro), sizeof(uint32_t));
        offset += sizeof(uint32_t);
        memcpy(buffer->stream + offset, &(unasTareas[i].posX), sizeof(uint32_t));
        offset += sizeof(uint32_t);
        memcpy(buffer->stream + offset, &(unasTareas[i].posY), sizeof(uint32_t));
        offset += sizeof(uint32_t);
        memcpy(buffer->stream + offset, &(unasTareas[i].duracionTarea), sizeof(uint32_t));
        offset += sizeof(uint32_t);
    }

    //CREAMOS EL PAQUETE
    t_paquete *paquete = malloc(sizeof(t_paquete));

    //buffer->stream = stream;

    // Cantidad de Tareas * Tamaño Tareas + Cant Tareas + PID + Size del Stream
    //paquete->buffer = malloc(cantTareas * sizeof(t_tarea) + sizeof(u_int8_t) + sizeof(u_int32_t) + sizeof(u_int32_t));

    paquete->codigo_operacion = COMENZAR_PATOTA;
    paquete->buffer = buffer;

    //CREO EL STREAM A ENVIAR
    void *a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
    offset = 0;

    memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

    //DEVUELVO LA MEMORIA UTILIZADA
    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);

    return a_enviar;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(a_enviar);
}

void *serializarInt(uint32_t valor, uint8_t CODIGO_OPERACION, int *tamanioSerializacion)
{

    //CREO EL BUFFER
    t_buffer *buffer = malloc(sizeof(t_buffer));

    //CARGO EL SIZE DEL BUFFER
    buffer->size = sizeof(uint32_t);

    //CARGO EL TAMAÑO SE LA SERIALIZACION (PARA QUE EL SEND SE PUEDA REALIZAR CORRECTAMENTE)
    *tamanioSerializacion = buffer->size + sizeof(uint32_t) + sizeof(uint8_t);

    //CARGO EL STREAM DEL BUFFER
    void *stream = malloc(buffer->size);

    memcpy(stream, &valor, sizeof(uint32_t));

    buffer->stream = stream;

    //CREAMOS EL PAQUETE
    t_paquete *paquete = malloc(sizeof(t_paquete));

    paquete->codigo_operacion = CODIGO_OPERACION;
    paquete->buffer = buffer;

    //CREO EL STREAM A ENVIAR
    void *a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
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

void *serializar_ActulizacionEstado(uint32_t pid, uint32_t tid, char nuevoEstado, int *tamanioSerializacion)
{

    //CREO EL BUFFER
    t_buffer *buffer = malloc(sizeof(t_buffer));

    //CARGO EL SIZE DEL BUFFER
    buffer->size = sizeof(uint32_t) * 2 + sizeof(char);

    //CARGO EL TAMAÑO SE LA SERIALIZACION (PARA QUE EL SEND SE PUEDA REALIZAR CORRECTAMENTE)
    *tamanioSerializacion = buffer->size + sizeof(uint32_t) + sizeof(uint8_t);

    //CARGO EL STREAM DEL BUFFER
    void *stream = malloc(buffer->size);
    int offset = 0;

    memcpy(stream + offset, &pid, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, &tid, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, &nuevoEstado, sizeof(char));
    offset += sizeof(char);

    buffer->stream = stream;

    //CREAMOS EL PAQUETE
    t_paquete *paquete = malloc(sizeof(t_paquete));

    paquete->codigo_operacion = ACTUALIZAR_ESTADO;
    paquete->buffer = buffer;

    //CREO EL STREAM A ENVIAR
    void *a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
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

void *serializar_envioPosicion(uint32_t pid, uint32_t tid, uint32_t posX, uint32_t posY, int *tamanioSerializacion)
{

    //CREO EL BUFFER
    t_buffer *buffer = malloc(sizeof(t_buffer));

    //CARGO EL SIZE DEL BUFFER
    buffer->size = sizeof(uint32_t) * 2 + sizeof(t_posicion);

    //CARGO EL TAMAÑO SE LA SERIALIZACION (PARA QUE EL SEND SE PUEDA REALIZAR CORRECTAMENTE)
    *tamanioSerializacion = buffer->size + sizeof(uint32_t) + sizeof(uint8_t);

    //CARGO EL STREAM DEL BUFFER
    void *stream = malloc(buffer->size);
    int offset = 0;

    memcpy(stream + offset, &pid, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, &tid, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, &posX, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, &posY, sizeof(uint32_t));

    buffer->stream = stream;

    //CREAMOS EL PAQUETE
    t_paquete *paquete = malloc(sizeof(t_paquete));
    paquete->codigo_operacion = MOVER_TRIPULANTE;
    paquete->buffer = buffer;

    //CREO EL STREAM A ENVIAR
    void *a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
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

void *serializar_envioSabotaje(uint32_t posX, uint32_t posY, int *tamanioSerializacion)
{

    //CREO EL BUFFER
    t_buffer *buffer = malloc(sizeof(t_buffer));

    //CARGO EL SIZE DEL BUFFER
    buffer->size = sizeof(t_posicion);

    //CARGO EL TAMAÑO SE LA SERIALIZACION (PARA QUE EL SEND SE PUEDA REALIZAR CORRECTAMENTE)
    *tamanioSerializacion = buffer->size + sizeof(uint32_t) + sizeof(uint8_t);

    //CARGO EL STREAM DEL BUFFER
    void *stream = malloc(buffer->size);
    int offset = 0;

    memcpy(stream + offset, &posX, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, &posY, sizeof(uint32_t));

    buffer->stream = stream;

    //CREAMOS EL PAQUETE
    t_paquete *paquete = malloc(sizeof(t_paquete));
    paquete->codigo_operacion = ALERTA_SABOTAJE;
    paquete->buffer = buffer;

    //CREO EL STREAM A ENVIAR
    void *a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
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

void *serializar_pidYtid(uint32_t pid, uint32_t tid, uint8_t COD_OP, int *tamanioSerializacion)
{

    //CREO EL BUFFER
    t_buffer *buffer = malloc(sizeof(t_buffer));

    //CARGO EL SIZE DEL BUFFER
    buffer->size = sizeof(uint32_t) * 2;

    //CARGO EL TAMAÑO SE LA SERIALIZACION (PARA QUE EL SEND SE PUEDA REALIZAR CORRECTAMENTE)
    *tamanioSerializacion = buffer->size + sizeof(uint32_t) + sizeof(uint8_t);

    //CARGO EL STREAM DEL BUFFER
    void *stream = malloc(buffer->size);
    int offset = 0;

    memcpy(stream + offset, &pid, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, &tid, sizeof(uint32_t));

    buffer->stream = stream;

    //CREAMOS EL PAQUETE
    t_paquete *paquete = malloc(sizeof(t_paquete));
    paquete->codigo_operacion = COD_OP;
    paquete->buffer = buffer;

    //CREO EL STREAM A ENVIAR
    void *a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
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

void* serializar_tcbMostrar(uint32_t cantTotalTripulantes, t_TCBmostrar* tcbs_enviar, int *tamanioSerializacion){

    //CREO EL BUFFER Y LE RESERVO LA MEMORIA
    t_buffer *buffer = malloc(sizeof(t_buffer));

    //CARGO EL SIZE DEL BUFFER
    buffer->size = (sizeof(t_TCBmostrar) * cantTotalTripulantes) + sizeof(uint32_t);

    *tamanioSerializacion = buffer->size + sizeof(uint32_t) + sizeof(uint8_t);

    //CARGO EL STREAM DEL BUFFER
    buffer->stream = malloc(buffer->size);

    int offset = 0;

    memcpy(buffer->stream + offset, &cantTotalTripulantes, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    for (int i = 0; i < cantTotalTripulantes; i++)
    {
        memcpy(buffer->stream + offset, &tcbs_enviar[i].PID, sizeof(u_int32_t));
        offset += sizeof(u_int32_t);
        memcpy(buffer->stream + offset, &tcbs_enviar[i].TID, sizeof(u_int32_t));
        offset += sizeof(u_int32_t);
        memcpy(buffer->stream + offset, &tcbs_enviar[i].estado, sizeof(char));
        offset += sizeof(char);
        memcpy(buffer->stream + offset, &tcbs_enviar[i].posX, sizeof(u_int32_t));
        offset += sizeof(u_int32_t);
        memcpy(buffer->stream + offset, &tcbs_enviar[i].posY, sizeof(u_int32_t));
        offset += sizeof(u_int32_t);
        memcpy(buffer->stream + offset, &tcbs_enviar[i].proximaInstruccion, sizeof(u_int32_t));
        offset += sizeof(u_int32_t);
    }

    //CREAMOS EL PAQUETE
    t_paquete *paquete = malloc(sizeof(t_paquete));

    paquete->codigo_operacion = LISTA_TRIPULANTES;
    paquete->buffer = buffer;

    //CREO EL STREAM A ENVIAR
    void *a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
    offset = 0;

    memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

    //DEVUELVO LA MEMORIA UTILIZADA
    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);

    return a_enviar;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(a_enviar);
}

void* serializar_bitacora(t_bitacora *unaBitacora, int *tamanioSerializacion){

    //CREO EL BUFFER Y LE RESERVO LA MEMORIA
    t_buffer *buffer = malloc(sizeof(t_buffer));

    //CARGO EL SIZE DEL BUFFER
    buffer->size = 0;

    buffer->size += sizeof(uint32_t) * 2 + unaBitacora->tamanioAcciones;

    *tamanioSerializacion = buffer->size + sizeof(uint32_t) + sizeof(uint8_t);

    //CARGO EL STREAM DEL BUFFER
    buffer->stream = malloc(buffer->size);

    int offset = 0;

    memcpy(buffer->stream + offset, &(unaBitacora->cantAcciones), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(buffer->stream + offset, &(unaBitacora->tamanioAcciones), sizeof(uint32_t));
    offset += sizeof(uint32_t);

    for (int i = 0; i < unaBitacora->cantAcciones; i++)
    {
        memcpy(buffer->stream + offset, &(unaBitacora->acciones[i].tamanioAccion), sizeof(u_int32_t));
        offset += sizeof(u_int32_t);
        memcpy(buffer->stream + offset, &(unaBitacora->acciones[i].accion), sizeof(unaBitacora->acciones[i].tamanioAccion));
        offset += sizeof(unaBitacora->acciones[i].tamanioAccion);
    }

    //CREAMOS EL PAQUETE
    t_paquete *paquete = malloc(sizeof(t_paquete));

    paquete->codigo_operacion = BITACORA_TRIPULANTE;
    paquete->buffer = buffer;

    //CREO EL STREAM A ENVIAR
    void *a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
    offset = 0;

    memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

    //DEVUELVO LA MEMORIA UTILIZADA
    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);

    return a_enviar;

    //NO OLVIDARSE DE LIBERAR LA MEMORIA QUE DEVUELVE ESTA FUNCION
    //free(a_enviar);


}