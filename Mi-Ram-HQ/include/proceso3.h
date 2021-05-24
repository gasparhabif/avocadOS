#ifndef MAIN_3
#define MAIN_3

#include <stdio.h>
#include <commons/log.h>
#include <stdbool.h>
#include <pthread.h>
#include "shared_utils.h"

t_log* logger;
int recibiendo_mensajes;
int aceptando_conexiones;

//Definidas en router.c
void comenzar_patota(int, t_tareas_cPID *);
void iniciar_tripulante(t_TCB *);

//Definidas en conexiones.c
void aceptar_conexiones(void *);
void recibir_mensaje   (void *);
void solicitar_tarea   (int* datos_recibidos);

#endif