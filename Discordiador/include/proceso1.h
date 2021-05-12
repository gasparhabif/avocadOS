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
void INICIAR_PATOTA(char **parametros);
void LISTAR_TRIPULANTES(char **parametros);
void EXPULSAR_TRIPULANTE(char **parametros);
void INICIAR_PLANIFICACION(char **parametros);
void PAUSAR_PLANIFICACION(char **parametros);
void OBTENER_BITACORA(char **parametros);

//DEFINIDAS EN aux.c
t_tareas* leer_tareas(FILE *);
int contar_caracteres_especiales(size_t, char *, char);

//DEFINIDAS EN tripulantes.c
void tripulante(void *tcb);

//VARIABLES GLOBALES
int sockfd_mongo;
int sockfd_ram;
t_log *logger;
int escuchando;
t_cpu_conf *config;
int patota_id;
int exec;
int turno;

#endif