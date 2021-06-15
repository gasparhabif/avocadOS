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

// Constantes de FS (deberían venir de config)
#define BLOCK_SIZE 4096
#define BLOCKS 128

// Directorios de FS
char *files_dir;
char *bitacoras_dir;

// Paths de FS
char *superbloque_path;

// Variables globales
t_log *logger;
t_store_conf *config;

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
void init_dirs();
t_bitarray *init_bitmap(size_t);
char *bitarray_to_string(t_bitarray *);
void init_superbloque(uint32_t, uint32_t);
void init_fs();

#endif