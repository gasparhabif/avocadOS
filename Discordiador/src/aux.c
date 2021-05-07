#include "proceso1.h"

void leer_tareas(t_tareas *tareas, FILE *fpTareas){

    char *line = NULL;
    char **instruccion;
    char **data;
    size_t len = 0;
    ssize_t read;
    int cantLineas = 0;

    //CHEQUEO LA CANTIDAD DE LINEAS PARA RESERVAR LA MEMORIA
    while ((read = getline(&line, &len, fpTareas)) != -1)
        cantLineas++;
    fseek(fpTareas, 0, SEEK_SET);

    //RESERVO LA MEMORIA
    tareas = malloc(cantLineas * sizeof(t_tareas));

    //LEO LINEA A LINEA
    for(int i = 0; i < cantLineas; i++){
        if((read = getline(&line, &len, fpTareas)) != -1){

            instruccion = string_split(line, " ");

            if (strcmp(instruccion[0],      "GENERAR_OXIGENO" ) == 0) 
                tareas[i].codigoTarea = GENERAR_OXIGENO;
            else if (strcmp(instruccion[0], "CONSUMIR_OXIGENO") == 0)
                tareas[i].codigoTarea = CONSUMIR_OXIGENO;
            else if (strcmp(instruccion[0], "GENERAR_COMIDA"  ) == 0) 
                tareas[i].codigoTarea = GENERAR_COMIDA;
            else if (strcmp(instruccion[0], "CONSUMIR_COMIDA" ) == 0)
                tareas[i].codigoTarea = CONSUMIR_COMIDA;
            else if (strcmp(instruccion[0], "GENERAR_BASURA"  ) == 0) 
                tareas[i].codigoTarea = GENERAR_BASURA;
            else if (strcmp(instruccion[0], "DESCARTAR_BASURA") == 0)
                tareas[i].codigoTarea = DESCARTAR_BASURA;
            else{
                tareas[i].codigoTarea = MOVER_POSICION;
            }

            data = string_split(instruccion[1], ";");

            tareas[i].parametro     = (u_int32_t) data[0];
            tareas[i].posX          = (u_int32_t) data[1];
            tareas[i].posY          = (u_int32_t) data[2];
            tareas[i].duracionTarea = (u_int32_t) data[3];

        }
        else{

        }
    }

    free(line);
} 