#include "proceso3.h"

void dump(int sig)
{
    mkdir("./Dump", 0777);

    if (sig == SIGUSR2)
    {

        log_info(logger, "Se ha solicitado un DUMP de la memoria");

        FILE *fpDump;

        char *nombreArchivo = string_new();
        string_append(&nombreArchivo, "./Dump/Dump_");
        string_append(&nombreArchivo, temporal_get_string_time("%d%m%y%H%M%S%MS"));
        string_append(&nombreArchivo, ".dmp");

        fpDump = fopen(nombreArchivo, "w+");

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

    t_pagina_proceso *pag_proc;

    for (int i = 0; i < list_size(tabla_paginas); i++)
    {
        pag_proc = list_get(tabla_paginas, i);

        for (int j = 0; j < list_size(pag_proc->paginas); j++)
            fprintf(fpDump, "Proceso: %d\tPagina: %d\tMarco: %d\n", pag_proc->pid, j, (((int)list_get(pag_proc->paginas, j) - (int)memoria) / tamanio_paginas));
    }

    return;
}