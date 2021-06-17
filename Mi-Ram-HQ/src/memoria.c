#include "proceso3.h"

int reservar_memoria(int bytes){

    if(strcmp(config->esquema_memoria, "SEGMENTACION") == 0){
        if(strcmp(config->esquema_memoria, "FF") == 0)
            return reservar_segmento_FF(bytes);
        if(strcmp(config->esquema_memoria, "BF") == 0)
            return reservar_segmento_BF(bytes);
    }
    else if(strcmp(config->esquema_memoria, "PAGINACION") == 0){

    }
}

int reservar_segmento_FF(int bytes){

    estado_segmentos segmento_obtenido, nuevo_segmento;

    for(int i = 0; i < cantidad_segmentos; i++){

        //Agarro el segmento en la posicion i
        segmento_obtenido = list_get(tabla_estado_segmentos, i);

        //En el caso de que no este ocupado
        if(segmento_obtenido->ocupado == 0 && segmento_obtenido->limite >= bytes){
            
            //Chequeo que no me este pasando de la memoria reservada
            if(segmento_obtenido->inicio + bytes < memoria + config->tamanio_memoria){

                //Inicio el nuevo segmento
                nuevo_segmento->inicio     = segmento_obtenido->inicio;
                nuevo_segmento->limite     = bytes;
                nuevo_segmento->ocupado    = 1;

                //Modifico el segmento libre
                segmento_obtenido->inicio += bytes;
                segmento_obtenido->limite -= bytes; 

                //Ajusto el libre
                list_replace(tabla_estado_segmentos, i, segmento_obtenido);

                //Añado el nuevo segmento a la lista
                list_add_in_index(tabla_estado_segmentos, i +1, nuevo_segmento);

                cantidad_segmentos++;
                
                return nuevo_segmento->inicio;
            }
        }
    }

    return -1;
}