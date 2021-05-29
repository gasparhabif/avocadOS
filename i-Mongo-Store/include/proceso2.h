#ifndef PROCESO2_H
#define PROCESO2_H

#include <stdio.h>
#include <commons/log.h>
#include <commons/bitarray.h>
#include <stdbool.h>
#include "shared_utils.h"
#include <pthread.h>

// Estructuras de FS
typedef struct
{
    uint32_t block_size;
    uint32_t blocks;
    t_bitarray *bitmap;
} t_superbloque;

typedef struct
{
    // ¿Cantidad libre?
    // ¿Array de caracteres?
} t_block;

typedef struct
{
    uint32_t size;
    uint32_t block_count;
    uint32_t blocks[];
    char caracter_llenado;
    char *md5_archivo;
} t_file;

typedef struct
{
    uint32_t size;
    uint32_t blocks;
} t_bitacora;

// Variables globales
t_log *logger;
t_store_conf *config;
t_superbloque *superbloque;

// Conexiones hacia el store (definidas en store_connections.c)
void discordiador_cxn_handler(void *);
void accept_tripulantes(void *);
void tripulante_cxn_handler(void *);

// Tareas de ES (definidas en tareas.c)
void generarOxigeno(int);
void consumirOxigeno(int);
void generarComida(int);
void consumirComida(int);
void generarBasura(int);
void descartarBasura();

// Registro de bitácora (definidas en tareas.c)
void registrarDesplazamiento();
void registrarInicioTarea();
void registrarFinTarea();
void registrarAtencionSabotaje();
void registrarResolucionSabotaje();

// Inicialización del FS (definidas en store_init.c)
bool file_exists(char *);
void init_fs();

#endif