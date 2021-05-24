#ifndef PROCESO2_H
#define PROCESO2_H

#include <stdio.h>
#include <commons/log.h>
#include <stdbool.h>
#include "shared_utils.h"
#include <pthread.h>

// Variables globales
t_log *logger;
t_store_conf *config;

void accept_connections(void *);
void connection_handler(void *);

// Tareas (definidas en tareas.c)
void generarOxigeno(int);
void consumirOxigeno(int);
void generarComida(int);
void consumirComida(int);
void generarBasura(int);
void descartarBasura();

#endif