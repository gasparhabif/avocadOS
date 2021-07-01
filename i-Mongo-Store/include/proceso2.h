#ifndef PROCESO2_H
#define PROCESO2_H

#include <stdio.h>
#include <commons/log.h>
#include <commons/bitarray.h>
#include <stdbool.h>
#include "shared_utils.h"
#include <pthread.h>

// Agregados para mapeo de memoria
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

// Constantes de FS
#define BLOCK_SIZE 64
#define BLOCKS 1024

// Estructuras
typedef struct
{
    uint32_t block_size;
    uint32_t blocks;
    t_bitarray *bitmap;
} t_superbloque;

// Paths de FS
char *files_dir_path;
char *bitacoras_dir_path;
char *superbloque_file_path;
char *blocks_file_path;
char *oxigeno_file_path;
char *comida_file_path;
char *basura_file_path;

// Variables globales
t_log *logger;
t_store_conf *config;
t_superbloque *superbloque;
char *blocks;

// Conexiones hacia el store (definidas en store_connections.c)
void discordiador_cxn_handler(void *);
void accept_tripulantes(void *);
void tripulante_cxn_handler(void *);

// Tareas de ES (definidas en tareas.c)
void ejecutarTarea(t_tarea *);
void finTareas();
void generarOxigeno(int);
void consumirOxigeno(int);
void generarComida(int);
void consumirComida(int);
void generarBasura(int);
void descartarBasura();
void tareaNormal();

// Registro de bitácora (definidas en tareas.c)
void registrarDesplazamiento();
void registrarInicioTarea();
void registrarFinTarea();
void registrarAtencionSabotaje();
void registrarResolucionSabotaje();

// Inicialización del FS (definidas en store_init.c)
void init_paths();
bool file_exists(char *);
void clean_bitarray(t_bitarray *);
t_superbloque *create_superbloque(uint32_t, uint32_t);
t_superbloque *load_superbloque();
t_superbloque *init_superbloque();
void print_superbloque();
void create_blocks(uint32_t);
char *load_blocks();
void sync_blocks();

#endif