#include "proceso1.h"

void sabotajes()
{
    t_admin_tripulantes *aux_admin;
    t_admin_tripulantes *tripulante_elegido;
    t_posicion *unSabotaje;
    int index;
    int distancia = -1;
    int bEnviar;
    void *d_enviar;

    while(escuchandoSabotajes){

        //RECIBO EL SABOTAJE
        unSabotaje = (t_posicion *) recibir_paquete(sockfd_mongo);

        //PAUSO LA PLANIFICACION
        pausar(0);

        //DOY EL AVISO CORRESPONDIENTE DE COMIENZO
        printf(">>>>>>>>>>>>>>>>>>>>>\nATENDIENDO UN SABOTAJE\n<<<<<<<<<<<<<<<<<<<<<<\n\n");
        log_info(logger, "ATENDIENDO UN SABOTAJE");

        //MUEVO LOS TRIPULANTES A LA LISTA DE BLOQ
        for (int i = 0; i < list_size(exec); i++)
        {
            index = menor_tid_list(exec);
            aux_admin = list_remove(exec, index);
            list_add(bloq, aux_admin);
        }
        
        for (int i = 0; i < list_size(ready); i++)
        {
            index = menor_tid_list(ready);
            aux_admin = list_remove(ready, index);
            list_add(bloq, aux_admin);
        }

        //SELECCIONO ELTRIPULANTE QUE VA A REALIZAR EL SABOTAJE
        for (int i = 0; i < list_size(bloq); i++)
        {
            aux_admin = list_get(bloq, i);

            //LE AVISO A RAM PARA QUE CAMBIE EL ESTADO
            d_enviar = serializar_ActulizacionEstado(aux_admin->pid, aux_admin->tid, BLOCKED_EMERGENCY, &bEnviar);
            send(aux_admin->sockfd_tripulante_ram, d_enviar, bEnviar, 0);
            free(d_enviar);

            //CHEQUEO LA DISTANCIA DE CADA TRIPULANTE CON EL SABOTAJE
            if(distancia == -1 || distancia_posiciones(unSabotaje, aux_admin->posX, aux_admin->posY) < distancia)
                index = i;
        }

        tripulante_elegido = list_get(bloq, index);

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
        send(sockfd_mongo, d_enviar, bEnviar, 0);
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
            aux_admin = list_get(bloq, i);

            //LE AVISO A RAM PARA QUE CAMBIE EL ESTADO
            d_enviar = serializar_ActulizacionEstado(aux_admin->pid, aux_admin->tid, aux_admin->estado, &bEnviar);
            send(aux_admin->sockfd_tripulante_ram, d_enviar, bEnviar, 0);
            free(d_enviar);

            //PONGO LOS TRIPULANTES EN LA COLA QUE PERTENECEN
            switch (aux_admin->estado)
            {
            case READY:
                eliminarTripulante(bloq, aux_admin->tid);
                list_add(ready, aux_admin);
                break;
            
            case EXEC:
                eliminarTripulante(bloq, aux_admin->tid);
                list_add(exec, aux_admin);
                break;
            }
        }

        //RETOMO LA PLANIFICACION
        planificando = 1;
        pausar(1);

        //DOY EL AVISO CORRESPONDIENTE DE FIN
        printf("\n\n--------------------------\nFIN RESOLUCION DE SABOTAJE\n--------------------------\n\n");
        log_info(logger, "FIN RESOLUCION DE SABOTAJE");

    }

    return;
}

void resolver_sabotaje(){
    sleep(config->duracion_sabotaje);
    return;
}

int distancia_posiciones(t_posicion *pSabotaje, int posX, int posY){
    return fabs(pSabotaje->posX - posX) + fabs(pSabotaje->posY - posY);
}