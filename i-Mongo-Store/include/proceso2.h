#ifndef PROCESO2_H
#define PROCESO2_H

#include <stdio.h>
#include <commons/log.h>
#include <commons/bitarray.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>
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
#define BLOCK_SIZE 8
#define BLOCKS 16

// Estructuras
typedef struct
{
    uint32_t block_size;
    uint32_t blocks;
    t_bitarray *bitmap;
} t_superbloque;

typedef struct
{
    char *path;
    int size;
    t_list *blocks;
} t_bitacora;

typedef struct
{
    char *path;
    int size;
    int block_count;
    t_list *blocks;
    char *caracter_llenado;
} t_recurso;

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
int superbloque_fd;
int superbloque_file_size;
void *superbloque_file;
int blocks_fd;
int blocks_file_size;
char *blocks_file;
char *blocks_file_copy;

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
void create_dirs();
void clean_bitarray(t_bitarray *);
void create_superbloque(uint32_t, uint32_t);
t_superbloque *load_superbloque();
void print_superbloque();
void create_blocks();
char *load_blocks();
void sync_blocks();

// Bitácoras utils (definidas en bitacoras_utils.c)
void create_bitacora(char *);
t_bitacora *load_bitacora(char *);
void update_bitacora(t_bitacora *);
void registrar_bitacora(t_bitacora *, char *);
char *blocks_list_to_string(t_list *);

// SuperBloque utils (definidas en superbloque_utils.c)
int get_free_block();
void set_block(int);

// Recursos utils (definidas en recursos_utils.c)
void create_recurso(char *, char *);
t_recurso *load_recurso(char *);
void print_recurso(t_recurso *);
void update_recurso(t_recurso *);
void agregar_recurso(t_recurso *, int);
void eliminar_recurso(t_recurso *, int);

#endif