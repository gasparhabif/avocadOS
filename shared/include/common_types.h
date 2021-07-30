#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <stdint.h>

enum CODIGOS_DE_OPERACION
{
    COMENZAR_PATOTA = 1,
    PUNTERO_PCB,
    INICIAR_TRIPULANTE,
    SOLICITAR_TAREA,
    ENVIAR_PROXIMA_TAREA,
    REALIZAR_TAREA,
    MOVER_TRIPULANTE,
    ACTUALIZAR_ESTADO,
    ELIMINAR_TRIPULANTE,
    ESTRUCTURAS_EN_MEMORIA,
    SOLICITAR_LISTA,
    LISTA_TRIPULANTES,
    ALERTA_SABOTAJE,
    SOLICITAR_BITACORA,
    BITACORA_TRIPULANTE,
    INICIO_PROTOCOLO_FSCK,
    IMPRIMIR_SEGMENTACION,
    INICIO_TRIPULANTE_MONGO
};

enum CODIGOS_DE_TAREAS
{
    FIN_TAREAS = 0,
    GENERAR_OXIGENO,
    CONSUMIR_OXIGENO,
    GENERAR_COMIDA,
    CONSUMIR_COMIDA,
    GENERAR_BASURA,
    DESCARTAR_BASURA,
    MOVER_POSICION
};

enum CODIGOS_DE_BITACORAS
{
    INICIO_TAREA = 'I',   //Notificacion de inicio para la bitacora
    EJECUTAR_TAREA = 'E', //Notificacion para ejecutar la tarea (no se manda para tareas normales)
    FIN_TAREA = 'F',
    INICIO_RESOLUCION_SABOTAJE = 'S',
    FIN_RESOLUCION_SABOTAJE = 'A'
};

//ESTADOS
#define NEW 'N'
#define READY 'R'
#define EXEC 'E'
#define BLOCKED_IO 'B'
#define BLOCKED_EMERGENCY 'Y'
#define EXIT 'X'

//BUFFERS Y PAQUETE
typedef struct
{
    uint32_t size;
    void *stream;
} t_buffer;

typedef struct
{
    uint8_t codigo_operacion;
    t_buffer *buffer;
} t_paquete;

//TCB Y PCB
typedef struct
{
    uint32_t PID;
    uint32_t tareas;
} t_PCB;

typedef struct
{
    uint32_t TID;
    char estado;
    uint32_t posX;
    uint32_t posY;
    uint32_t proximaInstruccion;
    uint32_t puntero_PCB;
} t_TCB;

typedef struct
{
    int pid;
    t_TCB tcb;
} t_TCBcPID;

typedef struct
{
    uint32_t pid;
    uint32_t tid;
} t_pidYtid;

typedef struct
{
    uint32_t PID;
    uint32_t TID;
    char estado;
    uint32_t posX;
    uint32_t posY;
    uint32_t proximaInstruccion;
} t_TCBmostrar;

typedef struct
{
    uint32_t cantTripulantes;
    t_TCBmostrar *tripulantes;
} t_ListaTripulantes;

typedef struct
{
    uint32_t posX;
    uint32_t posY;
} t_posicion;

typedef struct
{
    uint32_t TID;
    t_posicion pos;
} t_tidCposicion;

//BASICOS
typedef struct
{
    uint32_t PID;
    uint32_t TID;
    char estado;
} t_estado;

typedef struct
{
    uint32_t PID;
    uint32_t TID;
    t_posicion pos;
} t_envio_posicion;

//TAREAS
typedef struct
{
    uint32_t tamanio_tarea;
    char *tarea;
} t_tarea;

typedef struct
{
    uint8_t codigoTarea;
    uint32_t parametro;
    uint32_t posX;
    uint32_t posY;
    uint32_t duracionTarea;
} t_tarea_descomprimida;

typedef struct
{
    uint32_t PID;
    uint32_t cantTareas;
    t_tarea *tareas;
} t_tareas_cPID;

typedef struct
{
    uint32_t tid;
    uint32_t len_tarea;
    char *tarea;
} t_inicio_tarea;

typedef struct
{
    uint8_t codigoTarea;
    uint32_t parametro;
} t_ejecutar_tarea;

//ACTUALIZACIONES
typedef struct
{
    uint32_t TID;
    uint32_t posX;
    uint32_t posY;
} t_actualizar_posicion;

typedef struct
{
    uint32_t TID;
    char estado;
} t_actualizar_estado;

//ENVIO DE BITACORAS
typedef struct
{
    uint32_t tamanioAccion;
    char *accion;
} t_accion;

typedef struct
{
    uint32_t cantAcciones;
    uint32_t tamanioAcciones;
    t_accion *acciones;
} t_bitacora;

#endif