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
                
                //LEO LAS INSTRUCCIONES DEL ARCHIVO
                t_tareas *tareas = NULL;
                leer_tareas(tareas, fpTareas);
                //SERIALIZAR INSTRUCCIONES DEL ARCHIVO
                serializarTareas(tareas);

                //CREO LOS TCB
                t_TCB *tripulantes_tcb;
                tripulantes_tcb = malloc(sizeof(TCB) * cantTripulantes);

                for(int i = 0; i < cantTripulantes; i++){
                    //tripulantes_tcb[i].TID = ;
                    tripulantes_tcb[i].estado = 'n';
                    tripulantes_tcb[i].posX = 0;
                    tripulantes_tcb[i].posY = 0;
                    tripulantes_tcb[i].proximaInstruccion = 0;
                    //tripulantes_tcb[i].puntero_PCB = ;
                }
                //Le asigno las posiciones a los tripilantes si es que vinieron seteadas por consola
                for(int i = 3; i < cantParametros; i++){
                    tripulantes_tcb[i-3].posX = (uint32_t) parametros[i];
                    tripulantes_tcb[i-3].posY = (uint32_t) parametros[i];
                }
                
                //pthread_t *tripulantes_hilos = malloc(sizeof(pthread_t) * cantTripulantes);

                free(tareas);
                free(tripulantes_tcb);
            }
        }

    fclose(fpTareas);
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
