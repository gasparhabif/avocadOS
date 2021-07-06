#include "proceso3.h"

void* reservar_segmento_FF(int bytes){

    estado_segmentos *segmento_obtenido = malloc(sizeof(estado_segmentos));
    estado_segmentos *nuevo_segmento    = malloc(sizeof(estado_segmentos));

    for(int i = 0; i < list_size(tabla_estado_segmentos); i++){

        //Agarro el segmento en la posicion i
        segmento_obtenido = list_get(tabla_estado_segmentos, i);

        //En el caso de que no este ocupado
        if(segmento_obtenido->ocupado == 0 && segmento_obtenido->limite >= bytes){
            
            //Inicio el nuevo segmento
            nuevo_segmento->inicio     = segmento_obtenido->inicio;
            nuevo_segmento->limite     = bytes;
            nuevo_segmento->ocupado    = 1;

            //Modifico el segmento libre
            segmento_obtenido->inicio += bytes;
            segmento_obtenido->limite -= bytes;

            //Chequeo que no me este pasando de la memoria reservada
            if(segmento_obtenido->inicio + bytes < (int) memoria + config->tamanio_memoria){

                // En este caso podría ocurrir que los bytes entran perfectoen el 
                // segmento obtenido, en tal caso no seria necesaria una modificacion 
                // en la lista de segmentos
                if(segmento_obtenido->limite != 0){
                    //Ajusto el libre
                    list_replace(tabla_estado_segmentos, i, nuevo_segmento);

                    //Añado el nuevo segmento a la lista
                    list_add_in_index(tabla_estado_segmentos, i+1, segmento_obtenido);
                }
                
                return (void *) nuevo_segmento->inicio;
            }
        }
    }
    
    //OJO: Entra en un ciclo
    //compactar(SIGUSR1);
    //return reservar_segmento_FF(bytes);
    return (void *) -1;
}

void* reservar_segmento_BF(int bytes){

    uint32_t          inicio_minimo;
    uint32_t          tamanio_minimo    = -1;
    uint32_t          pos_seg           = 0;
    estado_segmentos *segmento_obtenido = malloc(sizeof(estado_segmentos));
    estado_segmentos *nuevo_segmento    = malloc(sizeof(estado_segmentos));

    for(int i = 0; i < list_size(tabla_estado_segmentos); i++){

        //Agarro el segmento en la posicion i
        segmento_obtenido = list_get(tabla_estado_segmentos, i);

        //En el caso de que no este ocupado y la cantidad de byres entre
        if(segmento_obtenido->ocupado == 0 && segmento_obtenido->limite >= bytes){
        
            //En el caso de que sea el primer segmento o que sea menor al menor ya tomado
            if (pos_seg || segmento_obtenido->limite < tamanio_minimo)
            {
                //Tomo un registro de este segmento libre
                inicio_minimo  = segmento_obtenido->inicio;
                tamanio_minimo = segmento_obtenido->limite;
                pos_seg = i;
            }
        }
    }

    //Tenemos un segmento candidato!
    if (tamanio_minimo != -1)
    {
        //Inicio el nuevo segmento
        nuevo_segmento->inicio  = inicio_minimo;
        nuevo_segmento->limite  = tamanio_minimo;
        nuevo_segmento->ocupado = 1;

        //Modifico el segmento libre
        segmento_obtenido->inicio += bytes;
        segmento_obtenido->limite -= bytes;

        //Chequeo que no me este pasando de la memoria reservada
        if(segmento_obtenido->inicio + bytes < (int) memoria + config->tamanio_memoria){
            
            // En este caso podría ocurrir que los bytes entran perfectoen el 
            // segmento obtenido, en tal caso no seria necesaria una modificacion 
            // en la lista de segmentos

            if(segmento_obtenido->limite != 0){
                //Ajusto el libre
                list_replace(tabla_estado_segmentos, pos_seg, nuevo_segmento);

                //Añado el nuevo segmento a la lista
                list_add_in_index(tabla_estado_segmentos, pos_seg, segmento_obtenido);
            }

            free(segmento_obtenido);
            return (void *) nuevo_segmento->inicio;
        }
    }

    //SI NO SE PUDO ENCONTRAR UN SEGMENTO, COMPACTAR Y PROBAR DE NUEVO

    free(segmento_obtenido);
    free(nuevo_segmento);
    
    compactar(SIGUSR1);
    return reservar_segmento_BF(bytes);
}

