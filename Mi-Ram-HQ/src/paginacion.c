#include "proceso3.h"

int solicitar_paginas(int bytes_solicitados, int pid)
{
    int cantidad_paginas_a_reservar;
    int err;
    int fragmentacion_interna = calcular_fragmentacion(pid);
    int paginas_reservadas = 0;
    t_pagina_proceso *paginas_proceso;

    if (fragmentacion_interna > bytes_solicitados)
    {
        return 0;
    }

    cantidad_paginas_a_reservar = (bytes_solicitados - fragmentacion_interna) / tamanio_paginas;
    if (((bytes_solicitados - fragmentacion_interna) % tamanio_paginas) != 0)
        cantidad_paginas_a_reservar++;

    //log_info(logger, "Se solicitaron %dB, la fragmentacion es %dB", bytes_solicitados, fragmentacion_interna);

    paginas_proceso = obtener_paginas_proceso(pid, &err);
    if (err)
    {
        paginas_proceso = malloc(sizeof(t_pagina_proceso));
        paginas_proceso->paginas = list_create();
        paginas_proceso->pid = pid;
    }

    //printf("i(null) < max_cant_pags(%d) && pags_reser(%d) < cant_pags_reser(%d)\n", maxima_cantidad_paginas, paginas_reservadas, cantidad_paginas_a_reservar);
    for (int i = 0; i < maxima_cantidad_paginas && paginas_reservadas < cantidad_paginas_a_reservar; i++)
    {
        //printf("i(%d) < max_cant_pags(%d) && pags_reser(%d) < cant_pags_reser(%d)\n", i, maxima_cantidad_paginas, paginas_reservadas, cantidad_paginas_a_reservar);
        t_estado_frame *frame = list_get(estado_frames, i);
        if (frame->ocupado == 0)
        {
            frame->pag_proc = cantidad_paginas_proceso(pid) + paginas_reservadas + 1;
            frame->ocupado = 1;
            frame->pid = pid;
            list_replace(estado_frames, i, frame);
            list_add(paginas_proceso->paginas, (void *)(i * config->tamanio_pagina + (int)memoria));
            paginas_reservadas++;
            //log_info(logger, "Reservo una pagina");
        }
    }

    if (list_size(paginas_proceso->paginas) < cantidad_paginas_a_reservar)
    {
        frame_a_memoria_virtual();
        // TODO: Una vez que este hecho el swapping y no se logre reservar la
        // cantidad de paginas -> Informar al discoridador
        log_error(logger, "No se pudo reserver memoria.");
        exit(EXIT_FAILURE);
        return EXIT_FAILURE;
    }

    //TODO: no solicitar paginas si la ultima del proceso no fue llenada

    list_add(tabla_paginas, paginas_proceso);
    return 0;
}

