#include "proceso3.h"

void generar_archivo_swap()
{
    int *swap_fd = open(config->path_swap, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    ftruncate(swap_fd, config->tamanio_swap);
    close(swap_fd);
}

void realizar_swap()
{
    if (strcmp(config->algoritmo, "LRU"))
    {
        swap_por_LRU();
    }
    else if (strcmp(config->algoritmo, "CLOCK"))
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