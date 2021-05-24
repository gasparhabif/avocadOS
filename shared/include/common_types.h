#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

enum CODIGOS_DE_OPERACION
{
    COMENZAR_PATOTA = 0,
    PUNTERO_PCB,
    INICIAR_TRIPULANTE,
    SOLICITAR_TAREA,
    ENVIAR_PROXIMA_TAREA,
    REALIZAR_TAREA
};

enum CODIGOS_DE_TAREAS
{
    FIN_TAREAS = 10,
    GENERAR_OXIGENO,
    CONSUMIR_OXIGENO,
    GENERAR_COMIDA,
    CONSUMIR_COMIDA,
    GENERAR_BASURA,
    DESCARTAR_BASURA,
    MOVER_POSICION // ¿De dónde sale esta tarea?
};

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

//TAREAS
typedef struct
{
    u_int8_t codigoTarea;
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

//RECEPCION DE TAREAS DEL MONGO
typedef struct
{
    u_int8_t codigoTarea;
    u_int32_t parametro;
} tareas_mongo;

#endif