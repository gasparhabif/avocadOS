#include "proceso3.h"

void *reservar_segmento_FF(int bytes)
{

    estado_segmentos *segmento_obtenido = malloc(sizeof(estado_segmentos));
    estado_segmentos *nuevo_segmento = malloc(sizeof(estado_segmentos));

    for (int i = 0; i < list_size(tabla_estado_segmentos); i++)
    {

        //Agarro el segmento en la posicion i
        segmento_obtenido = list_get(tabla_estado_segmentos, i);

        //En el caso de que no este ocupado
        if (segmento_obtenido->ocupado == 0 && segmento_obtenido->limite >= bytes)
        {

            //Inicio el nuevo segmento
            nuevo_segmento->inicio = segmento_obtenido->inicio;
            nuevo_segmento->limite = bytes;
            nuevo_segmento->ocupado = 1;

            //Modifico el segmento libre
            segmento_obtenido->inicio += bytes;
            segmento_obtenido->limite -= bytes;

            //Chequeo que no me este pasando de la memoria reservada
            if (((int) segmento_obtenido->inicio) < ((int) memoria + config->tamanio_memoria))
            {

                // En este caso podría ocurrir que los bytes entran perfecto en el
                // segmento obtenido, en tal caso no seria necesaria una modificacion
                // en la lista de segmentos
                if (segmento_obtenido->limite != 0)
                {
                    //Ajusto el libre
                    list_replace(tabla_estado_segmentos, i, nuevo_segmento);

                    //Añado el nuevo segmento a la lista
                    list_add_in_index(tabla_estado_segmentos, i + 1, segmento_obtenido);
                }
                else
                {
                    nuevo_segmento->ocupado = 1;
                    list_replace(tabla_estado_segmentos, i, nuevo_segmento);
                }

                return (void *)nuevo_segmento->inicio;
            }
        }
    }

    //OJO: Entra en un ciclo
    sinMemoria = 1;
    compactar(SIGUSR1);
    //return reservar_segmento_FF(bytes);

    for (int i = 0; i < list_size(tabla_estado_segmentos); i++)
    {

        //Agarro el segmento en la posicion i
        segmento_obtenido = list_get(tabla_estado_segmentos, i);

        //En el caso de que no este ocupado
        if (segmento_obtenido->ocupado == 0 && segmento_obtenido->limite >= bytes)
        {

            //Inicio el nuevo segmento
            nuevo_segmento->inicio = segmento_obtenido->inicio;
            nuevo_segmento->limite = bytes;
            nuevo_segmento->ocupado = 1;

            //Modifico el segmento libre
            segmento_obtenido->inicio += bytes;
            segmento_obtenido->limite -= bytes;

            //Chequeo que no me este pasando de la memoria reservada
            if (((int) segmento_obtenido->inicio) < ((int) memoria + config->tamanio_memoria))
            {

                // En este caso podría ocurrir que los bytes entran perfecto en el
                // segmento obtenido, en tal caso no seria necesaria una modificacion
                // en la lista de segmentos
                if (segmento_obtenido->limite != 0)
                {
                    //Ajusto el libre
                    list_replace(tabla_estado_segmentos, i, nuevo_segmento);

                    //Añado el nuevo segmento a la lista
                    list_add_in_index(tabla_estado_segmentos, i + 1, segmento_obtenido);
                }
                else
                {
                    nuevo_segmento->ocupado = 1;
                    list_replace(tabla_estado_segmentos, i, nuevo_segmento);
                }

                return (void *)nuevo_segmento->inicio;
            }
        }
    }

    return (void *)-1;
}

