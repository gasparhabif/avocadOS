#ifndef serializacion_H
#define serializacion_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "common_types.h"

//DEFINIDAS EN serializacion.c
void *serializarTCB        (t_TCB *unTCB, int *);
void *serializarTarea      (t_tarea, int *);
void *serializarTareas_cPID(t_tarea *unasTareas, int patotaID, int *, int);
void *serializarInt        (uint32_t, uint32_t, int *);

//DEFINIDAS EN deserializacion.c
t_TCB         *deserializarTCB         (t_buffer *);
t_tarea       *deserializarTarea       (t_buffer *);
t_tareas_cPID *deserializarTareas_cPID (t_buffer *);
int           *deserializarInt         (t_buffer *);

#endif