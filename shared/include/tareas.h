#ifndef serializacion_H
#define serializacion_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "common_types.h"

t_tarea_descomprimida* descomprimir_tarea(t_tarea*, int*, char*);

#endif