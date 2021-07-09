#include "proceso3.h"

void solicitar_paginas(int cantidad, int pid)
{
    t_pagina_proceso *paginas_proceso = malloc(sizeof(t_pagina_proceso));
    paginas_proceso->paginas = list_create();
    paginas_proceso->pid = pid;

    for (int i = 0; i < maxima_cantidad_paginas && list_size(paginas_proceso) < cantidad; i++)
    {
        if (estado_frames[i] == 0)
            list_add(paginas_proceso->paginas, i * tamanio_pagina + memoria);
    }

    if (list_size(paginas_proceso->paginas) < cantidad)
    {

        // TODO: Una vez que este hecho el swapping y no se logre reservar la
        // cantidad de paginas -> Informar al discoridador
        log_info(logger, "No se pudo reserver memoria.");
    }

    listAdd(tabla_paginas, paginas_proceso);
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

void swap_por_Clock()
{
}

void swap_por_LRU()
{
}