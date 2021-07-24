#ifndef PROCESO1_H
#define PROCESO1_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/txt.h>
#include <commons/collections/queue.h>
#include <readline/readline.h>
#include <readline/history.h>
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

//STRUCTS
typedef struct
{
    uint32_t pid;
    uint32_t tid;
    char estado;
    uint32_t posX;
    uint32_t posY;
    int sockfd_tripulante_ram;
    int sockfd_tripulante_mongo;
    pthread_mutex_t pausar_tripulante;
    int debeMorir;
} t_admin_tripulantes;

typedef struct
{
    t_TCB tcbTripulante;
    t_admin_tripulantes *admin;
} t_parametros_tripulantes;

//DEFINIDAS EN consola.c
void INICIAR_PATOTA(char **);
void LISTAR_TRIPULANTES(char **);
void EXPULSAR_TRIPULANTE(char **);
void INICIAR_PLANIFICACION(char **);
void PAUSAR_PLANIFICACION(char **);
void OBTENER_BITACORA(char **);
void IMPRIMIR_SEGMENTOS(char **);

//DEFINIDAS EN utils.c
t_tarea *leer_tareas(FILE *, int *, int *);
int contar_caracteres_especiales(size_t, char *, char);
int pausar(int);
void pausar_tripulantes(int);
void pausar_lista(t_list *, int);

int eliminarTripulante(t_list *, int);
int menor_tid_list(t_list *);
int mayor_tid_list(t_list *);
int matarTripulante(int);
int existeTripulante(int);
int avisoDeMuerte(int);
int revisarLista_avisoDeMuerte(t_list *, int);

//DEFINIDAS EN tripulantes.c
void tripulante(t_parametros_tripulantes *parametro);
t_tarea *solicitar_tarea(t_admin_tripulantes *admin, int *finTareas, int *duracionMovimientos, int *duracionEjecucion, int *duracionBloqueado);
int ejecutar_tarea(t_admin_tripulantes *admin, t_tarea *unaTarea, int *duracionMovimientos, int *duracionEjecucion);
void mover_tripulante(t_admin_tripulantes *admin, uint32_t posX, uint32_t posY, int movimientosPosibles, int *duracionMovimientos);
int ejecutar_tiempos_CPU(int duracionEjecucion, int tEjecutado);
void actualizar_estado(t_admin_tripulantes *admin, char nuevoEstado);
void mover_una_posicion(t_admin_tripulantes *admin, uint32_t posX, uint32_t posY);
int cantMovimientos(int xInicial, int yInicial, int xFinal, int yFinal);
void retardo_ciclo_cpu();
void retardo_ciclo_IO();

//DEFINIDAS EN sabotajes.c
void sabotajes();
void resolver_sabotaje();
int distancia_posiciones(t_posicion *, int, int);

//VARIABLES GLOBALES
t_log *logger;
t_cpu_conf *config;

int sockfd_mongo_sabotajes;
int sockfd_mongo_bitacoras;
int sockfd_ram;
int patota_id;
int ejecutandoTripulantes;
int ejecutandoPlanificador;
int escuchandoSabotajes;
int planificando;
int sabotaje;

sem_t s_multiprocesamiento;
pthread_mutex_t mutex_block;

t_list *exec;
t_list *ready;
t_list *bloq;
t_list *bloq_IO;

pthread_mutex_t m_listaExec;
pthread_mutex_t m_listaReady;
pthread_mutex_t m_listaBlock;
pthread_mutex_t m_listaBlockIO;

sem_t pause_exec;
pthread_mutex_t pause_block;

struct tm *tlocal;

#endif