void *reservar_segmento_BF(int bytes)
{

    uint32_t inicio_minimo;
    uint32_t tamanio_minimo = -1;
    uint32_t pos_seg = -1;
    estado_segmentos *segmento_obtenido;// = malloc(sizeof(estado_segmentos));
    estado_segmentos *nuevo_segmento = malloc(sizeof(estado_segmentos));

    for (int l = 0; l < 2; l++)
    {
    
        tamanio_minimo = -1;
        pos_seg        = -1;

        for (int i = 0; i < list_size(tabla_estado_segmentos); i++)
        {

            //Agarro el segmento en la posicion i
            segmento_obtenido = list_get(tabla_estado_segmentos, i);

            //En el caso de que no este ocupado y la cantidad de byres entre
            if (segmento_obtenido->ocupado == 0 && segmento_obtenido->limite >= bytes)
            {

                //En el caso de que sea el primer segmento o que sea menor al menor ya tomado
                if ((pos_seg == -1) || segmento_obtenido->limite < tamanio_minimo)
                {
                    //Tomo un registro de este segmento libre
                    inicio_minimo = segmento_obtenido->inicio;
                    tamanio_minimo = segmento_obtenido->limite;
                    pos_seg = i;
                }
            }
        }

        //Tenemos un segmento candidato!
        if (tamanio_minimo != -1)
        {
            //Inicio el nuevo segmento
            nuevo_segmento->inicio = inicio_minimo;
            nuevo_segmento->limite = bytes;
            nuevo_segmento->ocupado = 1;

            //Modifico el segmento libre
            segmento_obtenido = list_get(tabla_estado_segmentos, pos_seg);
            segmento_obtenido->inicio += bytes;
            segmento_obtenido->limite -= bytes;

            //Chequeo que no me este pasando de la memoria reservada
            if (segmento_obtenido->inicio < (int)memoria + config->tamanio_memoria)
            {

                // En este caso podría ocurrir que los bytes entran perfectoen el
                // segmento obtenido, en tal caso no seria necesaria una modificacion
                // en la lista de segmentos
                if (segmento_obtenido->limite != 0)
                {
                    //Ajusto el libre
                    list_replace(tabla_estado_segmentos, pos_seg, nuevo_segmento);

                    //Añado el nuevo segmento a la lista
                    list_add_in_index(tabla_estado_segmentos, pos_seg + 1, segmento_obtenido);
                }
                else
                {
                    list_replace(tabla_estado_segmentos, pos_seg, nuevo_segmento);
                }

                return (void *)nuevo_segmento->inicio;
            }
        }
        
        if(l==0){
            sinMemoria = 1;
            compactar(SIGUSR1);
        }
    }

    //SI NO SE PUDO ENCONTRAR UN SEGMENTO, COMPACTAR Y PROBAR DE NUEVO
    //free(segmento_obtenido);
    //free(nuevo_segmento);

    return (void *) -1;
}

void liberar_memoria_segmentacion(int baseDelSegmento)
{

    estado_segmentos *segmento_obtenido = malloc(sizeof(estado_segmentos));

    for (int i = 0; i < list_size(tabla_estado_segmentos); i++)
    {

        //Agarro el segmento en la posicion i
        segmento_obtenido = list_get(tabla_estado_segmentos, i);

        if ((int)segmento_obtenido->inicio == baseDelSegmento)
        {
            segmento_obtenido->ocupado = 0;
            list_replace(tabla_estado_segmentos, i, segmento_obtenido);
            return;
        }
    }

    return;
}

void compactar(int sig)
{

    if (sig == SIGUSR1 && strcmp(config->esquema_memoria, "SEGMENTACION") == 0)
    {

        log_info(logger, "COMENZANDO LA COMPACTACION");

        /*
        printf("------------------------------------------------------------------\n");
        for (int i = 0; i < list_size(tabla_estado_segmentos); i++)
        {
            estado_segmentos *reg_seg = list_get(tabla_estado_segmentos, i);
            printf("SEG N°: %d\t", i);
            printf("Inicio: %d\t", reg_seg->inicio);
            printf("Tamaño: %d\t", reg_seg->limite);
            printf("Ocupado: %d\n", reg_seg->ocupado);
        }
        printf("------------------------------------------------------------------\n");
        */

        if(!sinMemoria){
            pthread_mutex_lock(&acceso_memoria);
            pthread_mutex_lock(&m_procesos);
        }
        sinMemoria = 0;

        t_list *tabla_nueva = list_create();
        estado_segmentos *estado = malloc(sizeof(estado_segmentos));
        void *pos_sig = memoria;
        int memoria_libre = config->tamanio_memoria;

        for (int i = 0; i < list_size(tabla_estado_segmentos); i++)
        {
            //printf("Segmento: %d\n", i);

            estado = list_get(tabla_estado_segmentos, i);

            if (estado->ocupado == 1)
            {

                actualizar_tabla_procesos(estado->inicio, (int)pos_sig);

                //printf("Copio a %d lo que esta en %d. En total, %d bytes\n", (int) pos_sig, estado->inicio, estado->limite);

                memcpy(pos_sig, (void *)estado->inicio, estado->limite);
                estado->inicio = (int)pos_sig;

                //printf("Agrego a la nueva tabla:\n\tInicio: %d\n\tLimite: %d\n\tOcupado: %d\n", estado->inicio , estado->limite, estado->ocupado);

                list_add(tabla_nueva, estado);
                pos_sig += estado->limite;

                //printf("pos_sig: %d\n", (int) pos_sig);

                memoria_libre -= estado->limite;
            }
        }

        estado->inicio = (int)pos_sig;
        estado->limite = memoria_libre;
        estado->ocupado = 0;
        //printf("Agrego a la nueva tabla:\n\tInicio: %d\n\tLimite: %d\n\tOcupado: %d\n", estado->inicio , estado->limite, estado->ocupado);
        list_add(tabla_nueva, estado);

        tabla_estado_segmentos = tabla_nueva;

        
        log_info(logger, "TERMINE LA COMPACTACION\n");
        printf("------------------------------------------------------------------\n");
        for (int i = 0; i < list_size(tabla_nueva); i++)
        {
            estado_segmentos *reg_seg = list_get(tabla_nueva, i);
            printf("SEG N°: %d\t", i);
            printf("Inicio: %d\t", reg_seg->inicio);
            printf("Tamaño: %d\t", reg_seg->limite);
            printf("Ocupado: %d\n", reg_seg->ocupado);
        }
        printf("------------------------------------------------------------------\n");
        

        pthread_mutex_unlock(&acceso_memoria);
        pthread_mutex_unlock(&m_procesos);
    }

    return;
}

