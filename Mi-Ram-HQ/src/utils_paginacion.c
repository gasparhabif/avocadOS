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

t_list *obtener_lista_proceso(int pid, int *err)
{
    t_list *tabla_un_proceso;
    t_tabla_paginas_proceso *proceso = malloc(sizeof(t_tabla_paginas_proceso));

    printf("Tamaño tabla proceso: %d\n", list_size(tabla_procesos));
    
    if(list_size(tabla_procesos) != 0){
        
        for (int i = 0; i < list_size(tabla_procesos); i++)
        {
            tabla_un_proceso = list_get(tabla_procesos, i);

            for (int j = 0; j < list_size(tabla_un_proceso); j++)
            {
                proceso = list_get(tabla_un_proceso, j);

                printf("TIPO: %d\tID: %d\n", proceso->tipo, proceso->id);

                if (proceso->tipo == PCB && proceso->id == pid)
                {
                    *err = 0;
                    return tabla_un_proceso;
                }
            }
        }
    }

    *err = 1;
    return (t_list *) -1;
}

t_pagina_proceso* obtener_paginas_proceso(int pid, int *err)
{
    t_pagina_proceso *paginas_proceso = malloc(sizeof(t_pagina_proceso));

    if(list_size(tabla_paginas) > 1){

        for (int i = 0; i < list_size(tabla_paginas); i++)
        {
            paginas_proceso = list_get(tabla_procesos, i);

            if (paginas_proceso->pid)
            {
                *err = 0;
                return paginas_proceso;
            }
        }
    }

    *err = 1;
    return (t_pagina_proceso*) -1;
}

int calcular_fragmentacion(int pid)
{
    int err;
    int bytes_ocupados = 0;
    t_list *tabla_proceso = obtener_lista_proceso(pid, &err);
    printf("Err: %d\n", err);

    if(err)
        return 0;
    else{
        
        bytes_ocupados_lista(tabla_proceso);

        printf("Bytes ocupados %d\nTamanio pagina: %d\n", bytes_ocupados, tamanio_paginas);

        return tamanio_paginas-(bytes_ocupados-tamanio_paginas*(bytes_ocupados/tamanio_paginas));
    }
}

int bytes_ocupados_pid(int pid){

    int err;
    t_list *tabla_proceso = obtener_lista_proceso(pid, &err);

    if(err)
        return 0;
    else
        return bytes_ocupados_lista(tabla_procesos);

}

int bytes_ocupados_lista(t_list *lista_elementos_proceso){

    t_tabla_paginas_proceso *elemento_proceso;
    int bytes;

    for (int i = 0; i < list_size(lista_elementos_proceso); i++)
    {
        elemento_proceso = list_get(lista_elementos_proceso, i);
        bytes += elemento_proceso->tamanio;
    }

    return bytes;    

}

int cantidad_paginas_proceso(int pid){

    t_pagina_proceso *pagina_proceso;

    for (int i = 0; i < list_size(tabla_paginas); i++)
    {
        pagina_proceso = list_get(tabla_paginas, i);

        if (pagina_proceso->pid == pid)
            return list_size(pagina_proceso->paginas);
    }

    return -1;
    
}

void limpiar_estado_frames()
{
    for (int i = 0; i < (config->tamanio_memoria / tamanio_paginas); i++)
    {
        estado_frames[i] = 0;
    }
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