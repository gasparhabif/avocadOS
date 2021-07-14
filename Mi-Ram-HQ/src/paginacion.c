#include "proceso3.h"

int solicitar_paginas(int bytes_solicitados, int pid)
{
    int cantidad_paginas_a_reservar;
    int fragmentacionInterna = calcular_fragmentacion(pid);

    cantidad_paginas_a_reservar = (int)ceil((bytes_solicitados - fragmentacionInterna) / tamanio_paginas);

    t_pagina_proceso *paginas_proceso = malloc(sizeof(t_pagina_proceso));
    paginas_proceso->paginas = list_create();
    paginas_proceso->pid = pid;

    for (int i = 0; i < maxima_cantidad_paginas && list_size(paginas_proceso->paginas) < cantidad_paginas_a_reservar; i++)
    {
        if (estado_frames[i] == 0)
        {
            estado_frames[i] = 1;
            printf("Pagina: %d\n", i * config->tamanio_pagina + memoria);
            list_add(paginas_proceso->paginas, i * config->tamanio_pagina + memoria);
        }
    }

    if (list_size(paginas_proceso->paginas) < cantidad_paginas_a_reservar)
    {

        // TODO: Una vez que este hecho el swapping y no se logre reservar la
        // cantidad de paginas -> Informar al discoridador
        log_info(logger, "No se pudo reserver memoria.");
        return -1;
    }

    //TODO: no solicitar paginas si la ultima del proceso no fue llenada

    list_add(tabla_paginas, paginas_proceso);
    return 0;
}

void guardar_tareas_paginacion(t_tareas_cPID *tareas_cPID_recibidas)
{
    void *tareas = malloc(sizeof(t_tarea) * tareas_cPID_recibidas->cantTareas);
    int offset = 0;

    for (int i = 0; i < tareas_cPID_recibidas->cantTareas; i++)
    {
        memcpy(tareas + offset, serializar_TAREA(&(tareas_cPID_recibidas->tareas[i])), sizeof(t_tarea));
        offset += sizeof(t_tarea);
    }

    offset = 0;
    t_list *paginas_proceso = buscar_paginas_proceso(tareas_cPID_recibidas->PID);

    for (int i = 0; i < list_size(paginas_proceso); i++)
    {
        memcpy((void *)list_get(paginas_proceso, i), tareas + offset, tamanio_paginas);
        //offset += (tareas->cantTareas * sizeof(t_tarea) / tamanio_paginas;
        offset += tamanio_paginas;
    }

    t_tabla_paginas_proceso *tabla_paginas_proceso = malloc(sizeof(tabla_paginas_proceso));

    tabla_paginas_proceso->tipo = TAREAS;
    tabla_paginas_proceso->offset = 0;
    tabla_paginas_proceso->tamanio = tareas_cPID_recibidas->cantTareas * sizeof(t_tarea);
    tabla_paginas_proceso->modificado = 0;

    list_add(tabla_procesos, tabla_paginas_proceso);

    return;
}

void guardar_tcb_paginacion(t_TCBcPID *datos_recibidos)
{
}

void generar_archivo_swap()
{
    int *swap_fd = open(config->path_swap, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    ftruncate(swap_fd, config->tamanio_swap);
    close(swap_fd);
}

void realizar_swap()
{
    if (strcmp(config->algoritmo_reemplazo, "LRU"))
    {
        swap_por_LRU();
    }
    else // clock
    {
        swap_por_Clock();
    }
}

void dump_paginacion()
{
}

void swap_por_Clock()
{
}

void swap_por_LRU()
{
}