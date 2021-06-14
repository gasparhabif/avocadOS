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

    log_info(logger, "Se conectó un tripulante en el socket %d", client);

    // Recibir ID del tripulante
    // ...

    // Generar t_bitacora para el tripulante
    // ...

    int *cod_operacion;
    void *datos_recibidos = recibir_paquete_cCOP(client, &cod_operacion);

    while (!tareas_finalizadas)
    {
        log_info(logger, "Se recibió el código de operación %d", cod_operacion);

        // Verificar si es tarea de ES
        if (cod_operacion == REALIZAR_TAREA)
        {
            t_tarea *tarea_a_ejecutar = (t_tarea *)datos_recibidos;

            switch (tarea_a_ejecutar->codigoTarea)
            {
            case FIN_TAREAS:
                log_info(logger, "El tripulante del socket %d finalizó sus tareas", client);
                tareas_finalizadas = true;
                break;
            case GENERAR_OXIGENO:
                generarOxigeno(tarea_a_ejecutar->parametro);
                break;
            case CONSUMIR_OXIGENO:
                consumirOxigeno(tarea_a_ejecutar->parametro);
                break;
            case GENERAR_COMIDA:
                generarComida(tarea_a_ejecutar->parametro);
                break;
            case CONSUMIR_COMIDA:
                consumirComida(tarea_a_ejecutar->parametro);
                break;
            case GENERAR_BASURA:
                generarBasura(tarea_a_ejecutar->parametro);
                break;
            case DESCARTAR_BASURA:
                descartarBasura();
                break;
            default:
                log_error(logger, "Código de tarea desconocido");
                break;
            }
        }
        // Sino, registrar "algo" en bitácora
        else
        {
            switch (*cod_operacion)
            {
            case MOVER_POSICION:
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
            default:
                log_error(logger, "Código de operacion desconocido: %d", *cod_operacion);
                tareas_finalizadas = true;
                break;
            }
        }

        datos_recibidos = recibir_paquete_cCOP(client, &cod_operacion);
    }

    log_error(logger, "El tripulante del socket %d se desconectó", client);
}