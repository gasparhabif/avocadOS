#include "proceso3.h"

void* reservar_memoria(int bytes){

    if(strcmp(config->esquema_memoria, "SEGMENTACION") == 0){
        if(strcmp(config->esquema_memoria, "FF") == 0)
            return reservar_segmento_FF(bytes);
        if(strcmp(config->esquema_memoria, "BF") == 0)
            return reservar_segmento_BF(bytes);
    }
    else if(strcmp(config->esquema_memoria, "PAGINACION") == 0){

    }

    return (void*) -1;
}