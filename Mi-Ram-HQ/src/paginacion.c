#include "proceso3.h"

int solicitar_paginas(int bytes_solicitados, int pid)
{
    int cantidad_paginas_a_reservar;
    int err;
    int fragmentacionInterna = calcular_fragmentacion(pid);
    t_pagina_proceso* paginas_proceso;

    printf("Fragmentacion: %d\n", fragmentacionInterna);

    if(fragmentacionInterna > bytes_solicitados){
        printf("No debo reservar paginas\n");
        return 0;
    }

    cantidad_paginas_a_reservar = (bytes_solicitados - fragmentacionInterna) / tamanio_paginas;
    if (((bytes_solicitados - fragmentacionInterna) % tamanio_paginas) != 0)
        cantidad_paginas_a_reservar++;
    
    printf("bytes_solicitados %d\ncantidad_paginas_a_reservar: %d\n", bytes_solicitados, cantidad_paginas_a_reservar);

    paginas_proceso = obtener_paginas_proceso(pid, &err);
    if (err){    
        paginas_proceso = malloc(sizeof(t_pagina_proceso));
        paginas_proceso->paginas = list_create();
        paginas_proceso->pid = pid;
        printf("Tamaño lista cuando lo creo: %d\n", list_size(paginas_proceso->paginas));
    }

    for (int i = 0; i < maxima_cantidad_paginas && list_size(paginas_proceso->paginas) < cantidad_paginas_a_reservar; i++)
    {
        if (estado_frames[i] == 0)
        {
            estado_frames[i] = 1;
            printf("Pagina: %d\n", (int) (i * config->tamanio_pagina + memoria));
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

void guardar_tareas_pcb_paginacion(t_tareas_cPID *tareas_cPID_recibidas)
{
    //SERIALIZO LAS TAREAS
    void *tareas_pcb = malloc(sizeof(t_tarea) * tareas_cPID_recibidas->cantTareas + sizeof(t_PCB));
    int offset = 0;

    for (int i = 0; i < tareas_cPID_recibidas->cantTareas; i++)
    {
        memcpy(tareas_pcb + offset, serializar_TAREA(&(tareas_cPID_recibidas->tareas[i])), sizeof(t_tarea));
        offset += sizeof(t_tarea);
    }

    //CREO EL PCB
    t_PCB *pcb  = malloc(sizeof(t_PCB));
    pcb->PID    = tareas_cPID_recibidas->PID; 
    pcb->tareas = 0;

    //COPIO EL PCB A tareas_pcb
    memcpy(tareas_pcb + offset, pcb, sizeof(t_PCB));

    //LIBERO EL PCB
    free(pcb);

    //GUARDO LAS TAREAS SERIALIZADAS EN MEMORIA
    offset = 0;
    t_list *paginas_proceso = buscar_paginas_proceso(tareas_cPID_recibidas->PID);

    int ultPagina = (sizeof(t_tarea) * tareas_cPID_recibidas->cantTareas + sizeof(t_PCB)) % tamanio_paginas;

    for (int i = 0; i < list_size(paginas_proceso); i++)
    {
        if(i == list_size(paginas_proceso) - 1){
            memcpy((void *) list_get(paginas_proceso, i), tareas_pcb + offset, ultPagina);
        }
        else{
            memcpy((void *) list_get(paginas_proceso, i), tareas_pcb + offset, tamanio_paginas);
            offset += tamanio_paginas;
        }
    }

    //ACTUALIZO LA TABLA DEL PROCESO
    t_list *tabla_proceso = list_create();

    t_tabla_paginas_proceso *tabla_paginas_tareas = malloc(sizeof(t_tabla_paginas_proceso));
    tabla_paginas_tareas->id         = tareas_cPID_recibidas->cantTareas;
    tabla_paginas_tareas->tipo       = TAREAS;
    tabla_paginas_tareas->offset     = 0;
    tabla_paginas_tareas->tamanio    = tareas_cPID_recibidas->cantTareas * sizeof(t_tarea);
    tabla_paginas_tareas->modificado = 0;

    t_tabla_paginas_proceso *tabla_paginas_pcb = malloc(sizeof(t_tabla_paginas_proceso));
    tabla_paginas_pcb->id         = tareas_cPID_recibidas->PID;
    tabla_paginas_pcb->tipo       = PCB;
    tabla_paginas_pcb->offset     = tareas_cPID_recibidas->cantTareas * sizeof(t_tarea);;
    tabla_paginas_pcb->tamanio    = sizeof(t_PCB);
    tabla_paginas_pcb->modificado = 0;

    list_add(tabla_proceso, tabla_paginas_tareas);
    list_add(tabla_proceso, tabla_paginas_pcb);

    list_add(tabla_procesos, tabla_proceso);

    printf("La tabla del proceso %d tiene %d cosas\n", tareas_cPID_recibidas->PID, list_size(tabla_proceso));

    return;
}

void guardar_tcb_paginacion(t_TCBcPID *datos_recibidos)
{

    //SERIALIZO EL TCB
    void *tcb = serializar_TCB(datos_recibidos->tcb);

    //GUARDO EL TCB EN LA MEMORIA
    int bytesOcupados            = bytes_ocupados_pid(datos_recibidos->pid);
    int cantPaginasProceso       = cantidad_paginas_proceso(datos_recibidos->pid);
    int paginas_enteras_ocupadas = 0;
    int fragmentacionInterna     = calcular_fragmentacion(datos_recibidos->pid);
    int paginas_para_tcb         = 

    for (int i = 0; i < bytesOcupados; i+=tamanio_paginas)
        paginas_enteras_ocupadas++;

    int err;
    t_list* lista_paginas_proceso = obtener_lista_proceso(datos_recibidos->pid, &err);


    //EN EL CASO DE QUE EL TCB SE EMPIECE A GUARDAR DESDE EL PRINCIPIO DE UNA PAGINA
    if (bytesOcupados % tamanio_paginas == 0)
    {
        int bGuardados = 0;
        for (int i = paginas_enteras_ocupadas; i < list_size(lista_paginas_proceso); i++)
        {
            //
            if ((bGuardados - tamanio_paginas) >= tamanio_paginas || bGuardados == 0)
            {
                memcpy((void *) list_get(lista_paginas_proceso, i), tcb + bGuardados, tamanio_paginas)
                bGuardados += tamanio_paginas;
            }
            else{
                memcpy((void *) list_get(lista_paginas_proceso, i), tcb + bGuardados, )
                bGuardados += tamanio_paginas;
            }          
        }
    }
    else{

    }
        

    //ACTUALIZO LA TABLA DEL PROCESO
    t_tabla_paginas_proceso *tabla_paginas_tcb = malloc(sizeof(t_tabla_paginas_proceso));
    tabla_paginas_tareas->id         = datos_recibidos->tcb.TID;
    tabla_paginas_tareas->tipo       = TCB;
    tabla_paginas_tareas->offset     = bytesOcupados;
    tabla_paginas_tareas->tamanio    = sizeof(t_TCB);
    tabla_paginas_tareas->modificado = 0;


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