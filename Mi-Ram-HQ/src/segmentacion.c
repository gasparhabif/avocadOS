#include "proceso3.h"

void* reservar_segmento_FF(int bytes){

    estado_segmentos *segmento_obtenido = malloc(sizeof(estado_segmentos));
    estado_segmentos *nuevo_segmento    = malloc(sizeof(estado_segmentos));

    for(int i = 0; i < cantidad_segmentos; i++){

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
            
                cantidad_segmentos++;

                free(segmento_obtenido);
                return (void *) nuevo_segmento->inicio;
            }
        }
    }

    free(segmento_obtenido);
    free(nuevo_segmento);
    
    compactar(SIGUSR1);
    return reservar_segmento_BF(bytes);
}

void* reservar_segmento_BF(int bytes){

    uint32_t          inicio_minimo;
    uint32_t          tamanio_minimo    = -1;
    uint32_t          pos_seg           = 0;
    estado_segmentos *segmento_obtenido = malloc(sizeof(estado_segmentos));
    estado_segmentos *nuevo_segmento    = malloc(sizeof(estado_segmentos));

    for(int i = 0; i < cantidad_segmentos; i++){

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
            
            cantidad_segmentos++;

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

void compactar(int sig){

    if(sig == SIGUSR1 && strcmp(config->esquema_memoria, "SEGMENTACION") == 0)
    {

    }

    return;
}