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
            log_info(logger, "Comenzando una patota");
            comenzar_patota(client, (t_tareas_cPID *)datos_recibidos);
            break;
        case INICIAR_TRIPULANTE:
            log_info(logger, "Iniciando un tripulante");
            iniciar_tripulante(datos_recibidos);
            break;
        case SOLICITAR_TAREA:
            log_info(logger, "Tarea solicitada");
            solicitar_tarea(client, datos_recibidos);
            break;
        default:
            log_info(logger, "Llego un codigo de operacion desconocido: %d", cop_recibido);
            break;
        }
        
        free(datos_recibidos);
    }
}