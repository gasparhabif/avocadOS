#include "proceso3.h"

t_list *buscar_paginas_proceso(int pid)
{
    t_list *paginas = list_create();
    for (int i = 0; i < list_size(tabla_paginas); i++)
    {
        t_pagina_proceso paginaActual = list_get(tabla_paginas, i);
        if (paginaActual->pid == pid)
        {
            list_add(paginas, paginaActual);
        }
    }
    return paginas;
}
