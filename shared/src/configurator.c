#include "configurator.h"

static void verificar_existencia_campos(char **, t_config *);
int tamanio_string_array(char **);
static void llenar_campos_cpu(char **);
static void llenar_campos_store(char **);
static void llenar_campos_ram(char **);
static void verificar_algoritmo_valido(char *);

static void verificar_existencia_campos(char **campos, t_config *config)
{
    int i;
    int length = tamanio_string_array(campos);

    for (i = 0; i < length; i++)
    {
        if (!config_has_property(config, campos[i]))
        {
            printf("Error, no existe el campo %s en el archivo de configuración.\n", campos[i]);
            exit(0);
            // log error
        }
    }
}

int tamanio_string_array(char **array)
{
    return sizeof(array) / sizeof(array[0]);
}

t_cpu_conf *get_cpu_config(char *path)
{
    t_cpu_conf *response = malloc(sizeof(t_cpu_conf));
    t_config *basicConfig = config_create(path);
    char *campos_cpu[CANTIDAD_CAMPOS_CPU];
    llenar_campos_cpu(campos_cpu);
    verificar_existencia_campos(campos_cpu, basicConfig);

    // TODO: Add checks to formats
    response->ip_ram = config_get_string_value(basicConfig, campos_cpu[IP_RAM]);
    response->puerto_ram = config_get_int_value(basicConfig, campos_cpu[PUERTO_RAM]);
    response->ip_mongo = config_get_string_value(basicConfig, campos_cpu[IP_MONGO]);
    response->puerto_mongo = config_get_int_value(basicConfig, campos_cpu[PUERTO_MONGO]);
    response->grado_multitarea = config_get_int_value(basicConfig, campos_cpu[GRADO_MULTITAREA]);
    response->algoritmo = config_get_string_value(basicConfig, campos_cpu[ALGORITMO]);
    response->quantum = config_get_int_value(basicConfig, campos_cpu[QUANTUM]);
    response->duracion_sabotaje = config_get_int_value(basicConfig, campos_cpu[DURACION_SABOTAJE]);
    response->retardo_ciclo_cpu = config_get_int_value(basicConfig, campos_cpu[RETARDO_CICLO_CPU]);

    verificar_algoritmo_valido(response->algoritmo);

    // free de los campos?
    free(basicConfig);

    return response;
}

static void llenar_campos_cpu(char *response[CANTIDAD_CAMPOS_CPU])
{
    response[IP_RAM] = "IP_MI_RAM_HQ";
    response[PUERTO_RAM] = "PUERTO_MI_RAM_HQ";
    response[IP_MONGO] = "IP_I_MONGO_STORE";
    response[PUERTO_MONGO] = "PUERTO_I_MONGO_STORE";
    response[GRADO_MULTITAREA] = "GRADO_MULTITAREA";
    response[ALGORITMO] = "ALGORITMO";
    response[QUANTUM] = "QUANTUM";
    response[DURACION_SABOTAJE] = "DURACION_SABOTAJE";
    response[RETARDO_CICLO_CPU] = "RETARDO_CICLO_CPU";
}

static void verificar_algoritmo_valido(char *algoritmo)
{
    if (!string_equals_ignore_case("FIFO", algoritmo) && !string_equals_ignore_case("RR", algoritmo))
    {
        printf("Error, el algoritmo %s especificado en el archivo de configuración no es válido.\n", algoritmo);
        printf("\t(Algoritmos válidos: FIFO - RR)\n");
        exit(0);
        // log error
    }
}

t_store_conf *get_store_config(char *path)
{
    t_store_conf *response = malloc(sizeof(t_store_conf));
    t_config *basicConfig = config_create(path);
    char *campos_store[CANTIDAD_CAMPOS_STORE];
    llenar_campos_store(campos_store);
    verificar_existencia_campos(campos_store, basicConfig);

    // TODO: Check existing file or create it
    response->punto_montaje = config_get_string_value(basicConfig, campos_store[PUNTO_MONTAJE]);
    response->puerto = config_get_int_value(basicConfig, campos_store[STORE_PUERTO]);
    response->tiempo_sincronizacion = config_get_int_value(basicConfig, campos_store[TIEMPO_SINCRONIZACION]);

    free(basicConfig);

    return response;
}

static void llenar_campos_store(char *response[CANTIDAD_CAMPOS_STORE])
{
    response[PUNTO_MONTAJE] = "PUNTO_MONTAJE";
    response[STORE_PUERTO] = "PUERTO";
    response[TIEMPO_SINCRONIZACION] = "TIEMPO_SINCRONIZACION";
}

t_ram_conf *get_ram_config(char *path)
{
    t_ram_conf *response = malloc(sizeof(t_ram_conf));
    t_config *basicConfig = config_create(path);
    char *campos_ram[CANTIDAD_CAMPOS_RAM];
    llenar_campos_ram(campos_ram);
    verificar_existencia_campos(campos_ram, basicConfig);

    response->tamanio_memoria = config_get_int_value(basicConfig, campos_ram[TAMANIO_MEMORIA]);
    // TODO: Chequear esquema valido
    response->esquema_memoria = config_get_string_value(basicConfig, campos_ram[ESQUEMA_MEMORIA]);
    // TODO: Check que sea multiplo de 2
    response->tamanio_pagina = config_get_int_value(basicConfig, campos_ram[TAMANIO_PAGINA]);
    response->tamanio_swap = config_get_int_value(basicConfig, campos_ram[TAMANIO_SWAP]);
    // TODO: Check existing file or create it
    response->path_swap = config_get_string_value(basicConfig, campos_ram[PATH_SWAP]);
    // TODO: Chequear algoritmo valido
    response->algoritmo_reemplazo = config_get_string_value(basicConfig, campos_ram[ALGORITMO_REEMPLAZO]);
    response->puerto = config_get_int_value(basicConfig, campos_ram[RAM_PUERTO]);

    free(basicConfig);

    return response;
}

static void llenar_campos_ram(char *response[CANTIDAD_CAMPOS_RAM])
{
    response[TAMANIO_MEMORIA] = "TAMANIO_MEMORIA";
    response[ESQUEMA_MEMORIA] = "ESQUEMA_MEMORIA";
    response[TAMANIO_PAGINA] = "TAMANIO_PAGINA";
    response[TAMANIO_SWAP] = "TAMANIO_SWAP";
    response[PATH_SWAP] = "PATH_SWAP";
    response[ALGORITMO_REEMPLAZO] = "ALGORITMO_REEMPLAZO";
    response[RAM_PUERTO] = "PUERTO";
}