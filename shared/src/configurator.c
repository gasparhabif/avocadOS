#include "configurator.h"

t_cpu_conf *get_cpu_config(char *path)
{
    t_cpu_conf *response = malloc(sizeof(t_cpu_conf));
    t_config basicConfig = config_create(path);
    // TODO: Add check existance: bool config_has_property(t_config*, char* key);
    // TODO: Add checks to formats
    (response)->ip_ram = config_get_string_value(basicConfig, "IP_MI_RAM_HQ");
    response->puerto_ram = config_get_int_value(basicConfig, "PUERTO_MI_RAM_HQ");
    (response)->ip_mongo = config_get_string_value(basicConfig, "IP_I_MONGO_STORE");
    response->puerto_mongo = config_get_int_value(basicConfig, "PUERTO_I_MONGO_STORE");
    response->grado_multitarea = config_get_int_value(basicConfig, "GRADO_MULTITAREA");
    // TODO: Chequear algoritmo valido
    response->algoritmo = config_get_string_value(basicConfig, "ALGORITMO");
    response->quantum = config_get_int_value(basicConfig, "QUANTUM");
    response->duracion_sabotaje = config_get_int_value(basicConfig, "DURACION_SABOTAJE");
    response->retardo_ciclo_cpu = config_get_int_value(basicConfig, "RETARDO_CICLO_CPU");

    return response;
}
t_store_conf *get_store_config(char *path)
{
    t_store_conf *response = malloc(sizeof(t_store_conf));
    t_config basicConfig = config_create(path);
    // TODO: Add check existance: bool config_has_property(t_config*, char* key);

    // TODO: Check existing file or create it
    response->punto_montaje = config_get_string_value(basicConfig, "PUNTO_MONTAJE");
    response->puerto = config_get_int_value(basicConfig, "PUERTO");
    response->tiempo_sincronizacion = config_get_int_value(basicConfig, "TIEMPO_SINCRONIZACION");

    return response;
}
t_ram_conf *get_ram_config(char *path)
{
    t_ram_conf *response = malloc(sizeof(t_ram_conf));
    t_config basicConfig = config_create(path);

    // TODO: Add check existance: bool config_has_property(t_config*, char* key);
    response->tamanio_memoria = config_get_int_value(basicConfig, "TAMANIO_MEMORIA");
    // TODO: Chequear esquema valido
    response->esquema_memoria = config_get_string_value(basicConfig, "ESQUEMA_MEMORIA");
    // TODO: Check que sea multiplo de 2
    response->tamanio_pagina = config_get_int_value(basicConfig, "TAMANIO_PAGINA");
    response->tamanio_swap = config_get_int_value(basicConfig, "TAMANIO_SWAP");
    // TODO: Check existing file or create it
    response->path_swap = config_get_string_value(basicConfig, "PATH_SWAP");
    // TODO: Chequear algoritmo valido
    response->algoritmo_reemplazo = config_get_string_value(basicConfig, "ALGORITMO_REEMPLAZO");
    response->puerto = config_get_int_value(basicConfig, "PUERTO");

    return response;
}