#include "proceso3.h"

void generar_archivo_swap()
{
    int *swap_fd = open(config->path_swap, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    ftruncate(swap_fd, config->tamanio_swap);
    close(swap_fd);
}