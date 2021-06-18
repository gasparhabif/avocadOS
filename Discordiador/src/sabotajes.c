<<<<<<< HEAD
#include "proceso1.h"

void sabotajes()
{

    /*
    while(escuchandoSabotajes){

        t_posicion *unSabotaje = (t_posicion *) recibir_paquete(sockfd_mongo);

      BLOCKEO A TODOS LOS TRIPULANTES
        for (int i = 0; i < cantTripulantes; i++)
        {
            pthread_kill(listaTripulantes[i], SIGUSR2);
        }

        //SI HAY UN SABOTAJE
        sabotaje = 1;
    }

*/
=======
#include"proceso1.h"

void sabotajes(){

    while(escuchandoSabotajes){

        //t_posicion *unSabotaje = (t_posicion *) recibir_paquete(sockfd_mongo);

/*      BLOCKEO A TODOS LOS TRIPULANTES
        for (int i = 0; i < cantTripulantes; i++)
        {
            pthread_kill(listaTripulantes[i], SIGUSR2);
        }
*/
        //SI HAY UN SABOTAJE
        sabotaje = 1;
    }
>>>>>>> mi-ram-hq
}