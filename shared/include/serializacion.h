#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TCB 1
#define PCB 2

typedef struct {
    u_int32_t size;
    void* stream;
} t_buffer;

typedef struct{
    u_int8_t codigo_operacion;
    t_buffer* buffer;
} t_paquete;

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
