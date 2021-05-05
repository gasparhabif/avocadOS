#include "proceso1.h"

void INICIAR_PATOTA (char** parametros){

    int i = 0;
    while(parametros[i] != NULL)
        i++;

    if(i < 3)
        printf("Error: INICIAR_PATOTA [CANT TRIPULANTES] [RUTA DE TAREAS] [POSICIONES]");
    else{
        FILE *fpTareas;
        int cantTripulantes = (int) parametros[1];

        fpTareas = fopen(parametros[2], "r");
        if(!fpTareas){
            printf("Error abriendo el archivo\n");
        }
        else{
            //iniciar la patota en el módulo Mi-RAM HQ y luego instanciar a cada tripulante
        }
    }
}

void LISTAR_TRIPULANTES (char** parametros){

}

void EXPULSAR_TRIPULANTE (char** parametros){

}

void INICIAR_PLANIFICACION (char** parametros){

}

void PAUSAR_PLANIFICACION (char** parametros){

}

void OBTENER_BITACORA (char** parametros){

}
