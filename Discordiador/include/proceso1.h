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
void     tripulante           (void *);
t_tarea* solicitar_tarea      (int, uint32_t, int *, int *, int *, t_posicion);
int      ejecutar_tarea       (t_tarea *, int *, int *, int);
int      ejecutar_tiempos_CPU (int *);
void     actualizar_estado    (int, uint32_t, char);
void     mover_tripulante     (int, int, uint32_t, u_int32_t, u_int32_t);
int      cantMovimientos      (int, int, int, int);

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
pthread_mutex_t mutex;
sem_t s_multiprogramacion;

#endif