void actualizar_tabla_procesos(int posBuscada, int posNueva)
{

    t_list *tablaUnProceso;
    t_registro_segmentos *reg_seg = malloc(sizeof(t_registro_segmentos));

    //RECORRO LA LISTA DE PROCESOS
    for (int i = 0; i < list_size(tabla_procesos); i++)
    {
        tablaUnProceso = list_get(tabla_procesos, i);

        //RECORRO LOS SEGMENTO DE CADA PROCESO
        for (int j = 0; j < list_size(tablaUnProceso); j++)
        {
            reg_seg = list_get(tablaUnProceso, j);

            //VERIFICO SI EL SEGMENTO ES EL SOLICITADO
            if (reg_seg->base == (void *)posBuscada)
            {
                reg_seg->base = (void *)posNueva;
                list_replace(tablaUnProceso, j, reg_seg);

                //ACTUALIZO LOS PUNTEROS
                if (reg_seg->tipo == PCB)
                    cambio_posicion_pcb(posNueva, reg_seg->id);
                else if (reg_seg->tipo == TAREAS)
                    cambio_posicion_tareas(posNueva, obtener_PIDproceso(tablaUnProceso));

                return;
            }
        }
    }
}

void cambio_posicion_pcb(int posNueva, int pid)
{

    t_list *lista_proceso = buscar_lista_proceso(pid);
    t_registro_segmentos *reg_seg = malloc(sizeof(t_registro_segmentos));

    //RECORRO LA LISTA DE SEGMENTOS DEL PROCESO
    for (int i = 0; i < list_size(lista_proceso); i++)
    {
        reg_seg = list_get(lista_proceso, i);

        //EN CASO DE QUE ME ENCUENTRE UN TCB LE MODIFICO SU PUNTERO AL PCB
        if (reg_seg->tipo == TCB)
        {
            //CREO UN TCB PARA TRAERLO DE MEMORIA Y TRABAJARLO
            t_TCB *tcb = malloc(sizeof(t_TCB));

            //ME COPIO EL TCB DE MEMORIA
            memcpy(tcb, reg_seg->base, sizeof(t_TCB));

            //MODIFICO EL ESTADO
            tcb->puntero_PCB = posNueva;

            //LLEVO EL TCB NUEVAMENTE A MEMORIA
            memcpy(reg_seg->base, tcb, sizeof(t_TCB));
        }
    }

    //free(reg_seg);
    return;
}

void cambio_posicion_tareas(int posNueva, int pid)
{

    t_list *lista_proceso = buscar_lista_proceso(pid);
    t_registro_segmentos *reg_seg = malloc(sizeof(t_registro_segmentos));

    //RECORRO LA LISTA DE SEGMENTOS DEL PROCESO
    for (int i = 0; i < list_size(lista_proceso); i++)
    {
        reg_seg = list_get(lista_proceso, i);

        //CUANDO ENCUENTRO EL PCB LE MODIFICO SU PUNTERO A LAS TAREAS
        if (reg_seg->tipo == PCB)
        {
            //CREO UN PCB PARA TRAERLO DE MEMORIA Y TRABAJARLO
            t_PCB *pcb = malloc(sizeof(t_PCB));

            //ME COPIO EL PCB DE MEMORIA
            memcpy(pcb, reg_seg->base, sizeof(t_PCB));

            //MODIFICO EL PUNTERO A LAS TAREAS
            pcb->tareas = posNueva;

            //LLEVO EL PCB NUEVAMENTE A MEMORIA
            memcpy(reg_seg->base, pcb, sizeof(t_PCB));
        }
    }
}