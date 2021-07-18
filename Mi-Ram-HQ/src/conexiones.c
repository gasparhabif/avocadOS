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
        pthread_detach(recibir_mensajes_thread);
    }
}

void recibir_mensaje(void *parametro)
{

    int client = (int)parametro;
    int cop_recibido;
    void *datos_recibidos;
    int recibiendo_mensajes = 1;

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
                mover_tripulante(datos_recibidos);
                break;
            case ACTUALIZAR_ESTADO:
                log_info(logger, "El tripulante %d actualizo su estado", client);
                actualizar_estado(datos_recibidos);
                break;
            case SOLICITAR_LISTA:
                log_info(logger, "El discordiador %d solicito un listdo de tripulantes", client);
                solicitar_tripulantes(client);
                //free(datos_recibidos);
                break;
            case IMPRIMIR_SEGMENTACION:
                pthread_mutex_lock(&acceso_memoria);
                printf("------------------------------------------------------------------\n");
                for (int i = 0; i < list_size(tabla_estado_segmentos); i++)
                {
                    estado_segmentos *reg_seg = list_get(tabla_estado_segmentos, i);
                    printf("SEG N°: %d\t", i);
                    printf("Inicio: %d\t", reg_seg->inicio);
                    printf("Tamaño: %d\t", reg_seg->limite);
                    printf("Ocupado: %d\n", reg_seg->ocupado);
                }
                printf("------------------------------------------------------------------\n");
                pthread_mutex_unlock(&acceso_memoria);
                break;
            case ELIMINAR_TRIPULANTE:
                log_info(logger, "El tripulante %d quiere abandonar la nave", client);
                eliminar_tripulante(datos_recibidos);
                recibiendo_mensajes = 0;
                break;
            default:
                log_info(logger, "Llego un codigo de operacion desconocido: %d al cliente %d", cop_recibido, client);
                break;
        }
    }

    pthread_exit(0);
}