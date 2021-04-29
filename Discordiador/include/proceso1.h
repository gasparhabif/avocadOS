#ifndef PROCESO1_H
#define PROCESO1_H

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<commons/string.h>
#include<readline/history.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<netinet/in.h>

void iniciar_logger();
void iniciar_config();
void leer_config_dConexion(char *ip_RAM, char *puerto_RAM, char *ip_MONGO, char *puerto_MONGO);
char* leer_config(char* dato);

#endif