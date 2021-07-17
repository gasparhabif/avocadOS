#include "proceso3.h"

void comenzar_patota_paginada(int client, t_tareas_cPID *tareas_cPID_recibidas)
{
    int   tamanio_tareas = sizeof(t_tarea) * tareas_cPID_recibidas->cantTareas;
    void *paquete;
    int   tamanioSerializacion;

    if (solicitar_paginas(tamanio_tareas + sizeof(t_PCB), tareas_cPID_recibidas->PID) == -1)
        paquete = serializarInt(-1, PUNTERO_PCB, &tamanioSerializacion);
    else
    {
        guardar_tareas_pcb_paginacion(tareas_cPID_recibidas);

        //guardar_pcb_paginacion(tareas_cPID_recibidas->PID, /*DIRECCION TAREAS*/);

        paquete = serializarInt(1, PUNTERO_PCB, &tamanioSerializacion);
    }

    send(client, paquete, tamanioSerializacion, 0);
    free(paquete);
}

void iniciar_tripulante_paginado(int client, t_TCBcPID *datos_recibidos)
{
    void *paquete;
    int tamanioSerializacion;

    if (solicitar_paginas(sizeof(t_TCB), datos_recibidos->pid) == -1)
        paquete = serializarInt(-1, PUNTERO_PCB, &tamanioSerializacion);
    else
    {
        guardar_tcb_paginacion(datos_recibidos);
        paquete = serializarInt(1, PUNTERO_PCB, &tamanioSerializacion);
    }

    send(client, paquete, tamanioSerializacion, 0);
    free(paquete);
}

void solicitar_tarea_paginada(int client, t_pidYtid *datos_recibidos)
{
}

void mover_tripulante_paginada(t_envio_posicion *datos_recibidos)
{
}

void actualizar_estado_paginada(t_estado *datos_recibidos)
{
}

void solicitar_tripulantes_paginada(int client)
{
}

void eliminar_tripulante_paginado(t_pidYtid *datos_recibidos)
{
}
