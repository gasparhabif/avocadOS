#ifndef PROCESO1_H
#define PROCESO1_H

#include "shared_utils.h"
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include<commons/log.h>
#include<commons/config.h>
#include<commons/string.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

void iniciar_logger(t_log *l_logger);
void iniciar_config(t_config *l_config);
void leer_config_dConexion(t_config *l_config, char *ip_RAM, char *puerto_RAM, char *ip_MONGO, char *puerto_MONGO);
char* leer_config(t_config *l_config, char* dato);

#endif