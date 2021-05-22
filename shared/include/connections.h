#ifndef CONNECTIONS_H_
#define CONNECTIONS_H_

#include <commons/config.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "serializacion.h"

int connect_to(char *ip, int port);
int init_server(int port);

void* recibir_paquete(int socket);
void* recibir_paquete_cCOP(int, int *);

#endif