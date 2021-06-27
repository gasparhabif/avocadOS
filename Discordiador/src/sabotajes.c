#include "proceso1.h"

void sabotajes()
{
    
    t_admin_tripulantes *aux_admin;
    t_admin_tripulantes *tripulante_elegido;
    t_posicion *unSabotaje;
    int index;
    int distancia = -1;

    while(escuchandoSabotajes){

        //RECIBO EL SABOTAJE
        unSabotaje = (t_posicion *) recibir_paquete(sockfd_mongo);

        //PAUSO LA PLANIFICACION
        planificando = 0;
        pausar();

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
            if(distancia == -1 || distancia_posiciones(unSabotaje, aux_admin->posX, aux_admin->posY) < distancia)
                index = i;
        }

        tripulante_elegido = list_get(bloq, index);

        //MUEVO EL TRIPULANTE A LA POSICION DEL SABOTAJE
        //mover_tripulante(tripulante_elegido, unSabotaje->posX, unSabotaje->posY, ))
        
        //INVOCO AL FSCK Y REGISTRO EN BITACORA
        int tamanioSerializacion;
        void *dEnviar = serializarInt(tripulante_elegido->tid, INICIO_RESOLUCION_SABOTAJE, &tamanioSerializacion);
        send(tripulante_elegido->sockfd_tripulante_mongo, dEnviar, tamanioSerializacion, 0);
        free(dEnviar);

        //RESUELVO EL SABOTAJE 
        resolver_sabotaje();

        //DESBLOQUEO TRIPULANTES

    }
}

void resolver_sabotaje(){
    sleep(config->duracion_sabotaje);
    return;
}

int distancia_posiciones(t_posicion *pSabotaje, int posX, int posY){
    return fabs(pSabotaje->posX - posX) + fabs(pSabotaje->posY - posY);
}