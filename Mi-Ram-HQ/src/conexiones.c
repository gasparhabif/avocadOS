#include "proceso3.h"

void aceptar_conexiones(void *parametro)
{

    pthread_t recibir_mensajes_thread;
    int client;
    int server_instance = (int)parametro;
    struct sockaddr_in client_dir;
    unsigned int dir_size = sizeof(socklen_t);

    while (aceptando_conexiones)
    {
        listen(server_instance, 100);

        if ((client = accept(server_instance, (void *)&client_dir, &dir_size)) == -1)
        {
            log_error(logger, "Algo malió sal al aceptar al cliente");
            return;
        }

        log_info(logger, "Recibí una conexión en el socket %d", client);

        //CREO EL HILO QUE ESCUCHA CONEXIONES
        pthread_create(&recibir_mensajes_thread, NULL, (void *)recibir_mensaje, (void *)client);
    }
}

void recibir_mensaje(void *parametro)
{

    int client = (int)parametro;
    int cop_recibido;
    void *datos_recibidos;

    while (recibiendo_mensajes)
    {

        datos_recibidos = recibir_paquete_cCOP(client, &cop_recibido);

        switch (cop_recibido)
        {
            case COMENZAR_PATOTA:
                log_info(logger, "El discordiador %d comenzo una patota", client);
                comenzar_patota(client, (t_tareas_cPID *)datos_recibidos);
                break;
            case INICIAR_TRIPULANTE:
                log_info(logger, "El tripulante %d solicita el ingreso a la nave", client);
                iniciar_tripulante(client, datos_recibidos);
                break;
            case SOLICITAR_TAREA:
                log_info(logger, "El tripulante %d solicito una tarea", client);
                solicitar_tarea(client, datos_recibidos);
                break;
            case MOVER_TRIPULANTE:
                log_info(logger, "El tripulante %d ha realizado un movimiento", client);
                break;
            case ACTUALIZAR_ESTADO:
                log_info(logger, "El tripulante %d actualizo su estado", client);
                break;
            case ELIMINAR_TRIPULANTE:
                log_info(logger, "El tripulante %d quiere abandonar la nave", client);
                //¿LO MANDA EL DISCORDIADOR O EL TRIPULANTE QUE QUIERE MORIR?
                break;
            default:
                log_info(logger, "Llego un codigo de operacion desconocido: %d", cop_recibido);
                break;
        }
    }

    return;
}