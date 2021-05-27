#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

//CODIGOS DE OPERACION
#define COMENZAR_PATOTA      0
#define PUNTERO_PCB          1
#define INICIAR_TRIPULANTE   2
#define SOLICITAR_TAREA      3
#define ENVIAR_PROXIMA_TAREA 4
#define REALIZAR_TAREA       5
#define MOVER_TRIPULANTE     6
#define ACTUALIZAR_ESTADO    7

//CODIGOS DE TAREAS
#define FIN_TAREAS           0
#define MOVER_POSICION       1
#define GENERAR_OXIGENO      2
#define CONSUMIR_OXIGENO     3
#define GENERAR_COMIDA       4
#define CONSUMIR_COMIDA      5
#define GENERAR_BASURA       6
#define DESCARTAR_BASURA     7

//ESTADOS
#define NEW                 'N'
#define READY               'R'
#define EXEC                'E'
#define BLOCKED_IO          'B'
#define BLOCKED_EMERGENCY   'Y'
#define EXIT                'X'

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

//BASICOS
typedef struct
{
    u_int32_t posX;
    u_int32_t posY;
} t_posicion;

typedef struct
{
    u_int32_t TID;
    u_int32_t posX;
    u_int32_t posY;
} t_envio_posicion;

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