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

    if (list_size(tabla_procesos) != 0)
    {

        for (int i = 0; i < list_size(tabla_procesos); i++)
        {
            tabla_un_proceso = list_get(tabla_procesos, i);

            for (int j = 0; j < list_size(tabla_un_proceso); j++)
            {
                proceso = list_get(tabla_un_proceso, j);

                if (proceso->tipo == PCB && proceso->id == pid)
                {
                    *err = 0;
                    return tabla_un_proceso;
                }
            }
        }
    }

    *err = 1;
    return (t_list *)EXIT_FAILURE;
}

t_pagina_proceso *obtener_paginas_proceso(int pid, int *err)
{
    t_pagina_proceso *paginas_proceso;

    for (int i = 0; i < list_size(tabla_paginas); i++)
    {
        paginas_proceso = list_get(tabla_paginas, i);

        if (paginas_proceso->pid == pid)
        {
            *err = 0;
            return paginas_proceso;
        }
    }

    *err = 1;
    return (t_pagina_proceso *)-1;
}

int calcular_fragmentacion(int pid)
{
    int err;
    t_list *tabla_proceso = obtener_lista_proceso(pid, &err);

    if (err)
        return 0;
    else
    {
        int bytes_ocupados = bytes_ocupados_lista(tabla_proceso);
        int paginas_ocupadas = calcular_paginas_ocupadas(bytes_ocupados);

        // return tamanio_paginas - (bytes_ocupados - tamanio_paginas * (bytes_ocupados / tamanio_paginas));
        return tamanio_paginas * paginas_ocupadas - bytes_ocupados;
    }
}

int calcular_paginas_ocupadas(int bytes_ocupados)
{
    int cantidad = 1;
    while (bytes_ocupados > tamanio_paginas)
    {
        cantidad++;
        bytes_ocupados -= tamanio_paginas;
    }
    return cantidad;
}

int bytes_ocupados_pid(int pid)
{

    int err;
    t_list *tabla_proceso = obtener_lista_proceso(pid, &err);

    if (err)
        return 0;
    else
        return bytes_ocupados_lista(tabla_proceso);
}

int bytes_ocupados_lista(t_list *lista_elementos_proceso)
{

    t_tabla_paginas_proceso *elemento_proceso;
    int bytes = 0;

    for (int i = 0; i < list_size(lista_elementos_proceso); i++)
    {
        elemento_proceso = list_get(lista_elementos_proceso, i);
        bytes += elemento_proceso->tamanio;
    }
    return bytes;
}

int cantidad_paginas_proceso(int pid)
{

    t_pagina_proceso *pagina_proceso;

    for (int i = 0; i < list_size(tabla_paginas); i++)
    {
        pagina_proceso = list_get(tabla_paginas, i);

        if (pagina_proceso->pid == pid)
            return list_size(pagina_proceso->paginas);
    }

    return -1;
}

int obtener_numero_instruccion(t_list *tabla_proceso, int pid, int tid)
{

    t_tabla_paginas_proceso *elemento_proceso;
    void *d_proceso;
    void *tcb_serializado = malloc(sizeof(t_TCB));
    t_TCB *tcb;
    int numInst = 0;

    for (int i = 0; i < list_size(tabla_proceso); i++)
    {
        elemento_proceso = list_get(tabla_proceso, i);

        if (elemento_proceso->tipo == TCB && elemento_proceso->id == tid)
        {
            //OBTENGO EL TCB
            d_proceso = recuperar_elementos_proceso(pid);
            memcpy(tcb_serializado, d_proceso + elemento_proceso->offset, elemento_proceso->tamanio);
            tcb = deserializar_TCB(tcb_serializado);

            printf("Tengo el tcb\n");

            //INCREMENTO EL IP
            numInst = tcb->proximaInstruccion;
            tcb->proximaInstruccion++;

            printf("Incremento el tcb\n");

            //GUARDO EL TCB ACTUALIZADO
            free(tcb_serializado);
            tcb_serializado = serializar_pTCB(tcb);
            memcpy(d_proceso + elemento_proceso->offset, tcb_serializado, elemento_proceso->tamanio);
            guardar_elementos_proceso(pid, d_proceso);
            free(tcb);
            free(d_proceso);

            printf("Guardo el tcb\n");

            //printf("TID: %d\n", tcb->TID);
            //printf("estado: %c\n", tcb->estado);
            //printf("posX: %d\n", tcb->posX);
            //printf("posY: %d\n", tcb->posY);
            //printf("proximaInstruccion: %d\n", tcb->proximaInstruccion);
            //printf("puntero_PCB: %d\n", tcb->puntero_PCB);

            //printf("Devuelvo %d\n", numInst);

            printf("Termino\n");
            return numInst;
        }
    }

    return -1;
}