void liberar_memoria_segmentacion (int baseDelSegmento){
    
    estado_segmentos *segmento_obtenido = malloc(sizeof(estado_segmentos));

    for(int i = 0; i < list_size(tabla_estado_segmentos); i++){

        //Agarro el segmento en la posicion i
        segmento_obtenido = list_get(tabla_estado_segmentos, i);

        if((int) segmento_obtenido->inicio == baseDelSegmento){
            segmento_obtenido->ocupado = 0;
            list_replace(tabla_estado_segmentos, i, segmento_obtenido);
            return;
        }
    }

    return;
}

void compactar(int sig){

    if(sig == SIGUSR1 && strcmp(config->esquema_memoria, "SEGMENTACION") == 0)
    {
        pthread_mutex_lock(&acceso_memoria);
        
        log_info(logger, "COMENZANDO LA COMPACTACION");

        estado_segmentos *estado = malloc(sizeof(estado_segmentos));

        for (int i = 0; i < list_size(tabla_estado_segmentos); i++)
        {
            estado = list_get(tabla_estado_segmentos, i);

            if(estado->ocupado == 0 && i < list_size(tabla_estado_segmentos)){

                int pos = buscar_siguiente_segmento_ocupado(i);

                //memcpy(estado->base)

                actualizar_registro_segmento(pos);
            }
        }

        //RECORRO LOS ESTADOS Y CUANDO ENCUENTRO EL PRIMERO EN 0, ELIMINO TODOS A PARTIR DE AHI Y ME QUEDO CON UNO GRANDE
        int bytesOcupados;
        int pos_libertad;
        int index_ultimo = ultimo_ocupado(&bytesOcupados, &pos_libertad);
        
        //ELIMINO TODOS LOS SEGMENTOS NO USADOS QUE SE ENCUENTREN AL FINAL
        for (int i = index_ultimo; i < tabla_estado_segmentos; i++)
            list_remove(tabla_estado_segmentos, i);
        
        //PONGO AL FINAL UN SEGMENTO GRANDE LIBRE
        estado->base    = pos_libertad;
        estado->limite  = config->tamanio_memoria - bytesOcupados;
        estado->ocupado = 0;
        
        pthread_mutex_unlock(&acceso_memoria);
    }

    return;
}

int ultimo_ocupado(int *bytesOcupado, int *pos_libertad){

    estado_segmentos *estado = malloc(sizeof(estado_segmentos));
    *bytesOcupado = 0;

    for (int i = 0; i < list_size(tabla_estado_segmentos); i++)
    {
            estado = list_get(tabla_estado_segmentos, i);

            if(estado->ocupado == 0){
                *pos_libertad = estado->base;
                return i;
            }
            else
                *bytesOcupado += estado->limite;
    }        
}

int buscar_siguiente_segmento_ocupado(i){
    
    estado_segmentos *estado = malloc(sizeof(estado_segmentos));

    for (int j = i; j < list_size(tabla_estado_segmentos); j++)
    {
        estado = list_get(tabla_estado_segmentos, j);

        if(estado->ocupado == 1){
            estado->ocupado == 0;
            list_replace(tabla_estado_segmentos, j, estado);
            return estado->base;
        }
    }

    return -1;
}

void actualizar_registro_segmento(int posBuscada, int posNueva){
    
    t_list *tablaUnProceso;
    t_registro_segmentos *reg_seg = malloc(sizeof(t_registro_segmentos));

    //RECORRO LA LISTA DE PROCESOS
    for(int i=0; i < list_size(tabla_procesos); i++){
        tablaUnProceso = list_get(tabla_procesos, i);

        //RECORRO LOS SEGMENTO DE CADA PROCESO
        for(int j=0; j < list_size(tablaUnProceso); j++){
            reg_seg = list_get(tablaUnProceso, j);

            //VERIFICO SI EL SEGMENTO ES EL SOLICITADO
            if(reg_seg->base == posBuscada){
                reg_seg->base = posNueva;
                list_replace(tablaUnProceso, j, reg_seg);
                return;
            }
        }
    }
}
