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
        int cantTripulantes = atoi(parametros[1]);

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
                //EMPAQUETO LAS INSTRCCIONES
                leer_tareas(&tareas, fpTareas);
                //SERIALIZAR INSTRUCCIONES DEL ARCHIVO
                //serializarTareas(tareas);
                //ENVIAR TAREAS
                //.............
                //LIBERO LA MEMORIA DE LAS TAREAS
                //free(tareas);

                for(int i = 0; i< 6; i++){
                    printf("INST: %d\n", tareas[i].codigoTarea);
                    printf("PARA: %d\n", tareas[i].parametro);
                    printf("POSX: %d\n", tareas[i].posX);
                    printf("POSY: %d\n", tareas[i].posY);
                    printf("TIME: %d\n\n", tareas[i].duracionTarea);
                }
                
                //MANDO EL PCB

                //RECIBO LA DIRECCION LOGICA DEL PCB


                //CREO LOS TCB
                
                t_TCB *tripulantes_tcb;
                tripulantes_tcb = malloc(sizeof(t_TCB) * cantTripulantes);

                for(int i = 0; i < cantTripulantes; i++){
                    //EL TID SE AGREGA CUANDO EL HILO ESTA CREADO YA QUE EL EL TID CORRESPONDE AL THREAD_ID
                    tripulantes_tcb[i].estado = 'n';
                    tripulantes_tcb[i].posX = 0;
                    tripulantes_tcb[i].posY = 0;
                    tripulantes_tcb[i].proximaInstruccion = 0;
                    //tripulantes_tcb[i].puntero_PCB = ;
                }
                //Le asigno las posiciones a los tripilantes si es que vinieron seteadas por consola
                for(int i = 3; i < cantParametros; i++){
                    tripulantes_tcb[i-3].posX = atoi(string_substring(parametros[i], 0, 1));
                    tripulantes_tcb[i-3].posY = atoi(string_substring(parametros[i], 2, 1));
                }

                //CREO LOS THREADS DE LOS TRIPULANTES
                //cuando hago el free de estoo?
                pthread_t *threads_tripulantes = malloc(sizeof(pthread_t) * cantTripulantes);

                for(int i=0; i<cantTripulantes; i++){
                    pthread_create(&(threads_tripulantes[i]), NULL, (void *)tripulante, (void *)&tripulantes_tcb[i]);
                }

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
