#include "proceso3.h"

int solicitar_paginas(int bytes_solicitados, int pid)
{
    int cantidad_paginas_a_reservar;
    int err;
    int fragmentacionInterna = calcular_fragmentacion(pid);
    int paginas_reservadas = 0;
    t_pagina_proceso* paginas_proceso;

    printf("Me solicitaron %dB, la fragmentacion es %d\n", bytes_solicitados, fragmentacionInterna);

    if(fragmentacionInterna > bytes_solicitados){
        printf("No debo reservar paginas\n");
        return 0;
    }

    cantidad_paginas_a_reservar = (bytes_solicitados - fragmentacionInterna) / tamanio_paginas;
    if (((bytes_solicitados - fragmentacionInterna) % tamanio_paginas) != 0)
        cantidad_paginas_a_reservar++;

    printf("Reservo %d paginas\n", cantidad_paginas_a_reservar);
    
    paginas_proceso = obtener_paginas_proceso(pid, &err);
    if (err){    
        paginas_proceso = malloc(sizeof(t_pagina_proceso));
        paginas_proceso->paginas = list_create();
        paginas_proceso->pid = pid;
    }

    for (int i = 0; i < maxima_cantidad_paginas && paginas_reservadas < cantidad_paginas_a_reservar; i++)
    {
        if (estado_frames[i] == 0)
        {
            estado_frames[i] = 1;
            list_add(paginas_proceso->paginas, (void *) (i * config->tamanio_pagina + (int) memoria));
            paginas_reservadas++;
            printf("Reservo pagina\n");
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
    pcb->tareas = tareas_cPID_recibidas->cantTareas * sizeof(t_tarea);;

    //COPIO EL PCB A tareas_pcb
    memcpy(tareas_pcb + offset, pcb, sizeof(t_PCB));

    //LIBERO EL PCB
    free(pcb);

    //GUARDO LAS TAREAS SERIALIZADAS EN MEMORIA
    offset = 0;
    int err;
    t_list *paginas_proceso = obtener_paginas_proceso(tareas_cPID_recibidas->PID, &err)->paginas;


    int ultPagina = (sizeof(t_tarea) * tareas_cPID_recibidas->cantTareas + sizeof(t_PCB)) % tamanio_paginas;

    for (int i = 0; i < list_size(paginas_proceso); i++)
    {
        if(i == list_size(paginas_proceso) - 1){
            //printf("Guardo en última pagina (la %d) las tareas y pcb %d bytes\n", i, ultPagina);
            memcpy((void *) list_get(paginas_proceso, i), tareas_pcb + offset, ultPagina);
        }
        else{
            //printf("Guardo pagina %d\n", i);
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

    return;
}

void guardar_tcb_paginacion(t_TCBcPID *datos_recibidos)
{

    //SERIALIZO EL TCB
    void *tcb = serializar_TCB(datos_recibidos->tcb);

    //GUARDO EL TCB EN LA MEMORIA
    int bytesOcupados        = bytes_ocupados_pid(datos_recibidos->pid);
    int fragmentacionInterna = calcular_fragmentacion(datos_recibidos->pid);
    int escribirDesdePagina;
    int offsetPrimeraPagina  = tamanio_paginas - fragmentacionInterna;
    int paginas_acceder      = 1;
    int bGuardadros          = 0;

    for (int i = fragmentacionInterna; i < sizeof(t_TCB); i+=tamanio_paginas)
        paginas_acceder++;

    int err;
    t_pagina_proceso* lista_paginas_proceso = obtener_paginas_proceso(datos_recibidos->pid, &err);

    escribirDesdePagina = (((int) list_get(lista_paginas_proceso->paginas, bytesOcupados / tamanio_paginas) - (int) memoria) / tamanio_paginas);
    //printf("Escribir desde pagina %d el byte %d\n", escribirDesdePagina, offsetPrimeraPagina);


    //EN EL CASO DE QUE ENTRE EN LA ULTIMA PAGINA
    if(fragmentacionInterna >= sizeof(t_TCB))
        memcpy((list_get(lista_paginas_proceso->paginas, escribirDesdePagina) + offsetPrimeraPagina), tcb, sizeof(t_TCB));
    //EN EL CASO DE QUE DEBA ALMACENARSE EN UNA NUEVA PAGINA O FINALIZAR LA ULTIMA Y UTILIZAR LA SIGUIENTE
    else{
        //ESCRIBO DESDE LA PAGINA QUE DEBA LLENAR HASTA TODAS LAS QUE NECESITE PARA GUARDAR EL TCB
        for (int i = escribirDesdePagina; i < (escribirDesdePagina + paginas_acceder); i++)
        {
            //printf("i: %d\n", i);
            //LA PRIMERA PAGINA DEBO UNICAMENTE LLENARLA CON LOS BYTES QUE FALTAN
            if(i==escribirDesdePagina){
                memcpy((list_get(lista_paginas_proceso->paginas, i) + offsetPrimeraPagina), tcb, fragmentacionInterna);
                bGuardadros += fragmentacionInterna;
            }
            //EL RESTO DE LAS PAGINAS DEBO LLENARLAS ENTERAS O CON EL RESTANTE DEL TCB
            else{
                if(bGuardadros + tamanio_paginas < sizeof(t_TCB)){
                    //printf("Se copia en %p lo que esta en %p, un total de %dB\n", list_get(lista_paginas_proceso->paginas, i), tcb + bGuardadros, tamanio_paginas);
                    memcpy(list_get(lista_paginas_proceso->paginas, i), tcb + bGuardadros, tamanio_paginas);
                    bGuardadros += tamanio_paginas;
                }
                else{
                    //printf("Se copia en %p lo que esta en %p, un total de %dB\n", list_get(lista_paginas_proceso->paginas, i), tcb + bGuardadros, sizeof(t_TCB)-bGuardadros);
                    memcpy(list_get(lista_paginas_proceso->paginas, i), tcb + bGuardadros, sizeof(t_TCB)-bGuardadros);
                    bGuardadros += sizeof(t_TCB)-bGuardadros;
                }
            }
        }
    } 

    //ACTUALIZO LA TABLA DEL PROCESO
    t_tabla_paginas_proceso *tabla_paginas_tcb = malloc(sizeof(t_tabla_paginas_proceso));
    
    tabla_paginas_tcb->id         = datos_recibidos->tcb.TID;
    tabla_paginas_tcb->tipo       = TCB;
    tabla_paginas_tcb->offset     = bytesOcupados;
    tabla_paginas_tcb->tamanio    = sizeof(t_TCB);
    tabla_paginas_tcb->modificado = 0;

    t_list* tabla_proceso = obtener_lista_proceso(datos_recibidos->pid, &err);
    list_add(tabla_proceso, tabla_paginas_tcb);
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