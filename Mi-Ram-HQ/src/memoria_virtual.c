#include "proceso3.h"

void frame_a_memoria_virtual()
{
    if (!strcmp(config->algoritmo_reemplazo, "LRU"))
    {
    }
    else // CLOCK
    {
    }
}

/*
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
*/