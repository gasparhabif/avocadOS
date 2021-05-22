#include"proceso3.h"

void comenzar_patota(int client, t_tareas_cPID *tareas_cPID_recibidas)
{
    printf("INICIO DE PATOTA CON PID: %d\n", tareas_cPID_recibidas->PID);
    printf("CANT TAREAS: %d\n", tareas_cPID_recibidas->cantTareas);

    for (int i = 0; i < tareas_cPID_recibidas->cantTareas; i++)
    {
        printf("-----TAREA %d-----\n", i + 1);
        printf("CODT: %d\n", tareas_cPID_recibidas->tareas[i].codigoTarea);
        printf("PARA: %d\n", tareas_cPID_recibidas->tareas[i].parametro);
        printf("POSX: %d\n", tareas_cPID_recibidas->tareas[i].posX);
        printf("POSY: %d\n", tareas_cPID_recibidas->tareas[i].posY);
        printf("DURA: %d\n", tareas_cPID_recibidas->tareas[i].duracionTarea);
        printf("------------------\n\n");
    }

    //CREAR PCB, GUARDAR PID Y TAREAS EN MEMORIA, ENVIAR A DISCORDIADOR EL PUNTERO AL PCB
    printf("Enviando datos...\n");
    int tamanioSerializacion;
    void *paquete = serializarInt(11111, PUNTERO_PCB, &tamanioSerializacion);
    printf("Se enviaron %d bytes\n", send(client, paquete, tamanioSerializacion, 0));
    free(paquete);
}