#ifndef PROCESO1_H
#define PROCESO1_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <readline/readline.h>
#include <commons/string.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "shared_utils.h"

struct d_conexion {
    char *puerto;
    int *socket;
};

struct d_mensaje {
    char *datos;
    int *socket;
};

void abrir_conexion(void *unaConexion);
void enviar_mensaje(void *unMensaje);
void recibir_mensaje(int sockfd);

int *sockfd_mongo;
int *sockfd_ram;
t_log *logger;
int escuchando;

#endif