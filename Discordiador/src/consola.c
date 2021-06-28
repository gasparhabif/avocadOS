#include "proceso1.h"

void INICIAR_PATOTA(char **parametros)
{

    static int cantParametros;
    cantParametros = 0;
    
    while (parametros[cantParametros] != NULL)
        cantParametros++;

    //MANEJO DE ERRORES
    if (cantParametros < 3)
        printf("Error: INICIAR_PATOTA [CANT TRIPULANTES] [RUTA DE TAREAS] [POSICIONES]\n");
    else
    {
        FILE *fpTareas;
        int cantTripulantes = atoi(parametros[1]);

        fpTareas = fopen(parametros[2], "r");
        if (!fpTareas)
        {
            printf("Error abriendo el archivo\n");
        }
        else
        {
            //printf("Tripulantes: %d, Param: %d\n", cantTripulantes, cantParametros);
            if (cantTripulantes < (cantParametros - 3))
                printf("Usted especifio mas tripulantes de los que desea crear\n");
            else
            {
                //LEO LAS INSTRUCCIONES DEL ARCHIVO Y LAS EMPAQUETO
                int cantTareas = 0;
                int errorLeyendoTareas = 0;
                
                t_tarea *tareas = leer_tareas(fpTareas, &cantTareas, &errorLeyendoTareas);

                if(errorLeyendoTareas)
                    return;
                
                //SERIALIZAR INSTRUCCIONES DEL ARCHIVO
                //printf("Serializando...\n");
                int tamanioSerializacion;
                void *d_Enviar = serializarTareas_cPID(tareas, patota_id, &tamanioSerializacion, cantTareas);
                //printf("Serializacion completada, %d bytes se enviaran\n", tamanioSerializacion);
                patota_id++;

                //ENVIAR TAREAS
                //printf("Enviando datos...\n");
                int bytesEnviados = send(sockfd_ram, d_Enviar, tamanioSerializacion, 0);
                //printf("Datos enviados! %d bytes\n", bytesEnviados);

                //LIBERO LA MEMORIA DE LAS TAREAS
                free(tareas);
                free(d_Enviar);

                //RECIBO LA DIRECCION LOGICA DEL PCB
                //printf("Recibiendo datos\n");
                int direccionPCB = (int) recibir_paquete(sockfd_ram);
                //printf("Pos recibida: %d\n", direccionPCB);

                //CREO LOS TCB
                t_TCB *tripulantes_tcb;
                tripulantes_tcb = malloc(sizeof(t_TCB) * cantTripulantes);

                for (int i = 0; i < cantTripulantes; i++)
                {
                    tripulantes_tcb[i].TID = 0;
                    //EL TID EN REALIDAD SE AGREGA CUANDO EL HILO ESTA CREADO YA QUE EL EL TID CORRESPONDE AL THREAD_ID
                    tripulantes_tcb[i].estado = NEW;
                    tripulantes_tcb[i].posX = 0;
                    tripulantes_tcb[i].posY = 0;
                    tripulantes_tcb[i].proximaInstruccion = 0;
                    tripulantes_tcb[i].puntero_PCB = direccionPCB;
                }

                //Le asigno las posiciones a los tripilantes si es que vinieron seteadas por consola
                for (int i = 3; i < cantParametros; i++)
                {
                    tripulantes_tcb[i - 3].posX = atoi(string_substring(parametros[i], 0, 1));
                    tripulantes_tcb[i - 3].posY = atoi(string_substring(parametros[i], 2, 1));
                }

                /*
                for (int i = 0; i < cantTripulantes; i++)
                {
                    printf("TID: %d\n",   tripulantes_tcb[i].TID);
                    printf("EST: %c\n",   tripulantes_tcb[i].estado);
                    printf("P_X: %d\n",   tripulantes_tcb[i].posX);
                    printf("P_Y: %d\n",   tripulantes_tcb[i].posY);
                    printf("P_I: %d\n",   tripulantes_tcb[i].proximaInstruccion);
                    printf("P_P: %d\n\n", tripulantes_tcb[i].puntero_PCB);
                }
                */

                //CREO LOS THREADS DE LOS TRIPULANTES
                
                //cuando hago el free de estoo?
                pthread_t *threads_tripulantes = malloc(sizeof(pthread_t) * cantTripulantes);

                for (int i = 0; i < cantTripulantes; i++)
                    pthread_create(&(threads_tripulantes[i]), NULL, (void *)tripulante, (void *) &(tripulantes_tcb[i]));
            
                printf("Iniciando %d tripulantes\n", cantTripulantes);
            
            }
        }
        fclose(fpTareas);
    }
}

void LISTAR_TRIPULANTES(char **parametros)
{
}

void EXPULSAR_TRIPULANTE(char **parametros)
{
}

void INICIAR_PLANIFICACION(char **parametros)
{
    if (planificando)
        printf("El planificador ya se encuentra trabajando!\n");
    else
    {
        log_info(logger, "Iniciando planificacion...\n");
        printf("Iniciando planificacion...\n");
        planificando = 1;
        for (int i = 0; i < config->grado_multitarea; i++)
            sem_post(&s_multiprocesamiento);     
        pthread_mutex_unlock(&mutex_block);
        pausar();
    }
}

void PAUSAR_PLANIFICACION(char **parametros)
{
    if (!planificando)
        printf("El planificador ya se encuentra pausado!\n");
    else
    {
        log_info(logger, "Pausando planificacion...\n");
        printf("Pausando planificacion...\n");
        planificando = 0;
        pausar();
    }
}

void OBTENER_BITACORA(char **parametros)
{
    
}
