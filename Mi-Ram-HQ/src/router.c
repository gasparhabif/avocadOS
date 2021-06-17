#include"proceso3.h"

void comenzar_patota(int client, t_tareas_cPID *tareas_cPID_recibidas)
{
    printf("\n-----UNA NUEVA PATOTA ABORDA LA NAVE-----\n");
    printf("INICIO DE PATOTA CON PID: %d\n", tareas_cPID_recibidas->PID);
    printf("CANT TAREAS: %d\n", tareas_cPID_recibidas->cantTareas);

    for (int i = 0; i < tareas_cPID_recibidas->cantTareas; i++)
    {
        printf("TAREA: %d\n",  i + 1);
        printf("CODT: %d\n",   tareas_cPID_recibidas->tareas[i].codigoTarea);
        printf("PARA: %d\n",   tareas_cPID_recibidas->tareas[i].parametro);
        printf("POSX: %d\n",   tareas_cPID_recibidas->tareas[i].posX);
        printf("POSY: %d\n",   tareas_cPID_recibidas->tareas[i].posY);
        printf("DURA: %d\n\n", tareas_cPID_recibidas->tareas[i].duracionTarea);
    }
    printf("-----------------------------------------\n\n\n");

    //RESERVO MEMORIA PARA LAS TAREAS Y PCB
    void *pos_tareas  = reservar_memoria(tareas_cPID_recibidas->cantTareas * sizeof(t_tarea));
    void *pcb         = reservar_memoria(sizeof(t_PCB));
    //COPIO LAS TAREAS EN LA POSICION RESERVADA
    memcpy(pos_tareas, tareas_cPID_recibidas->tareas, tareas_cPID_recibidas->cantTareas * sizeof(t_tarea));

    //CREO EL PCB
    t_PCB *nuevo_pcb  = malloc(t_PCB);
    //CARGO EL PCB
    nuevo_pcb->pid    = tareas_cPID_recibidas -> pid;
    nuevo_pcb->tareas = pos_tareas;
    //COPIO EL PCB EN LA POSICION RESERVADA
    memcpy(pcb, nuevo_pcb, sizeof(t_TCB));
    //DEVUELVO MEMORIA
    free(nuevo_pcb);

    //CREO EL REGISTRO DE SEGMENTOS DEL PROCESO
    t_list* registro_segmentos = list_create();
    //CREO LOS DOS REGISTROS QUE SE VAN A AÑADIR A LA TABLA
    t_registro_segmentos segmento_pcb, segmento_tareas;
    //LLENO EL SEGMENTO DEL PCB
    segmento_pcb->segmento    = 1;
    segmento_pcb->base        = pos_tareas;
    segmento_pcb->tamanio     = tareas_cPID_recibidas->cantTareas * sizeof(t_tarea);
    segmento_pcb->tipo        = PCB;
    //LLENO EL SEGMENTO DE LAS TAREAS
    segmento_tareas->segmento = 2;
    segmento_tareas->base     = pos_tareas;
    segmento_tareas->tamanio  = sizeof(t_PCB);
    segmento_tareas->tipo     = TAREAS;
    //AGREGO LOS SEGMENTOS A LA LISTA
    list_add(registro_segmentos, segmento_pcb);
    list_add(registro_segmentos, segmento_tareas);

    //CREO LA NUEVA FILA DE LA LISTA DE PROCESOS
    t_registro_procesos *nuevo_proceso;
    //LLENO LA FILA DE LA LISTA DE PROCESOS
    nuevo_proceso->pid       = tareas_cPID_recibidas->pid;
    nuevo_proceso->registros = t_registro_segmentos;
    //nuevo_proceso->indice    =
    //AÑADO LA FILA A LA LISTA DE PROCESOS
    list_add(tabla_procesos, nuevo_proceso);

    //ENVIAR A DISCORDIADOR EL PUNTERO AL PCB
    printf("Enviando datos...\n");
    int tamanioSerializacion;
    void *paquete = serializarInt((int) pcb, PUNTERO_PCB, &tamanioSerializacion);
    printf("Se enviaron %d bytes\n\n", send(client, paquete, tamanioSerializacion, 0));
    free(paquete);

    free(tareas_cPID_recibidas);
}

void iniciar_tripulante(t_TCB* tcb_recibido){

    printf("\n-----UN TRIPULANTE ABORDO LA NAVE-----\n");
    printf("TID: %d\n", tcb_recibido->TID);
    printf("EST: %c\n", tcb_recibido->estado);
    printf("P_X: %d\n", tcb_recibido->posX);
    printf("P_Y: %d\n", tcb_recibido->posY);
    printf("PRI: %d\n", tcb_recibido->proximaInstruccion);
    printf("PCB: %d\n", tcb_recibido->puntero_PCB);
    printf("--------------------------------------\n\n\n");

    void *tcb = reservar_memoria(sizeof(t_TCB));
    memcpy(tcb, tcb_recibido, sizeof(t_TCB));
}

void solicitar_tarea(int client, int* datos_recibidos){
   printf("El tripulante %d me ha solicitado una tarea", *datos_recibidos);

   t_tarea *tareaDePrueba = malloc(sizeof(t_tarea));
   tareaDePrueba->codigoTarea=MOVER_POSICION;
   tareaDePrueba->parametro=3;
   tareaDePrueba->posX=1;
   tareaDePrueba->posY=4;
   tareaDePrueba->duracionTarea=5;

   int bEnviar;
   void *d_enviar = serializarTarea(tareaDePrueba, &bEnviar);
   send(client, d_enviar, bEnviar, 0);
   free(d_enviar);
}