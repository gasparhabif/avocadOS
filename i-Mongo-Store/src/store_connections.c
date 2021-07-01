#include "proceso2.h"

void discordiador_cxn_handler(void *arg)
{
    int server_instance = (int)arg;

    listen(server_instance, SOMAXCONN);

    // Aceptar conexión
    struct sockaddr_in client_dir;
    unsigned int dir_size = sizeof(socklen_t);
    int client = accept(server_instance, (void *)&client_dir, &dir_size);

    // Verificar conexión aceptada
    if (client == -1)
    {
        log_error(logger, "Error al aceptar conexión");
        return;
    }

    log_info(logger, "Se conectó el discordiador en el socket %d", client);

    while (1)
    {
    }
}

void accept_tripulantes(void *arg)
{
    int server_instance = (int)arg;

    while (1)
    {
        listen(server_instance, SOMAXCONN);

        // Aceptar conexión
        struct sockaddr_in client_dir;
        unsigned int dir_size = sizeof(socklen_t);
        int client = accept(server_instance, (void *)&client_dir, &dir_size);

        // Verificar conexión aceptada
        if (client == -1)
        {
            log_error(logger, "Error al aceptar conexión");
            break;
        }

        pthread_t connection_thread;
        pthread_create(&connection_thread, NULL, (void *)tripulante_cxn_handler, (void *)client);
        // Acá no ejecuto pthread_join() porque sino no crea nuevos threads hasta que no termine el último
    }
}

void tripulante_cxn_handler(void *arg)
{
    int client = (int)arg;
    bool tareas_finalizadas = false;

    // Recibir tripulante y su posición inicial
    t_envio_posicion *tripulante = recibir_paquete(client);
    log_info(logger, "Se conectó el tripulante TID: %d - Posición inicial: %d|%d", tripulante->TID, tripulante->pos.posX, tripulante->pos.posY);

    char *tid = string_itoa(tripulante->TID);

    // Archivo Tripulante#.ims
    char *bitacora_file_path = string_from_format("%s/Tripulante%s.ims", bitacoras_dir_path, tid);
    int bitacora_fd = open(bitacora_file_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    int cod_operacion;
    void *datos_recibidos = recibir_paquete_cCOP(client, &cod_operacion);

    while (!tareas_finalizadas)
    {
        // log_info(logger, "Se recibió el código de operación %d", cod_operacion);
        log_info(logger, "Socket %d -  Código %d", client, cod_operacion);

        switch (cod_operacion)
        {
        case MOVER_TRIPULANTE:
            registrarDesplazamiento();
            break;
        case INICIO_TAREA:
            registrarInicioTarea();
            break;
        case FIN_TAREA:
            registrarFinTarea();
            break;
        case INICIO_RESOLUCION_SABOTAJE:
            registrarAtencionSabotaje();
            break;
        case FIN_RESOLUCION_SABOTAJE:
            registrarResolucionSabotaje();
            break;
        case ENVIAR_PROXIMA_TAREA:;
            t_tarea *tarea_a_ejecutar = (t_tarea *)datos_recibidos;
            ejecutarTarea(tarea_a_ejecutar);
            break;
        default:
            log_error(logger, "Socket %d - Código %d desconocido", client, cod_operacion);
            tareas_finalizadas = true;
            break;
        }

        datos_recibidos = recibir_paquete_cCOP(client, &cod_operacion);
    }

    log_error(logger, "El tripulante del socket %d se desconectó", client);

    close(bitacora_fd);
    free(bitacora_file_path);
}
