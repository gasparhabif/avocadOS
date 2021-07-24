#include "proceso1.h"

void sabotajes()
{
    t_admin_tripulantes *aux_admin;
    t_admin_tripulantes *tripulante_elegido;
    t_posicion *unSabotaje;
    int index;
    int distancia;
    int bEnviar;
    void *d_enviar;

    while (escuchandoSabotajes)
    {
        distancia = -1;

        //RECIBO EL SABOTAJE
        unSabotaje = (t_posicion *)recibir_paquete(sockfd_mongo_sabotajes);

        //printf("Pos del sabotaje en %d|%d", unSabotaje->posX, unSabotaje->posY);

        //PAUSO LA PLANIFICACION
        pausar(1);
        sabotaje = 1;

        //DOY EL AVISO CORRESPONDIENTE DE COMIENZO
        printf(">>>>>>>>>>>>>>>>>>>>>\nATENDIENDO UN SABOTAJE\n<<<<<<<<<<<<<<<<<<<<<<\n\n");
        log_info(logger, "ATENDIENDO UN SABOTAJE");

        //MUEVO LOS TRIPULANTES A LA LISTA DE BLOQ
        // printf("Tripulantes en Exec: %d\n", list_size(exec));
        for (int i = 0; !list_is_empty(exec); i++)
        {
            index = menor_tid_list(exec);
            aux_admin = list_remove(exec, index);
            list_add(bloq, aux_admin);
        }

        // printf("Tripulantes en Ready: %d\n", list_size(ready));
        for (int i = 0; !list_is_empty(ready); i++)
        {
            index = menor_tid_list(ready);
            // printf("Index: %d\n", index);
            aux_admin = list_remove(ready, index);
            // printf("TID: %d\n", aux_admin->tid);
            list_add(bloq, aux_admin);
        }

        //SELECCIONO EL TRIPULANTE QUE VA A REALIZAR EL SABOTAJE
        for (int i = 0; i < list_size(bloq); i++)
        {
            aux_admin = list_get(bloq, i);

            //LE AVISO A RAM PARA QUE CAMBIE EL ESTADO
            d_enviar = serializar_ActulizacionEstado(aux_admin->pid, aux_admin->tid, BLOCKED_EMERGENCY, &bEnviar);
            send(aux_admin->sockfd_tripulante_ram, d_enviar, bEnviar, 0);
            free(d_enviar);

            //CHEQUEO LA DISTANCIA DE CADA TRIPULANTE CON EL SABOTAJE
            if (distancia == -1 || (abs(unSabotaje->posX - aux_admin->posX) + abs(unSabotaje->posY - aux_admin->posY)) < distancia)
            {
                index = i;
                distancia = (abs(unSabotaje->posX - aux_admin->posX) + abs(unSabotaje->posY - aux_admin->posY));
            }
        }

        tripulante_elegido = list_get(bloq, index);

        log_info(logger, "El tripulante %d resolverá el sabotaje\n", tripulante_elegido->tid);

        //REGISTRO EN BITACORA EL TRIPULANTE ELEGIDO PARA REALIZAR EL SABOTAJE
        d_enviar = serializarInt(tripulante_elegido->tid, INICIO_RESOLUCION_SABOTAJE, &bEnviar);
        send(tripulante_elegido->sockfd_tripulante_mongo, d_enviar, bEnviar, 0);
        free(d_enviar);

        //MUEVO EL TRIPULANTE A LA POSICION DEL SABOTAJE
        int movs = cantMovimientos(tripulante_elegido->posX, tripulante_elegido->posY, unSabotaje->posX, unSabotaje->posY);
        bEnviar = movs;
        mover_tripulante(tripulante_elegido, unSabotaje->posX, unSabotaje->posY, movs, &bEnviar);

        //INVOCO AL PROTOCOLO FSCK
        d_enviar = serializarInt(tripulante_elegido->tid, INICIO_PROTOCOLO_FSCK, &bEnviar);
        send(tripulante_elegido->sockfd_tripulante_mongo, d_enviar, bEnviar, 0);
        free(d_enviar);

        //RESUELVO EL SABOTAJE
        resolver_sabotaje();

        //AVISO DE FIN DE RESOLUCION DE SABOTAJE AL MONGO
        d_enviar = serializarInt(tripulante_elegido->tid, FIN_RESOLUCION_SABOTAJE, &bEnviar);
        send(tripulante_elegido->sockfd_tripulante_mongo, d_enviar, bEnviar, 0);
        free(d_enviar);

        //DESBLOQUEO TRIPULANTES
        for (int i = 0; i < list_size(bloq); i++)
        {
            // printf("i: %d\n", i);
            aux_admin = list_get(bloq, i);

            //LE AVISO A RAM PARA QUE CAMBIE EL ESTADO
            d_enviar = serializar_ActulizacionEstado(aux_admin->pid, aux_admin->tid, aux_admin->estado, &bEnviar);
            send(aux_admin->sockfd_tripulante_ram, d_enviar, bEnviar, 0);
            free(d_enviar);

            //PONGO LOS TRIPULANTES EN LA COLA QUE PERTENECEN
            switch (aux_admin->estado)
            {
            case READY:
                list_add(ready, aux_admin);
                break;

            case EXEC:
                list_add(exec, aux_admin);
                break;
            }
        }

        // Remover todos los TID de bloq
        while (!list_is_empty(bloq))
        {
            list_remove(bloq, 0);
        }

        //RETOMO LA PLANIFICACION
        sabotaje = 0;
        pausar(0);

        //DOY EL AVISO CORRESPONDIENTE DE FIN
        printf("\n\n--------------------------\nFIN RESOLUCION DE SABOTAJE\n--------------------------\n\n>");
        log_info(logger, "FIN RESOLUCION DE SABOTAJE");
    }

    return;
}

void resolver_sabotaje()
{
    sleep(config->duracion_sabotaje);
    return;
}

int distancia_posiciones(t_posicion *pSabotaje, int posX, int posY)
{
    int difX = fabs(pSabotaje->posX - posX);
    printf("Dif X: %d\n", difX);
    int difY = fabs(pSabotaje->posY - posY);
    printf("Dif Y: %d\n", difY);
    return difX + difY;
}

/*
int abs(int n)
{

    if (n < 0)
        return (-1) * n;
    else
        return n;
}
*/