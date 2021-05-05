#include "proceso1.h"

void INICIAR_PATOTA (char** parametros){

    int cantParametros = 0;
    while(parametros[cantParametros] != NULL)
        cantParametros++;

    //MANEJO DE ERRORES

    if(cantParametros < 3)
        printf("Error: INICIAR_PATOTA [CANT TRIPULANTES] [RUTA DE TAREAS] [POSICIONES]\n");
    else{
        FILE *fpTareas;
        int cantTripulantes = (int) parametros[1];

        fpTareas = fopen(parametros[2], "r");
        if(!fpTareas){
            printf("Error abriendo el archivo\n");
        }
        else{
            if(cantTripulantes < (cantParametros - 3))
                printf("Usted especifio mas tripulantes de los que desea crear\n");
            else{
                //iniciar la patota en el módulo Mi-RAM HQ y luego instanciar a cada tripulante
                //CREO LOS TCB
                
                TCB *tripulantes = malloc(sizeof(tripulantes) * cantTripulantes);
//TODO: el substring de 33 y 34, los unicos paramtros del for son posx y posy, fijate que estas rescribiendo los trupus salame
                for(int i = 3; i < parametros[cantParametros-1]; i++){
                    //tripulantes[i].TID = 
                    tripulantes[i]->estado = 'n';
                    tripulantes[i]->posX = parametros[i]
                    tripulantes[i]->posY = parametros[i];
                    tripulantes[i]->proximaInstruccion = ;
                    tripulantes[i]->puntero_PCB = ;
                }
                for(int i = 0; i < (cantTripulantes - cantParametros - 3); i++)){
                    //tripulantes[i].TID = ;
                    tripulantes[i]->estado = 'n';
                    tripulantes[i]->posX = 0;
                    tripulantes[i]->posY = 0;
                    tripulantes[i]->proximaInstruccion = ;
                    tripulantes[i]->puntero_PCB = ;
                }
                
                pthread_t *tripulantes = malloc(sizeof(pthread_t) * cantTripulantes);
            }
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
