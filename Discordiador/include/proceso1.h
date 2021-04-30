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

#include "shared_utils.h"

void iniciar_logger();

#endif