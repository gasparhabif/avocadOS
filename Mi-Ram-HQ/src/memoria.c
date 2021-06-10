#include"proceso3.h"

enum TIPO {
    PCB = 0,
    TCB,
    TAREAS,
    ...
}

typedef struct {
    u_int32_t pid;
    t_list* registros;
    u_int32_t indice;
} registro_proceso;

typedef struct {
    u_int32_t segmento; // Identificador (numero) del segmento
    u_int32_t base; // Posicion inicio del segmento
    u_int32_t tamanio; // Tamaño del contenido del segmento
    TIPO tipo; // Tipo del contenido del segmento
    u_int32_t tid = -1; // Opcional
    u_int8_t usado = 0; // Booleano para saber si el segmento puede sacarse
} registro_datos_proceso;

t_list* tabla_procesos; // Se crea con list_create

void iniciar_patota(int client, t_tareas_cPID *tareas_cPID_recibidas)
{
    //GUARDAR TAREAS EN MEMORIA
    void* tareas_recibidas = malloc(tareas_cPID_recibidas->cantTareas * sizeof(t_tarea));

    //CREAR PCB
    t_PCB *pcb = malloc(sizeof(t_PCB));

    //LLENAR PCB
    pcb->PID    = tareas_cPID_recibidas->PID;
    pcb->tareas = (int) tareas_recibidas;

    // Creamos un nuevo registro de proceso
    // Asignamos sus datos
    // Agregamos a la lista de procesos
    registro_proceso nuevo;
    nuevo.pid = pcb->PID;
    nuevo->registros = list_create();
    list_add(tabla_procesos, nuevo);

    // Creamos un nuevo registros de datos del proceso
    // Asignamos sus datos
    // Agregamos a la lista de datos del proceso
    registro_datos_proceso nuevo_datos;
    nuevo_datos.segmento = nuevo.indice++; // Asigna y dsps aumenta
    nuevo_datos.base = pcb;
    nuevo_datos.tamanio = sizeof(pcb);
    nuevo_datos.tipo = PCB;
    nuevo_datos.usado = 0;
    list_add(nuevo->registros, nuevo_datos);
    
    // TODO agregar registro de tareas

    //ENVIAR A DISCORDIADOR EL PUNTERO AL PCB
    printf("Enviando datos...\n");
    int tamanioSerializacion;
    void *paquete = serializarInt((int) tareas_recibidas, PUNTERO_PCB, &tamanioSerializacion);
    printf("Se enviaron %d bytes\n\n", send(client, paquete, tamanioSerializacion, 0));
    free(paquete);
}

void iniciar_tripulante(t_TCB* tcb_recibido)
{
    bool buscar_proceso(registro_proceso* proceso) {
        t_PCB* pcb = tcb_recibido->puntero_PCB;
        return proceso->pid == pcb->PID;
    }

    // Buscamos al proceso que es el creador del hilo
    registro_proceso* proceso = list_find(tabla_procesos, (*buscar_proceso));

    // Creamos un nuevo registros de datos del proceso
    // Asignamos sus datos
    // Agregamos a la lista de datos del proceso
    registro_datos_proceso nuevo_datos;
    nuevo_datos.segmento = nuevo.indice++; // Asigna y dsps aumenta
    nuevo_datos.base = tcb_recibido;
    nuevo_datos.tamanio = sizeof(tcb_recibido);
    nuevo_datos.tipo = TCB;
    nuevo_datos.usado = 0;
    list_add(proceso->registros, nuevo_datos);
}

void listarTripulantes () {
    void mostrar_tripulantes_por_patota(registro_proceso* registro)
    {
        void mostrar_tripulante(registro_datos_proceso* registro)
        {
            printf("TID: %d\nEstado: %c", registro->tid, registro->estado);
        }

        bool esTcb (registro_datos_proceso* data) {
            return registro_datos_proceso->tipo == TCB;
        }

        printf("PID: %d", registro->pid);
        // Filtramos de la sublista los registros que solo sean tcbs
        t_list* tcbs = list_filter(registro->registros, (*esTcb));
        // Recorremos la lista filtrada mostrando los datos del tripulante
        list_iterate(tcbs, (*mostrar_tripulante))
    }

    // Recorremos la lista y aplicamos la funcion a cada valor de cada pos
    list_iterate(tabla_procesos, (*mostrar_tripulantes_por_patota))
}

