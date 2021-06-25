#include "proceso1.h"

void sabotajes()
{

    while(escuchandoSabotajes){

        //RECIBO EL SABOTAJE
        t_posicion *unSabotaje = (t_posicion *) recibir_paquete(sockfd_mongo);

        //PAUSO LA PLANIFICACION
        planificando = 0;
        pausar();

        //MUEVO LOS TRIPULANTES A LA LISTA DE BLOQ
        int index;
        t_admin_tripulantes *aux_admin;

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
        int distancia;

        for (int i = 0; i < list_size(bloq); i++)
        {
            
        }
        

        //MUEVO EL TRIPULANTE A LA POSICION DEL SABOTAJE


        //
    }
}