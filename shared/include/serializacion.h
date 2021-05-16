#ifndef serializacion_H
#define serializacion_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//CODIGOS DE OPERACION
#define COMENZAR_PATOTA 0
#define PUNTERO_PCB 1
#define INICIAR_TRIPULANTE 2
#define SOLICITAR_TAREA 3
#define ENVIAR_PROXIMA_TAREA 4
#define REALIZAR_TAREA 5

//CODIGOS DE TAREAS
#define MOVER_POSICION 0
#define GENERAR_OXIGENO 1
#define CONSUMIR_OXIGENO 2
#define GENERAR_COMIDA 3
#define CONSUMIR_COMIDA 4
#define GENERAR_BASURA 5
#define DESCARTAR_BASURA 6

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

//DEFINIDAS EN serializacion.c
void *serializarTCB(t_TCB *);
void *serializarTarea(t_tarea);
void *serializarTareas_cPID(t_tarea *unasTareas, int patotaID, int *, int);
void *serializarInt(uint32_t, uint32_t);

//DEFINIDAS EN deserializacion.c
t_TCB *deserializarTCB(t_buffer *);
t_tarea *deserializarTarea(t_buffer *);
t_tareas_cPID *deserializarTareas_cPID(t_buffer *);
int *deserializarInt(t_buffer *);

#endif