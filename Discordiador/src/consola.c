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

                t_tarea *tareas = leer_tareas(fpTareas, &cantTareas);

                //SERIALIZAR INSTRUCCIONES DEL ARCHIVO
                //printf("Serializando...\n");
                int tamanioSerializacion;
                void *d_Enviar = serializarTareas_cPID(tareas, patota_id, &tamanioSerializacion, cantTareas);
                //printf("Serializacion completada, %d bytes se enviaran\n", tamanioSerializacion);

                //ENVIAR TAREAS
                //printf("Enviando datos...\n");
                send(sockfd_ram, d_Enviar, tamanioSerializacion, 0);

                //LIBERO LA MEMORIA DE LAS TAREAS
                free(tareas);
                free(d_Enviar);

                //RECIBO LA DIRECCION LOGICA DEL PCB
                //printf("Recibiendo datos\n");
                int direccionPCB = (int)recibir_paquete(sockfd_ram);
                //printf("Pos recibida: %p\n", direccionPCB);

                if (direccionPCB < 0)
                {
                    log_info(logger, "Error guardando PCB y/o tareas en memoria, no se crearon los tripulantes");
                    printf("Error guardando PCB y/o tareas en memoria, no se crearon los tripulantes\n");
                    return;
                }

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
                pthread_t threads_tripulantes[cantTripulantes];

                for (int i = 0; i < cantTripulantes; i++)
                {

                    //CREO LA ESTRUCTURA DE ADMINISTACION DEL TRIPULANTE Y LE ASIGNO INFORMACION QUE DESPUES NO LE VOY A PODER ASIGNAR
                    t_admin_tripulantes *admin = malloc(sizeof(t_admin_tripulantes));
                    admin->pid = patota_id;

                    //CREO LOS PARAMETROS PARA EL TRIPULANTE A CREAR
                    t_parametros_tripulantes *parametros_tripulante = malloc(sizeof(t_parametros_tripulantes));
                    //parametros_tripulante->tcbTripulante            = malloc(sizeof(t_TCB));
                    parametros_tripulante->tcbTripulante = tripulantes_tcb[i];
                    parametros_tripulante->admin = admin;

                    //CREO EL TRIPULANTE
                    pthread_create(&(threads_tripulantes[i]), NULL, (void *)tripulante, (t_parametros_tripulantes *)parametros_tripulante);
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
    //SOLICITO LOS TRIPULANTES DE LA MEMORIA RAM
    int tamanioSerializacion;
    void *paquete = serializarInt(1, SOLICITAR_LISTA, &tamanioSerializacion);
    send(sockfd_ram, paquete, tamanioSerializacion, 0);
    free(paquete);

    //RECIBO LOS TRIPULANTES DE LA RAM
    t_ListaTripulantes *tripulantesRecibidos = (t_ListaTripulantes *)recibir_paquete(sockfd_ram);

    //LISTO LOS TRIPULANTES
    time_t tiempo = time(0);
    tlocal = localtime(&tiempo);
    char *hora = malloc(128);
    strftime(hora, 128, "%d/%m/%y %H:%M:%S", tlocal);

    printf("--------------------------------------------------------------------------------\n");

    printf("Estado de la nave: %s\n", hora);
    free(hora);
    printf("Actualmente hay %d tripulantes en la nave\n", tripulantesRecibidos->cantTripulantes);

    for (int i = 0; i < tripulantesRecibidos->cantTripulantes; i++)
    {
        printf("Patota: %d\t", tripulantesRecibidos->tripulantes[i].PID);
        printf("Tripulante: %d\t", tripulantesRecibidos->tripulantes[i].TID);
        printf("Status: %c\t", tripulantesRecibidos->tripulantes[i].estado);
        printf("Pos X: %d\t", tripulantesRecibidos->tripulantes[i].posX);
        printf("Pos Y: %d\t", tripulantesRecibidos->tripulantes[i].posY);
        printf("Nro. Inst: %d\n", tripulantesRecibidos->tripulantes[i].proximaInstruccion);
    }

    printf("--------------------------------------------------------------------------------\n");
}

void EXPULSAR_TRIPULANTE(char **parametros)
{

    //SACO AL TRIPULANTE DE LA LISTA EN LA QUE SE ENCUENTRE (SI EXISTE)
    if (existeTripulante(atoi(parametros[1])))
    {

        //LE DOY EL AVISO DE MUERTE AL TRIPULANTE
        avisoDeMuerte(atoi(parametros[1]));

        //DOY EL AVISO
        printf("Se le dio el aviso de muerte al tripulante %d\n", atoi(parametros[1]));
        log_info(logger, "Se le dio el aviso de muerte al tripulante %d\n", atoi(parametros[1]));
    }
    else
        printf("El tripulante %d no existe\n", atoi(parametros[1]));
}

void INICIAR_PLANIFICACION(char **parametros)
{
    if (pausar(0))
    {
        log_info(logger, "Iniciando planificacion...\n");
        printf("Iniciando planificacion...\n");
    }
    else
        printf("El planificador ya se encuentra trabajando!\n");
}

void PAUSAR_PLANIFICACION(char **parametros)
{

    if (pausar(1))
    {
        log_info(logger, "Pausando planificacion...\n");
        printf("Pausando planificacion...\n");
    }
    else
        printf("El planificador ya se encuentra pausado!\n");
}

void OBTENER_BITACORA(char **parametros)
{
    //SOLICITO LA BITACORA DE UN TRIPULANTE
    int bEnviar;
    void *dEnviar = serializarInt(atoi(parametros[1]), SOLICITAR_BITACORA, &bEnviar);
    send(sockfd_mongo_bitacoras, dEnviar, bEnviar, 0);
    free(dEnviar);

    //RECIBO LA BITACORA DEL MONGO
    char *bitacoraRecibida = (char *)recibir_paquete(sockfd_mongo_bitacoras);
    char **bitacoras_separadas = string_split(bitacoraRecibida, "$");

    // IMPRIMO LA BITACORA
    printf("--------------------------------------------------------------------------------------------------------\n");
    printf("Esta es la bitacora del tripulante %d\n", atoi(parametros[1]));
    for (int i = 0; bitacoras_separadas[i] != NULL; i++)
        printf("ACCION N°%d: %s\n", i, bitacoras_separadas[i]);
    printf("--------------------------------------------------------------------------------------------------------\n");
}

void IMPRIMIR_SEGMENTOS(char **parametros)
{
    int bEnviar;
    void *dEnviar = serializarInt(0, IMPRIMIR_SEGMENTACION, &bEnviar);
    send(sockfd_ram, dEnviar, bEnviar, 0);
    free(dEnviar);
}
