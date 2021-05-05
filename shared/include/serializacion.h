#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define TCBcop 0
#define PCBcop 1

typedef struct {
    u_int32_t size;
    void* stream;
} t_buffer;

typedef struct{
    uint8_t codigo_operacion;
    t_buffer* buffer;
} t_paquete;

typedef struct{
    uint32_t PID;
    uint32_t tareas;
} PCB;

typedef struct{
    u_int32_t TID;
    char estado;
    uint32_t posX;
    uint32_t posY;
    uint32_t proximaInstruccion;
    uint32_t puntero_PCB;
} TCB;
