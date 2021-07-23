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
    char *tid = string_itoa(tripulante->TID);
    t_posicion *current_pos = malloc(sizeof(t_posicion));
    current_pos->posX = tripulante->pos.posX;
    current_pos->posY = tripulante->pos.posY;

    log_info(logger, "Se conectó el tripulante TID: %s - Posición inicial: %s", tid, pos_to_string(current_pos));

    // Obtener path
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

    // printf("Bitacora reconstruida: %s\n", reconstruir_bitacora(bitacora));

    // while (1)
    // {
    // }

    int cod_operacion;
    void *datos_recibidos = recibir_paquete_cCOP(client, &cod_operacion);

    while (!tareas_finalizadas)
    {
        sync_blocks();

        switch (cod_operacion)
        {
        case MOVER_TRIPULANTE:;
            t_envio_posicion *datos_posicion = (t_envio_posicion *)datos_recibidos;
            t_posicion *next_pos = malloc(sizeof(t_posicion));
            next_pos->posX = datos_posicion->pos.posX;
            next_pos->posY = datos_posicion->pos.posY;
            registrar_bitacora(bitacora, string_from_format("Se mueve de %s a %s\0", pos_to_string(current_pos), pos_to_string(next_pos)));
            update_pos(current_pos, next_pos);
            free(next_pos);
            free(datos_posicion);
            break;

        case INICIO_TAREA:
            registrar_bitacora(bitacora, string_from_format("Comienza ejecución de tarea %s\0", get_nombre_tarea((int)datos_recibidos)));
            break;

        case INICIO_RESOLUCION_SABOTAJE:
            registrar_bitacora(bitacora, "Se corre en pánico hacia la ubicación del sabotaje\0");
            break;

        case INICIO_PROTOCOLO_FSCK:
            ejecutar_fsck();
            break;

        case FIN_RESOLUCION_SABOTAJE:
            registrar_bitacora(bitacora, "Se resuelve el sabotaje\0");
            break;

        case EJECUTAR_TAREA:;
            t_ejecutar_tarea *tarea_a_ejecutar = (t_ejecutar_tarea *)datos_recibidos;
            ejecutarTarea(tarea_a_ejecutar);
            registrar_bitacora(bitacora, string_from_format("Se finaliza la tarea %s\0", get_nombre_tarea(tarea_a_ejecutar->codigoTarea)));
            free(tarea_a_ejecutar);
            break;

        default:
            log_error(logger, "Código %d desconocido. El tripulante %d se desconectó", cod_operacion, tripulante->TID);
            tareas_finalizadas = true;
            break;
        }

        datos_recibidos = recibir_paquete_cCOP(client, &cod_operacion);
    }

    free(bitacora_file_path);
    free(tid);
    free(tripulante);
    liberar_bitacora(bitacora);

    // TODO: Cerrar socket
    // ...

    return;
}
