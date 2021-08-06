#ifndef serializacion_H
#define serializacion_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "common_types.h"

//DEFINIDAS EN serializacion.c
void *serializarTCB(uint32_t, t_TCB, int *);
void *serializarTarea(t_tarea *, int, int *);
void *serializarTareas_cPID(t_tarea *, int, int *, int);
void *serializarInt(uint32_t, uint8_t, int *);
void *serializar_ActulizacionEstado(uint32_t, uint32_t, char, int *);
void *serializar_envioPosicion(uint32_t, uint32_t, uint32_t, uint32_t, int *);
void *serializar_envioSabotaje(uint32_t, uint32_t, int *);
void *serializar_pidYtid(uint32_t, uint32_t, uint8_t, int *);
void *serializar_tcbMostrar(uint32_t, t_TCBmostrar *, int *);
void *serializar_bitacora(t_bitacora *, uint32_t *);
void *serializar_ejecutarTarea(uint32_t, char *, uint32_t, int *);
void *serializar_posicionCtid(uint32_t, uint32_t, uint32_t, int *);
void *serializar_bitacora_tripulante(uint32_t, char *, int *);
void *serializar_string(char *, uint32_t, int *);

//DEFINIDAS EN deserializacion.c
t_TCBcPID *deserializarTCB(t_buffer *);
t_tarea *deserializarTarea(t_buffer *);
t_tareas_cPID *deserializarTareas_cPID(t_buffer *);
int *deserializarInt(t_buffer *);
t_estado *deserializar_ActulizacionEstado(t_buffer *);
t_envio_posicion *deserializar_envioPosicion(t_buffer *);
t_posicion *deserealizar_posicion(t_buffer *);
t_pidYtid *deserealizar_pidYtid(t_buffer *);
t_ListaTripulantes *deserealizarTCBmostrar(t_buffer *);
t_bitacora *deserializar_bitacora(t_buffer *);
t_ejecutar_tarea *deserealizar_ejecutarTarea(t_buffer *);
t_tidCposicion *deserializar_posicionCtid(t_buffer *);
char *deserializar_bitacora_tripulante(t_buffer *);

#endif