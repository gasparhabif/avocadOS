#include "proceso3.h"

void dump (int sig){
    if(sig == SIGUSR2){

        FILE *fpDump;

        char *nombreArchivo = string_new();
	    string_append(&nombreArchivo, "./Dump/Dump_<timestamp>.dmp");
	    string_append(&nombreArchivo, temporal_get_string_time("%d%m%y%H%M%S%MS"));
        
        fpDump = fopen(nombreArchivo, "w+");

        if (strcmp(config->esquema_memoria, "SEGMENTACION") == 0)
        {
            dump_segmentacion(fpDump);
        }
        else if (strcmp(config->esquema_memoria, "PAGINACION") == 0)
        {
            //dump_paginacion();
        }
    }
}

void dump_segmentacion(FILE *fpDump){
    
}