#include "proceso3.h"

void dump(int sig)
{
    mkdir("/home/utnso/Dump", 0777);

    if (sig == SIGUSR2)
    {

        log_info(logger, "Se ha solicitado un DUMP de la memoria");

        FILE *fpDump;

        char *nombreArchivo = string_new();
        nombreArchivo = string_from_format("%s%d%s", "/home/utnso/Dump/Dump_", obtener_timestamp(), ".dmp");

        fpDump = fopen(nombreArchivo, "w+");

        log_info(logger, "Archivo de dump creado en path: %s", nombreArchivo);

        free(nombreArchivo);

        fprintf(fpDump, "----------------------------------------------------------------\n");
        fprintf(fpDump, "Dump %s\n", temporal_get_string_time("%d/%m/%y %H:%M:%S"));

        if (strcmp(config->esquema_memoria, "SEGMENTACION") == 0)
        {
            dump_segmentacion(fpDump);
        }
        else if (strcmp(config->esquema_memoria, "PAGINACION") == 0)
        {
            dump_paginacion(fpDump);
        }

        fprintf(fpDump, "----------------------------------------------------------------\n");

        fclose(fpDump);
    }
}

void dump_segmentacion(FILE *fpDump)
{

    //pthread_mutex_lock(&m_procesos);

    t_list *tablaUnProceso;
    t_registro_segmentos *reg_seg = malloc(sizeof(t_registro_segmentos));

    for (int i = 0; i < list_size(tabla_procesos); i++)
    {
        tablaUnProceso = list_get(tabla_procesos, i);

        for (int j = 0; j < list_size(tablaUnProceso); j++)
        {
            reg_seg = list_get(tablaUnProceso, j);
            fprintf(fpDump, "Proceso: %d\tSegmento: %d\tInicio: %p\tTam: %dB\n", obtener_PIDproceso(tablaUnProceso), j + 1, reg_seg->base, reg_seg->tamanio);
        }
    }

    //pthread_mutex_lock(&m_procesos);

    return;
}

void dump_paginacion(FILE *fpDump)
{
    for (int i = 0; i < maxima_cantidad_paginas; i++)
    {
        t_estado_frame *frame = list_get(estado_frames, i);
        char *estado = frame->ocupado ? "Ocupado" : "Libre";
        fprintf(fpDump, "Marco: %d \t", i);
        fprintf(fpDump, "Estado: %s \t", estado);
        frame->pid == 0 ? fprintf(fpDump, "Proceso: -\t") : fprintf(fpDump, "Proceso: %d\t", frame->pid);
        frame->pag_proc == 0 ? fprintf(fpDump, "Pagina: -\n") : fprintf(fpDump, "Pagina: %d \n", frame->pag_proc);
    }

    return;
}