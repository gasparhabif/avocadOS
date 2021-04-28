#ifndef CONFIGURATOR_H_
#define CONFIGURATOR_H_

#include <commons/config.h>
#include <string.h>

// DEBATE: Pasar ip y puerto a otro struct
// DEBATE: Usar enum para todos los campos de cada archivo

typedef struct
{
    char *ip_ram;
    int puerto_ram;
    char *ip_mongo;
    int puerto_mongo;
    int grado_multitarea;
    char *algoritmo;
    int quantum;
    int duracion_sabotaje;
    int retardo_ciclo_cpu;
} t_cpu_conf;

typedef struct
{
    char *punto_montaje;
    int puerto;
    int tiempo_sincronizacion;

} t_store_conf;

typedef struct
{
    int tamanio_memoria;
    char *esquema_memoria;
    int tamanio_pagina;
    int tamanio_swap;
    char *path_swap;
    char *algoritmo_reemplazo;
    int puerto;

} t_ram_conf;

// Free the return value of init_config after using it
t_cpu_conf *get_cpu_config(char *);
t_store_conf *get_store_config(char *);
t_ram_conf *get_ram_config(char *);
