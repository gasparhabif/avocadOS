#include "proceso3.h"
#include <math.h>

void comenzar_patota_paginada(int client, t_tareas_cPID *tareas_cPID_recibidas)
{
    int tamanio_tareas = sizeof(t_tareas) * t_tareas_cPID_recibidas->cantTareas;
    int paginas_necesarias = ceil(tamanio_tareas / tamanio_paginas);
    solicitar_paginas(paginas_necesarias);
}

void iniciar_tripulante_paginada(int client, t_TCBcPID *datos_recibidos)
{
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