void *recuperar_elementos_proceso(int pid)
{

    int err;
    int bProceso = bytes_ocupados_pid(pid);
    int bObtenidos = 0;
    void *elementos_proceso = malloc(bProceso);
    t_pagina_proceso *paginas_proceso = obtener_paginas_proceso(pid, &err);

    for (int i = 0; i < list_size(paginas_proceso->paginas); i++)
    {
        if ((bObtenidos + tamanio_paginas) < bProceso)
        {
            memcpy(elementos_proceso + bObtenidos, list_get(paginas_proceso->paginas, i), tamanio_paginas);
            bObtenidos += tamanio_paginas;
        }
        else
        {
            memcpy(elementos_proceso + bObtenidos, list_get(paginas_proceso->paginas, i), (tamanio_paginas - calcular_fragmentacion(pid)));
            bObtenidos += (tamanio_paginas - calcular_fragmentacion(pid));
        }
    }

    return elementos_proceso;
}

void guardar_elementos_proceso(int pid, void *datosProceso)
{
    int bProceso = bytes_ocupados_pid(pid);
    int bGuardados = 0;
    int err;
    t_list *pag_proc = obtener_paginas_proceso(pid, &err)->paginas;

    //printf("bProceso: %d\n", bProceso);

    for (int i = 0; i < list_size(pag_proc); i++)
    {
        if ((bGuardados + tamanio_paginas) < bProceso)
        {
            //printf("Guardo bien\n");
            memcpy(list_get(pag_proc, i), datosProceso + bGuardados, tamanio_paginas);
            bGuardados += tamanio_paginas;
        }
        else
        {
            //printf("Guardo en %p, lo que esta en %p, cantidad %d\n", list_get(pag_proc, i), datosProceso + bGuardados, (tamanio_paginas-calcular_fragmentacion(pid)));
            memcpy(list_get(pag_proc, i), datosProceso + bGuardados, (tamanio_paginas - calcular_fragmentacion(pid)));
            bGuardados += (tamanio_paginas - calcular_fragmentacion(pid));
        }
    }
}

t_tarea *obtenerTarea(t_list *lista_proceso, int pid, int nInstruccion)
{

    void *elementos_proceso;
    elementos_proceso = recuperar_elementos_proceso(pid);
    t_tabla_paginas_proceso *pagina_proceso;
    void *tarea_serializada;
    tarea_serializada = malloc(sizeof(t_tarea));
    t_tarea *tarea;

    for (int i = 0; i < list_size(lista_proceso); i++)
    {
        pagina_proceso = list_get(lista_proceso, i);

        if (pagina_proceso->tipo == TAREAS)
        {

            //printf("Tarea N°%d\n", nInstruccion);

            if (pagina_proceso->tamanio / sizeof(t_tarea) == nInstruccion)
            {
                printf("ULTIMA INSTRUCCION\n");

                tarea = malloc(sizeof(t_tarea));
                tarea->codigoTarea = FIN_TAREAS;
                tarea->parametro = 0;
                tarea->posX = 0;
                tarea->posY = 0;
                tarea->duracionTarea = 0;

                return tarea;
            }
            else
            {
                memcpy(tarea_serializada, elementos_proceso + pagina_proceso->offset + nInstruccion * sizeof(t_tarea), sizeof(t_tarea));
                tarea = deserializar_TAREA(tarea_serializada);

                //printf("codigoTarea: %d\n", tarea->codigoTarea);
                //printf("parametro: %d\n", tarea->parametro);
                //printf("posX: %d\n", tarea->posX);
                //printf("posY: %d\n", tarea->posY);
                //printf("duracionTarea: %d\n", tarea->duracionTarea);

                return tarea;
            }
        }
    }

    return (t_tarea *)-1;
}

int cant_tripulantes_paginacion(t_list *tabla_proceso)
{
    int cant = 0;
    t_tabla_paginas_proceso *pagina_proceso;

    for (int i = 0; i < list_size(tabla_proceso); i++)
    {
        pagina_proceso = list_get(tabla_proceso, i);

        if (pagina_proceso->tipo == TCB)
            cant++;
    }

    return cant;
}

int cant_tripulantes_proceso(int pid)
{
    int err;
    t_list *lista_proc = obtener_lista_proceso(pid, &err);

    return cant_tripulantes_paginacion(lista_proc);
}

int obtener_pid_pag(t_list *tablaUnProceso)
{

    t_tabla_paginas_proceso *pagina_proceso;

    for (int i = 0; i < list_size(tablaUnProceso); i++)
    {
        pagina_proceso = list_get(tablaUnProceso, i);

        if (pagina_proceso->tipo == PCB)
            return pagina_proceso->id;
    }

    return -1;
}

void limpiar_estado_frames()
{
    for (int i = 0; i < maxima_cantidad_paginas; i++)
    {
        estado_frames[i] = 0;
    }
}