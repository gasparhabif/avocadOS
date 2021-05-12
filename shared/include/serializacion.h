#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//CODIGOS DE OPERACION
#define TCB            0
#define PCB            1
#define TAREAS         2
#define TAREAS_cPID    3
#define REALIZAR_TAREA 4

//CODIGOS DE TAREAS
#define MOVER_POSICION   0
#define GENERAR_OXIGENO  1
#define CONSUMIR_OXIGENO 2
#define GENERAR_COMIDA   3
#define CONSUMIR_COMIDA  4
#define GENERAR_BASURA   5
#define DESCARTAR_BASURA 6

//BUFFERS Y PAQUETE
typedef struct {
    u_int32_t size;
    u_int32_t PID;
    void* stream;
} t_buffer_PID;

typedef struct{
    u_int8_t codigo_operacion;
    t_buffer_PID* buffer;
} t_paquete_PID;

typedef struct {
    u_int32_t size;
    void* stream;
} t_buffer;

typedef struct{
    u_int8_t codigo_operacion;
    t_buffer* buffer;
} t_paquete;

//TCB Y PCB
typedef struct{
    u_int32_t PID;
    u_int32_t tareas;
} t_PCB;

typedef struct {
    u_int32_t TID;
    char estado;
    u_int32_t posX;
    u_int32_t posY;
    u_int32_t proximaInstruccion;
    u_int32_t puntero_PCB;
} t_TCB;

//TAREAS
typedef struct {
    u_int8_t codigoTarea;
    u_int32_t parametro;
    u_int32_t posX;
    u_int32_t posY;
    u_int32_t duracionTarea;
} t_tareas;

//RECEPCION DE TAREAS DEL MONGO
typedef struct {
    u_int8_t codigoTarea;
    u_int32_t parametro;
} tareas_mongo;

//DEFINIDAS EN serializacion.c
void* serializarTCB        (t_TCB*);
void* serializarPCB        (t_PCB);
void* serializarTareas     (t_tareas*);
void* serializarTareas_cPID(t_tareas*, int);

//DEFINIDAS EN deserializacion.c
t_TCB*    deserializarTCB(t_paquete*);
t_PCB*    deserializarPCB(t_paquete*);
t_tareas* deserializarTareas(t_paquete*);