void guardar_tareas_pcb_paginacion(t_tareas_cPID *tareas_cPID_recibidas)
{

    int tamanio_tareas = 0;
    for (int i = 0; i < tareas_cPID_recibidas->cantTareas; i++)
        tamanio_tareas += tareas_cPID_recibidas->tareas[i].tamanio_tarea;

    //SERIALIZO LAS TAREAS
    void *tareas_pcb = malloc(tamanio_tareas + sizeof(t_PCB));
    int offset = 0;

    for (int i = 0; i < tareas_cPID_recibidas->cantTareas; i++)
    {
        memcpy(tareas_pcb + offset, serializar_TAREA(tareas_cPID_recibidas->tareas[i].tarea, tareas_cPID_recibidas->tareas[i].tamanio_tarea), tareas_cPID_recibidas->tareas[i].tamanio_tarea);
        offset += tareas_cPID_recibidas->tareas[i].tamanio_tarea;
    }

    //CREO EL PCB
    t_PCB *pcb = malloc(sizeof(t_PCB));
    pcb->PID = tareas_cPID_recibidas->PID;
    pcb->tareas = tamanio_tareas;

    //COPIO EL PCB A tareas_pcb
    memcpy(tareas_pcb + offset, pcb, sizeof(t_PCB));

    //LIBERO EL PCB
    free(pcb);

    //GUARDO LAS TAREAS SERIALIZADAS EN MEMORIA
    offset = 0;
    int err;
    t_list *paginas_proceso = obtener_paginas_proceso(tareas_cPID_recibidas->PID, &err)->paginas;

    int ultPagina = (tamanio_tareas + sizeof(t_PCB)) % tamanio_paginas;

    for (int i = 0; i < list_size(paginas_proceso); i++)
    {
        if (i == list_size(paginas_proceso) - 1)
        {
            //printf("Guardo en última pagina (la %d) las tareas y pcb %d bytes\n", i, ultPagina);
            memcpy((void *)list_get(paginas_proceso, i), tareas_pcb + offset, ultPagina);
        }
        else
        {
            //printf("Guardo pagina %d\n", i);
            memcpy((void *)list_get(paginas_proceso, i), tareas_pcb + offset, tamanio_paginas);
            offset += tamanio_paginas;
        }
    }

    //ACTUALIZO LA TABLA DEL PROCESO
    t_list *tabla_proceso = list_create();

    t_tabla_paginas_proceso *tabla_paginas_tareas = malloc(sizeof(t_tabla_paginas_proceso));
    tabla_paginas_tareas->id = tareas_cPID_recibidas->cantTareas;
    tabla_paginas_tareas->tipo = TAREAS;
    tabla_paginas_tareas->offset = 0;
    tabla_paginas_tareas->tamanio = tareas_cPID_recibidas->cantTareas * sizeof(t_tarea);
    tabla_paginas_tareas->modificado = 0;
    tabla_paginas_tareas->len_tareas = list_create();

    for (int i = 0; i < tareas_cPID_recibidas->cantTareas; i++)
    {
        list_add(tabla_paginas_tareas->len_tareas, (void *)tareas_cPID_recibidas->tareas[i].tamanio_tarea);
    }

    t_tabla_paginas_proceso *tabla_paginas_pcb = malloc(sizeof(t_tabla_paginas_proceso));
    tabla_paginas_pcb->id = tareas_cPID_recibidas->PID;
    tabla_paginas_pcb->tipo = PCB;
    tabla_paginas_pcb->offset = tareas_cPID_recibidas->cantTareas * sizeof(t_tarea);
    tabla_paginas_pcb->tamanio = sizeof(t_PCB);
    tabla_paginas_pcb->modificado = 0;
    //gettimeofday(&tv, NULL);
    //tabla_paginas_tcb->ultimo_acceso = tv.tv_usec;

    list_add(tabla_proceso, tabla_paginas_tareas);
    list_add(tabla_proceso, tabla_paginas_pcb);

    list_add(tabla_procesos, tabla_proceso);

    return;
}

