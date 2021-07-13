#ifndef CONFIGURATOR_H_
#define CONFIGURATOR_H_

#include <commons/config.h>
#include <commons/string.h>
#include <stdlib.h>
#include <stdio.h> // Eliminar luego

// TODO: Desarrollar destroyers para cada TAD

#define CANTIDAD_CAMPOS_CPU 9
#define CANTIDAD_CAMPOS_STORE 3
#define CANTIDAD_CAMPOS_RAM 8

enum POS_CAMPOS_CPU
{
    IP_RAM,
    PUERTO_RAM,
    IP_MONGO,
    PUERTO_MONGO,
    GRADO_MULTITAREA,
    ALGORITMO,
    QUANTUM,
    DURACION_SABOTAJE,
    RETARDO_CICLO_CPU
};

enum POS_CAMPOS_STORE
{
    PUNTO_MONTAJE,
    STORE_PUERTO,
    TIEMPO_SINCRONIZACION
};
enum POS_CAMPOS_RAM
{
    TAMANIO_MEMORIA,
    ESQUEMA_MEMORIA,
    TAMANIO_PAGINA,
    TAMANIO_SWAP,
    PATH_SWAP,
    ALGORITMO_REEMPLAZO,
    CRITERIO_SELECCION,
    RAM_PUERTO
};

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
    char *criterio_seleccion;
    int puerto;

} t_ram_conf;

/* 
    Loads all configuration values for CPU (Discordiador)
    in a Struct with previous corresponding checks.
    You must free the return value after using it.
*/
t_cpu_conf *get_cpu_config(char *);
/* 
    Loads all configuration values for Store (i-Mongo-Store)
    in a Struct with previous corresponding checks.
    You must free the return value after using it.
*/
t_store_conf *get_store_config(char *);
/* 
    Loads all configuration values for CPU (Mi-Ram-HQ)
    in a Struct with previous corresponding checks.
    You must free the return value after using it.
*/
t_ram_conf *get_ram_config(char *);

#endif