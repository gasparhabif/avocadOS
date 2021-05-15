#ifndef PROCESO1_H
#define PROCESO1_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/txt.h>
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
#include "shared_utils.h"

#define MAX_DATA_SIZE 30

//DEFINIDAS EN consola.c
void INICIAR_PATOTA       (char **);
void LISTAR_TRIPULANTES   (char **);
void EXPULSAR_TRIPULANTE  (char **);
void INICIAR_PLANIFICACION(char **);
void PAUSAR_PLANIFICACION (char **);
void OBTENER_BITACORA     (char **);

//DEFINIDAS EN aux.c
t_tarea* leer_tareas(FILE *);
int contar_caracteres_especiales(size_t, char *, char);

//DEFINIDAS EN tripulantes.c
void tripulante(void *);
int ejecutar_tarea (t_tarea*, int);

//VARIABLES GLOBALES
int sockfd_mongo;
int sockfd_ram;
t_log *logger;
t_cpu_conf *config;
int patota_id;
int ejecutandoTripulantes;
int ejecutandoPlanificador;
int planificando;
int turno;

#endif