#ifndef MAIN_3
#define MAIN_3

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include "shared_utils.h"

t_log *logger;
t_ram_conf *config;

int recibiendo_mensajes;
int aceptando_conexiones;
void *memoria;
int cantidad_segmentos;

enum TIPO {
    PCB = 0,
    TCB,
    TAREAS
};

typedef struct {
    u_int32_t pid;
    t_list* registros;
    u_int32_t indice;
} t_registro_procesos;

typedef struct {
    u_int32_t segmento; // Identificador (numero) del segmento
    void*     base;     // Posicion inicio del segmento
    u_int32_t tamanio;  // Tamaño del contenido del segmento
    u_int8_t  tipo;     // Tipo del contenido del segmento
    u_int32_t tid;      // Opcional
    u_int8_t  usado;    // Booleano para saber si el segmento puede sacarse
} t_registro_segmentos;

typedef struct {
    u_int32_t inicio;
    u_int32_t limite;
    u_int8_t  ocupado;
} estado_segmentos;

t_list*         tabla_procesos;
pthread_mutex_t acceso_memoria;
//GESTION DE MEMORIA EN LA SEGMENTACIOS
t_list*         tabla_estado_segmentos;
// GESTION DE MEMORIA EN LA PAGINACION
u_int8_t*       estado_frames;
u_int32_t       tamanio_paginas;

//Definidas en router.c
void comenzar_patota      (int, t_tareas_cPID *);
void iniciar_tripulante   (t_TCB *);
void solicitar_tarea      (int, int *datos_recibidos);

//Definidas en conexiones.c
void aceptar_conexiones   (void *);
void recibir_mensaje      (void *);

//Definidas en memoria.c
void*                 reservar_memoria(int);
t_registro_segmentos* guardar_tareas  (int, t_tarea*);
t_registro_segmentos* guardar_pcb     (t_PCB *);
t_registro_segmentos* guardar_tcb     (t_TCB);

//Definidas en segmentacion.c
void* reservar_segmento_FF(int);
void* reservar_segmento_BF(int);
void  compactar           (int);

#endif