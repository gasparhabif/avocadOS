#include "proceso2.h"

void accept_tripulantes(void *arg)
{
    int server_instance = (int)arg;

    while (1) // TODO: Usar una flag para emprolijar
    {
        listen(server_instance, SOMAXCONN);

        struct sockaddr_in client_dir;
        unsigned int dir_size = sizeof(socklen_t);
        int client = accept(server_instance, (void *)&client_dir, &dir_size);

        if (client == -1)
        {
            log_error(logger, "Error al aceptar conexión");
            break;
        }

        pthread_t connection_thread;
        pthread_create(&connection_thread, NULL, (void *)tripulante_cxn_handler, (void *)client);
        pthread_detach(connection_thread);
    }
}

void tripulante_cxn_handler(void *arg)
{
    int client = (int)arg;
    bool tareas_finalizadas = false;

    // Recibir tripulante y su posición inicial
    t_tidCposicion *tripulante = recibir_paquete(client);
    log_info(logger, "Se conectó el tripulante TID: %d - Posición inicial: %d|%d", tripulante->TID, tripulante->pos.posX, tripulante->pos.posY);

    // Obtener path
    char *tid = string_itoa(tripulante->TID);
    // char *bitacora_file_path = string_from_format("%s/Tripulante%s.ims", bitacoras_dir_path, tid);
    char *bitacora_file_path = string_from_format("%s/Tripulante.ims", bitacoras_dir_path, tid);

    if (!file_exists(bitacora_file_path))
    {
        create_bitacora(bitacora_file_path);
        log_info(logger, "Se creó Tripulante%s.ims", tid);
    }

    // Cargar Tripulante#.ims
    t_bitacora_mongo *bitacora = load_bitacora(bitacora_file_path);
    log_info(logger, "Se cargó Tripulante%s.ims", tid);

    // ...

    int cod_operacion;
    void *datos_recibidos = recibir_paquete_cCOP(client, &cod_operacion);

    while (!tareas_finalizadas)
    {
        switch (cod_operacion)
        {
        case MOVER_TRIPULANTE:
            // TODO: Actualizar posicion del tripulante
            registrarDesplazamiento();
            break;

        case INICIO_TAREA:
            registrarInicioTarea();
            break;

        case INICIO_RESOLUCION_SABOTAJE:
            registrarAtencionSabotaje();
            break;

        case INICIO_PROTOCOLO_FSCK:
            ejecutar_fsck();
            break;

        case FIN_RESOLUCION_SABOTAJE:
            registrarResolucionSabotaje();
            break;

        case EJECUTAR_TAREA:;
            t_ejecutar_tarea *tarea_a_ejecutar = (t_ejecutar_tarea *)datos_recibidos;
            ejecutarTarea(tarea_a_ejecutar);
            break;

        default:
            log_error(logger, "Código %d desconocido. El tripulante %d se desconectó", cod_operacion, tripulante->TID);
            tareas_finalizadas = true;
            break;
        }

        datos_recibidos = recibir_paquete_cCOP(client, &cod_operacion);
    }

    // TODO: Remover
    // sync_blocks();

    free(bitacora_file_path);
    free(tid);
    free(tripulante);
    liberar_bitacora(bitacora);

    // TODO: Cerrar socket
    // ...

    return;
}