void guardar_tcb_paginacion(t_TCBcPID *datos_recibidos)
{
    //SERIALIZO EL TCB
    void *tcb = serializar_TCB(datos_recibidos->tcb);

    //GUARDO EL TCB EN LA MEMORIA
    int bytesOcupados = bytes_ocupados_pid(datos_recibidos->pid);
    int fragmentacion_interna = calcular_fragmentacion(datos_recibidos->pid);
    int escribirDesdePagina;
    int offsetPrimeraPagina = fragmentacion_interna == 0 ? 0 : tamanio_paginas - fragmentacion_interna;
    int paginas_acceder = 0;
    int bGuardados = 0;

    //printf("\n\nBYTES OCUPADOS: %d\n", bytesOcupados);
    //printf("FRAGMENTACION INTERNA: %d\n", fragmentacion_interna);
    //printf("OFFSET PRIMERA PAGINA: %d\n", offsetPrimeraPagina);

    for (int i = fragmentacion_interna; i < sizeof(t_TCB); i += tamanio_paginas)
        paginas_acceder++;

    // printf("PAGINAS A ACCEDER: %d\n", paginas_acceder);

    int err;
    t_pagina_proceso *lista_paginas_proceso = obtener_paginas_proceso(datos_recibidos->pid, &err);

    //escribirDesdePagina = (((int) list_get(lista_paginas_proceso->paginas, bytesOcupados / tamanio_paginas) - (int) memoria) / tamanio_paginas);
    escribirDesdePagina = bytesOcupados / tamanio_paginas;
    //printf("Escribir desde pagina %d el byte %d\nbytesOcupados / tamanio_paginas: %d\n", escribirDesdePagina, offsetPrimeraPagina, bytesOcupados / tamanio_paginas);

    //log_info(logger, "Tamanio lista: %d\nPaginas acceder: %d\n", list_size(lista_paginas_proceso->paginas), paginas_acceder);

    //EN EL CASO DE QUE ENTRE EN LA ULTIMA PAGINA
    if (fragmentacion_interna >= sizeof(t_TCB))
        memcpy((list_get(lista_paginas_proceso->paginas, escribirDesdePagina) + offsetPrimeraPagina), tcb, sizeof(t_TCB));
    //EN EL CASO DE QUE DEBA ALMACENARSE EN UNA NUEVA PAGINA O FINALIZAR LA ULTIMA Y UTILIZAR LA SIGUIENTE
    else
    {
        //ESCRIBO DESDE LA PAGINA QUE DEBA LLENAR HASTA TODAS LAS QUE NECESITE PARA GUARDAR EL TCB
        for (int i = escribirDesdePagina; i <= (escribirDesdePagina + paginas_acceder); i++)
        {
            //log_info(logger, "i: %d", i);
            if (bGuardados == sizeof(t_TCB))
                continue;

            //LA PRIMERA PAGINA DEBO UNICAMENTE LLENARLA CON LOS BYTES QUE FALTAN
            if (i == escribirDesdePagina)
            {
                int bytes_a_copiar = fragmentacion_interna == 0 ? tamanio_paginas : fragmentacion_interna;
                // printf("Ahora guardo %d bytes\n", bGuardados + bytes_a_copiar);
                // printf("Bytes a copiar %d\n", bytes_a_copiar);
                memcpy((list_get(lista_paginas_proceso->paginas, i) + offsetPrimeraPagina), tcb, bytes_a_copiar);
                // memcpy((list_get(lista_paginas_proceso->paginas, i) + offsetPrimeraPagina), tcb, fragmentacion_interna);
                // bGuardados += fragmentacion_interna;
                bGuardados += bytes_a_copiar;
            }
            //EL RESTO DE LAS PAGINAS DEBO LLENARLAS ENTERAS O CON EL RESTANTE DEL TCB
            else
            {
                if (bGuardados + tamanio_paginas < sizeof(t_TCB))
                {
                    //printf("Guardé %d bytes\n", bGuardados + tamanio_paginas);
                    //printf("Se copia en %p lo que esta en %p, un total de %dB\n", list_get(lista_paginas_proceso->paginas, i), tcb + bGuardados, tamanio_paginas);
                    memcpy(list_get(lista_paginas_proceso->paginas, i), tcb + bGuardados, tamanio_paginas);
                    bGuardados += tamanio_paginas;
                }
                else
                {
                    //printf("Se copia en %p lo que esta en %p, un total de %dB\n", list_get(lista_paginas_proceso->paginas, i), tcb + bGuardados, sizeof(t_TCB)-bGuardados);
                    // printf("bGuardados: %d\tEscribo: %d\n", bGuardados, sizeof(t_TCB) - bGuardados);
                    memcpy(list_get(lista_paginas_proceso->paginas, i), tcb + bGuardados, sizeof(t_TCB) - bGuardados);
                    bGuardados += sizeof(t_TCB) - bGuardados;
                }
            }

            // printf("Copié %d bytes\n", bGuardados);
        }
    }

    //ACTUALIZO LA TABLA DEL PROCESO
    t_tabla_paginas_proceso *tabla_paginas_tcb = malloc(sizeof(t_tabla_paginas_proceso));

    tabla_paginas_tcb->id = datos_recibidos->tcb.TID;
    tabla_paginas_tcb->tipo = TCB;
    tabla_paginas_tcb->offset = bytesOcupados;
    tabla_paginas_tcb->tamanio = sizeof(t_TCB);
    tabla_paginas_tcb->modificado = 0;

    t_list *tabla_proceso = obtener_lista_proceso(datos_recibidos->pid, &err);
    list_add(tabla_proceso, tabla_paginas_tcb);
}