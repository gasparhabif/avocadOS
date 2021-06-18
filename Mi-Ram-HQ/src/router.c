#include "proceso3.h"

void comenzar_patota(int client, t_tareas_cPID *tareas_cPID_recibidas)
{
    printf("\n-----UNA NUEVA PATOTA ABORDA LA NAVE-----\n");
    printf("INICIO DE PATOTA CON PID: %d\n", tareas_cPID_recibidas->PID);
    printf("CANT TAREAS: %d\n", tareas_cPID_recibidas->cantTareas);

    for (int i = 0; i < tareas_cPID_recibidas->cantTareas; i++)
    {
        printf("TAREA: %d\n", i + 1);
        printf("CODT: %d\n", tareas_cPID_recibidas->tareas[i].codigoTarea);
        printf("PARA: %d\n", tareas_cPID_recibidas->tareas[i].parametro);
        printf("POSX: %d\n", tareas_cPID_recibidas->tareas[i].posX);
        printf("POSY: %d\n", tareas_cPID_recibidas->tareas[i].posY);
        printf("DURA: %d\n\n", tareas_cPID_recibidas->tareas[i].duracionTarea);
    }
    printf("-----------------------------------------\n\n\n");

    //GUARDAR TAREAS EN MEMORIA
    void *tareas_recibidas = malloc(tareas_cPID_recibidas->cantTareas * sizeof(t_tarea));

    //CREAR PCB
    t_PCB *pcb = malloc(sizeof(t_PCB));

    //LLENAR PCB
    pcb->PID = tareas_cPID_recibidas->PID;
    pcb->tareas = (int)tareas_recibidas;

    //ENVIAR A DISCORDIADOR EL PUNTERO AL PCB
    printf("Enviando datos...\n");
    int tamanioSerializacion;
    void *paquete = serializarInt((int)tareas_recibidas, PUNTERO_PCB, &tamanioSerializacion);
    printf("Se enviaron %d bytes\n\n", send(client, paquete, tamanioSerializacion, 0));
    free(paquete);
}

void iniciar_tripulante(t_TCB *tcb_recibido)
{

    printf("\n-----UN TRIPULANTE ABORDO LA NAVE-----\n");
    printf("TID: %d\n", tcb_recibido->TID);
    printf("EST: %c\n", tcb_recibido->estado);
    printf("P_X: %d\n", tcb_recibido->posX);
    printf("P_Y: %d\n", tcb_recibido->posY);
    printf("PRI: %d\n", tcb_recibido->proximaInstruccion);
    printf("PCB: %d\n", tcb_recibido->puntero_PCB);
    printf("--------------------------------------\n\n\n");
}

void solicitar_tarea(int client, int *datos_recibidos)
{
    printf("El tripulante %d me ha solicitado una tarea", *datos_recibidos);

    t_tarea *tareaDePrueba = malloc(sizeof(t_tarea));
    tareaDePrueba->codigoTarea = MOVER_POSICION;
    tareaDePrueba->parametro = 3;
    tareaDePrueba->posX = 1;
    tareaDePrueba->posY = 4;
    tareaDePrueba->duracionTarea = 5;

    int *bEnviar;
    void *d_enviar = serializarTarea(tareaDePrueba, &bEnviar);
    send(client, d_enviar, bEnviar, 0);
    free(d_enviar);
}