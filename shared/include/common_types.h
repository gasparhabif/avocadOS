#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

enum CODIGOS_DE_OPERACION
{
    COMENZAR_PATOTA = 0,
    PUNTERO_PCB,
    INICIAR_TRIPULANTE,
    SOLICITAR_TAREA,
    ENVIAR_PROXIMA_TAREA,
    REALIZAR_TAREA,
    MOVER_TRIPULANTE,
    ACTUALIZAR_ESTADO,
    ELIMINAR_TRIPULANTE,
    ESTRUCTURAS_EN_MEMORIA,
    ALERTA_SABOTAJE,
    SOLICITAR_BITACORA,
    INICIO_PROTOCOLO_FSCK
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
    INICIO_TAREA               = 'I',
    FIN_TAREA                  = 'F',
    INICIO_RESOLUCION_SABOTAJE = 'S',
    FIN_RESOLUCION_SABOTAJE    = 'A'
};

//ESTADOS
#define NEW                      'N'
#define READY                    'R'
#define EXEC                     'E'
#define BLOCKED_IO               'B'
#define BLOCKED_EMERGENCY        'Y'
#define EXIT                     'X'

//BUFFERS Y PAQUETE
typedef struct
{
    u_int32_t size;
    void *stream;
} t_buffer;

typedef struct
{
    u_int8_t codigo_operacion;
    t_buffer *buffer;
} t_paquete;

//TCB Y PCB
typedef struct
{
    u_int32_t PID;
    u_int32_t tareas;
} t_PCB;

typedef struct
{
    u_int32_t TID;
    char estado;
    u_int32_t posX;
    u_int32_t posY;
    u_int32_t proximaInstruccion;
    u_int32_t puntero_PCB;
} t_TCB;

typedef struct
{
    int pid;
    t_TCB tcb;
} t_TCBcPID;

typedef struct
{
    u_int32_t pid;
    u_int32_t tid;
} t_pidYtid;


//BASICOS
typedef struct
{
    u_int32_t posX;
    u_int32_t posY;
} t_posicion;

typedef struct
{
    u_int32_t PID;
    u_int32_t TID;
    char estado;
} t_estado;

typedef struct
{
    u_int32_t PID;
    u_int32_t TID;
    t_posicion pos;
} t_envio_posicion;

//TAREAS
typedef struct
{
    u_int8_t  codigoTarea;
    u_int32_t parametro;
    u_int32_t posX;
    u_int32_t posY;
    u_int32_t duracionTarea;
} t_tarea;

typedef struct
{
    u_int32_t PID;
    u_int8_t cantTareas;
    t_tarea *tareas;
} t_tareas_cPID;

//ACTUALIZACIONES
typedef struct
{
    u_int32_t TID;
    u_int32_t posX;
    u_int32_t posY;
} t_actualizar_posicion;

typedef struct
{
    u_int32_t TID;
    char estado;
} t_actualizar_estado;

#endif