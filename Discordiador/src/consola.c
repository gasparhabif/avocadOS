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

                //ENVIAR TAREAS
                //printf("Enviando datos...\n");
                int bytesEnviados = send(sockfd_ram, d_Enviar, tamanioSerializacion, 0);
                //printf("Datos enviados! %d bytes\n", bytesEnviados);

                //LIBERO LA MEMORIA DE LAS TAREAS
                free(tareas);
                free(d_Enviar);

                //RECIBO LA DIRECCION LOGICA DEL PCB
                //printf("Recibiendo datos\n");
                //int direccionPCB = (int) recibir_paquete(sockfd_ram);
                //printf("Pos recibida: %d\n", direccionPCB);

                //CREO LOS TCB
                t_TCB tripulantes_tcb[cantTripulantes];

                for (int i = 0; i < cantTripulantes; i++)
                {
                    tripulantes_tcb[i].TID = 0;
                    //EL TID EN REALIDAD SE AGREGA CUANDO EL HILO ESTA CREADO YA QUE EL EL TID CORRESPONDE AL THREAD_ID
                    tripulantes_tcb[i].estado = NEW;
                    tripulantes_tcb[i].posX = 0;
                    tripulantes_tcb[i].posY = 0;
                    tripulantes_tcb[i].proximaInstruccion = 0;
                    tripulantes_tcb[i].puntero_PCB = /*direccionPCB*/20;
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
                pthread_t threads_tripulantes[cantTripulantes];

                for (int i = 0; i < cantTripulantes; i++){
                    
                    //CREO LA ESTRUCTURA DE ADMINISTACION DEL TRIPULANTE Y LE ASIGNO INFORMACION QUE DESPUES NO LE VOY A PODER ASIGNAR                    
                    t_admin_tripulantes *admin = malloc(sizeof(t_admin_tripulantes));
                    admin->pid                 = patota_id;
                    admin->threadTripulante    = threads_tripulantes[i];

                    //CREO LOS PARAMETROS PARA EL TRIPULANTE A CREAR
                    t_parametros_tripulantes *parametros_tripulante = malloc(sizeof(t_parametros_tripulantes));
                    //parametros_tripulante->tcbTripulante            = malloc(sizeof(t_TCB));
                    parametros_tripulante->tcbTripulante            = tripulantes_tcb[i];
                    parametros_tripulante->admin                    = admin;

                    //CREO EL TRIPULANTE
                    pthread_create(&(threads_tripulantes[i]), NULL, (void *)tripulante, (t_parametros_tripulantes *) parametros_tripulante);
                    pthread_detach(threads_tripulantes[i]);                   
                }

                patota_id++;
                
                printf("Iniciando %d tripulantes\n", cantTripulantes);
            
            }
            
            fclose(fpTareas);
        }
    }
}

void LISTAR_TRIPULANTES(char **parametros)
{
	time_t tiempo = time(0);
    tlocal = localtime(&tiempo);
    char *hora = malloc(128);
    strftime(hora,128,"%d/%m/%y %H:%M:%S",tlocal);

    printf("--------------------------------------------------------------------------------\n");
    
    printf("Estado de la nave: %s\n", hora);
    free(hora);

    t_admin_tripulantes *aux_admin;
    
    if(sabotaje){

        pthread_mutex_lock(&m_listaBlock);
        for (int i = 0; i < list_size(bloq); i++)
        {
            
            aux_admin = list_get(bloq, i);
            printf("Patota: %d\t",     aux_admin->pid);
            printf("Tripulante: %d\t", aux_admin->tid);
            printf("Status: BLOQ E\t");
            printf("Pos X: %d\t",      aux_admin->posX);
            printf("Pos Y: %d\n",      aux_admin->posY);
        }
        pthread_mutex_unlock(&m_listaBlock);

    }
    else{
        pthread_mutex_lock(&m_listaReady);
        for (int i = 0; i < list_size(ready); i++)
        {
            
            aux_admin = list_get(ready, i);
            printf("Patota: %d\t",     aux_admin->pid);
            printf("Tripulante: %d\t", aux_admin->tid);
            printf("Status: %c\t",     aux_admin->estado);
            printf("Pos X: %d\t",      aux_admin->posX);
            printf("Pos Y: %d\n",      aux_admin->posY);
        }
        pthread_mutex_unlock(&m_listaReady);

        pthread_mutex_lock(&m_listaExec);
        for (int i = 0; i < list_size(exec); i++)
        {
            aux_admin = list_get(exec, i);
            printf("Patota: %d\t",     aux_admin->pid);
            printf("Tripulante: %d\t", aux_admin->tid);
            printf("Status: %c\t",     aux_admin->estado);
            printf("Pos X: %d\t",      aux_admin->posX);
            printf("Pos Y: %d\n",      aux_admin->posY);
        }
        pthread_mutex_unlock(&m_listaExec);

        pthread_mutex_lock(&m_listaBlockIO);
        for (int i = 0; i < list_size(bloq_IO); i++)
        {
            
            aux_admin = list_get(bloq_IO, i);
            printf("Patota: %d\t",     aux_admin->pid);
            printf("Tripulante: %d\t", aux_admin->tid);
            printf("Status: %c\t",     aux_admin->estado);
            printf("Pos X: %d\t",      aux_admin->posX);
            printf("Pos Y: %d\n",      aux_admin->posY);
        }
        pthread_mutex_unlock(&m_listaBlockIO);
    }

    printf("--------------------------------------------------------------------------------\n");
    
}

void EXPULSAR_TRIPULANTE(char **parametros)
{
    pthread_t thread_eliminar;

    //SACO AL TRIPULANTE DE LA LISTA EN LA QUE SE ENCUENTRE (SI EXISTE)
    if(matarTripulante(atoi(parametros[1]), &thread_eliminar)){

        //ELIMINO AL TRIPULANTE DE LA MEMORIA
        int bEnviar;
        void *dEnviar = serializarInt(atoi(parametros[1]), ELIMINAR_TRIPULANTE, &bEnviar);
        send(sockfd_ram, dEnviar, bEnviar, 0);
        free(dEnviar);

        //ELIMINO EL THREAD
        pthread_cancel(thread_eliminar);

        //DOY EL AVISO
        printf("Se elimino al tripulante %d\n", atoi(parametros[1]));
        log_info(logger, "Se elimino al tripulante %d\n", atoi(parametros[1]));
    }
    else
        printf("El tripulante %d no existe\n", atoi(parametros[1]));
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
