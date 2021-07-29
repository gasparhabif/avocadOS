#ifndef MAIN_3
#define MAIN_3

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <curses.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <commons/temporal.h>
#include "shared_utils.h"
#include <nivel-gui/nivel-gui.h>
#include <nivel-gui/tad_nivel.h>
#include <sys/stat.h>
#include <sys/time.h>

#define ASSERT_CREATE(level, id, err)

t_log *logger;
t_ram_conf *config;

int aceptando_conexiones;
void *memoria;

struct timeval tv;

enum TIPO
{
    PCB = 0,
    TCB,
    TAREAS
};

typedef struct
{
    void *base;       // Posicion inicio del segmento
    uint32_t tamanio; // Tamaño del contenido del segmento
    uint8_t tipo;     // Tipo del contenido del segmento
    uint32_t id;      // Opcional
} t_registro_segmentos;

typedef struct
{
    uint32_t inicio;
    uint32_t limite;
    uint8_t ocupado;
} estado_segmentos;

typedef struct
{
    uint32_t ultimo_acceso;
    uint8_t ocupado;
} t_estado_frame;

typedef struct
{
    uint32_t id;  //para tripulantes es el tcb y para el pcb es el pid
    uint8_t tipo; // Tipo del contenido
    uint32_t offset;
    uint32_t tamanio;
    uint8_t modificado;
    uint8_t presencia;
} t_tabla_paginas_proceso;

typedef struct
{
    uint32_t offset;
    uint8_t pid;
    uint8_t num_pagina;
} t_pagina_memoria_virtual;

typedef struct
{
    uint32_t pid;
    t_list *paginas;
} t_pagina_proceso;

t_list *tabla_procesos;
t_list *tabla_memoria_virtual;
pthread_mutex_t acceso_memoria;
pthread_mutex_t m_procesos;
NIVEL *level;
char idMapa;
//GESTION DE MEMORIA EN LA SEGMENTACIOS
t_list *tabla_estado_segmentos;
// GESTION DE MEMORIA EN LA PAGINACION
t_list *estado_frames;
uint32_t tamanio_paginas;
t_list *tabla_paginas;
int maxima_cantidad_paginas;

//Definidas en conexiones.c
void aceptar_conexiones(void *);
void recibir_mensaje(void *);

//Definidas en router_segmentacion.c
void comenzar_patota(int, t_tareas_cPID *);
void iniciar_tripulante(int, t_TCBcPID *, char);
void solicitar_tarea(int, t_pidYtid *);
void mover_tripulante(t_envio_posicion *, char);
void actualizar_estado(t_estado *);
void eliminar_tripulante(t_pidYtid *, char);
void solicitar_tripulantes(int);

//Definidas en router_paginacion.c
void comenzar_patota_paginada(int, t_tareas_cPID *);
void iniciar_tripulante_paginado(int, t_TCBcPID *, char);
void solicitar_tarea_paginada(int, t_pidYtid *);
void mover_tripulante_paginada(t_envio_posicion *, char);
void actualizar_estado_paginada(t_estado *);
void solicitar_tripulantes_paginada(int);
void eliminar_tripulante_paginado(t_pidYtid *, char);
void guardar_tcb_paginacion(t_TCBcPID *);

//Definidas en memoria.c
void *reservar_memoria(int);
void liberar_memoria(int);
t_registro_segmentos *guardar_tareas(int, t_tarea *);
t_registro_segmentos *guardar_pcb(t_PCB *);
t_registro_segmentos *guardar_tcb(t_TCB);

//Definidas en segmentacion.c
void *reservar_segmento_FF(int);
void *reservar_segmento_BF(int);
void liberar_memoria_segmentacion(int);
void compactar(int);
void actualizar_tabla_procesos(int, int);
void cambio_posicion_pcb(int, int);
void cambio_posicion_tareas(int, int);

//Definidas en paginacion.c
void generar_archivo_swap();
void realizar_swap();
void swap_por_Clock();
void swap_por_LRU();
void guardar_tareas_pcb_paginacion(t_tareas_cPID *);
int solicitar_paginas(int, int);
int calcular_fragmentacion(int);

//Definidas en utils_segmentacion.c
t_list *buscar_lista_proceso(int);
t_PCB *buscar_pcb_proceso(t_list *, int);
t_registro_segmentos *buscar_registro_tcb(t_list *, int);
int traer_tarea(void *, t_list *, int, t_tarea *);
int cant_tareas(t_list *);
int cant_tripulantes(t_list *);
int eliminar_tcb(t_list *, int);
int eliminar_pcbOtareas(t_list *, int);
int obtener_PID(void *);
void eliminar_proceso(int);
int obtener_PIDproceso(t_list *);

//Definidas en utils_paginacion.c
t_list *buscar_paginas_proceso(int);
t_list *obtener_lista_proceso(int, int *);
void iniciar_frames(void);
t_pagina_proceso *obtener_paginas_proceso(int, int *);
int bytes_ocupados_pid(int);
int bytes_ocupados_lista(t_list *);
int cantidad_paginas_proceso(int);
int obtener_numero_instruccion(t_list *, int, int);
void *recuperar_elementos_proceso(int);
void guardar_elementos_proceso(int, void *);
t_tarea *obtenerTarea(t_list *, int, int);
int cant_tripulantes_paginacion(t_list *);
int obtener_pid_pag(t_list *);
int cant_tripulantes_proceso(int);
int calcular_paginas_ocupadas(int);
int obtener_timestamp(void);

//Definidas en serializacionPaginacion.c
void *serializar_PCB(t_PCB *);
void *serializar_TCB(t_TCB);
void *serializar_pTCB(t_TCB *);
void *serializar_TAREA(t_tarea *);
t_PCB *deserializar_PCB(void *);
t_TCB *deserializar_TCB(void *);
t_tarea *deserializar_TAREA(void *);

//Definidas en dump.c
void dump(int);
void dump_segmentacion(FILE *);
void dump_paginacion(FILE *);

// Definidas en memoria_virtual.c
void frame_a_memoria_virtual(void);

#endif