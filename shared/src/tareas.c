#include "tareas.h"

t_tarea_descomprimida* descomprimir_tarea(t_tarea* tarea_recibida, int* len_tarea, char* nom_tarea){

    t_tarea_descomprimida *tarea_descomprimida = malloc(sizeof(t_tarea_descomprimida));
    
    int cantEspacios = 0;

    for (int i = 0; tarea_recibida->tarea[i]!=NULL; i++)
    {
        if (tarea_recibida->tarea[i] == ' ')
            cantEspacios++;   
    }

    if (cantEspacios > 0)
    {
        //ES UNA TAREA DE E/S
        char **tarea = string_split(tarea_recibida->tarea, " ");

        if (strcmp(tarea[0], "GENERAR_OXIGENO") == 0)
            tarea_descomprimida->codigoTarea = GENERAR_OXIGENO;
        else if (strcmp(tarea[0], "CONSUMIR_OXIGENO") == 0)
            tarea_descomprimida->codigoTarea = CONSUMIR_OXIGENO;      
        else if (strcmp(tarea[0], "GENERAR_COMIDA") == 0)
            tarea_descomprimida->codigoTarea = GENERAR_COMIDA;
        else if (strcmp(tarea[0], "CONSUMIR_COMIDA") == 0)
            tarea_descomprimida->codigoTarea = CONSUMIR_COMIDA;
        else if (strcmp(tarea[0], "GENERAR_BASURA") == 0)
            tarea_descomprimida->codigoTarea = GENERAR_BASURA;
        else if (strcmp(tarea[0], "DESCARTAR_BASURA") == 0)
            tarea_descomprimida->codigoTarea = DESCARTAR_BASURA;

        char **parametros = string_split(tarea, ";");

        tarea_descomprimida->parametro = atoi(parametros[0]);
        tarea_descomprimida->posX = atoi(parametros[1]);
        tarea_descomprimida->posY = atoi(parametros[2]);
        tarea_descomprimida->duracionTarea = atoi(parametros[3]);

    }
    else
    {
        //ES UNA TAREA NORMAL
        char **tarea = string_split(tarea_recibida->tarea, ";");

        *len_tarea = tarea_recibida->tamanio_tarea;
        nom_tarea = malloc(tarea_recibida->tamanio_tarea);
        strcpy(nom_tarea, tarea_recibida->tarea);
        
        tarea_descomprimida->codigoTarea   = MOVER_POSICION;
        tarea_descomprimida->parametro     = 0;
        tarea_descomprimida->posX          = atoi(tarea[1]);
        tarea_descomprimida->posY          = atoi(tarea[2]);
        tarea_descomprimida->duracionTarea = atoi(tarea[3]);
    }

    return tarea_descomprimida;
}