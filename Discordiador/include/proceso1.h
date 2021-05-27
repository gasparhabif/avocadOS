#ifndef PROCESO1_H
#define PROCESO1_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/txt.h>
#include <commons/collections/queue.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include "shared_utils.h"

#define MAX_DATA_SIZE 30

//DEFINIDAS EN consola.c
void INICIAR_PATOTA       (char **);
void LISTAR_TRIPULANTES   (char **);
void EXPULSAR_TRIPULANTE  (char **);
void INICIAR_PLANIFICACION(char **);
void PAUSAR_PLANIFICACION (char **);
void OBTENER_BITACORA     (char **);

//DEFINIDAS EN utils.c
t_tarea *leer_tareas                 (FILE *, int *);
int      contar_caracteres_especiales(size_t, char *, char);

//DEFINIDAS EN tripulantes.c
void     tripulante          (void *parametro);
t_tarea* solicitar_tarea     (int *finTareas);
int      ejecutar_tarea      (t_tarea *unaTarea);
int      ejecutar_tiempos_CPU(int duracionEjecucion, int tEjecutado);
void     actualizar_estado   (int socket, uint32_t tid, char nuevoEstado);
void     mover_tripulante    (int cantMovimientos, u_int32_t posX, u_int32_t posY);
void     mover_una_posicion  (u_int32_t posX, u_int32_t posY);
int      cantMovimientos     (int xInicial, int yInicial, int xFinal, int yFinal);
void     retardo_ciclo_cpu   ();
    
//DEFINIDAS EN sabotajes.c
void sabotajes();

//VARIABLES GLOBALES
t_log *logger;
t_cpu_conf *config;
int sockfd_mongo;
int sockfd_ram;
int patota_id;
int ejecutandoTripulantes;
int ejecutandoPlanificador;
int escuchandoSabotajes;
int planificando;
int sabotaje;
pthread_mutex_t mutex_exec;
pthread_mutex_t mutex_block;
sem_t s_multiprogramacion;

#endif