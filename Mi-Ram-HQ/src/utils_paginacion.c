#include "proceso3.h"

t_list *buscar_paginas_proceso(int pid)
{
    t_pagina_proceso *tabla_un_proceso;

    for (int i = 0; i < list_size(tabla_procesos); i++)
    {
        tabla_un_proceso = list_get(tabla_procesos, i);
        if (tabla_un_proceso->pid == pid)
        {
            return tabla_un_proceso->paginas;
        }
    }
    return list_create();
}

t_list *obtener_lista_proceso(int pid)
{
    t_list *tabla_un_proceso;
    t_tabla_paginas_proceso *proceso;

    for (int i = 0; i < list_size(tabla_procesos); i++)
    {
        tabla_un_proceso = list_get(tabla_procesos, i);

        for (int j = 0; j < list_size(tabla_un_proceso); j++)
        {
            proceso = list_get(tabla_un_proceso, j);

            if (proceso->tipo == PCB && proceso->id == pid)
            {
                return tabla_un_proceso;
            }
        }
    }
    return list_create();
}

int calcular_fragmentacion(int pid)
{

    t_tabla_paginas_proceso *elemento_proceso;
    t_list *tabla_proceso = obtener_lista_proceso(pid);
    int bytes_ocupados = 0;

    for (int i = 0; i < list_size(tabla_proceso); i++)
    {
        elemento_proceso = list_get(tabla_proceso, i);
        bytes_ocupados += elemento_proceso->offset;
    }

    return bytes_ocupados % tamanio_paginas;
}

// t_pagina_proceso obtener_ultima_pagina_del_proceso(int pid)
// {
//     t_pagina_proceso ultima_pagina = null;
//     for (int i = 0; i < list_size(tabla_paginas->paginas); i++)
//     {
//         t_pagina_proceso pagina_actual = list_get(tabla_paginas->paginas, i);
//         if (pagina_actual->pid == pid)
//             ultima_pagina = pagina_actual;
//     }
// }