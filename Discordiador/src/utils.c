<<<<<<< HEAD
#include "proceso1.h"

t_tareas* leer_tareas(FILE *fpTareas)
{

    char *line = NULL;
    char **data;
    size_t len = 0;
    ssize_t read;
    int cantLineas = 0, cantInstrucciones = 0, cantParametros = 0;
    char **instruccion = NULL;
    t_tareas *tareas;

    //CHEQUEO LA CANTIDAD DE LINEAS PARA RESERVAR LA MEMORIA
    while ((read = getline(&line, &len, fpTareas)) != -1)
        cantLineas++;
    fseek(fpTareas, 0, SEEK_SET);

    //RESERVO LA MEMORIA
    tareas = malloc(sizeof(t_tareas) * cantLineas);

    //LEO LINEA A LINEA
    for (int i = 0; i < cantLineas; i++)
    {
        if ((read = getline(&line, &len, fpTareas)) != -1)
        {
            instruccion = string_split(line, " ");
            cantInstrucciones = 1 + contar_caracteres_especiales(read, line, ' ');

            if (cantInstrucciones != 2)
            {
                //LA INSTRUCCION ES DESCARTAR BASURA o ES UN ERROR

                data = string_split(instruccion[0], ";");

                if (strcmp(data[0], "DESCARTAR_BASURA") == 0)
                    tareas[i].codigoTarea = DESCARTAR_BASURA;
                else
                    tareas[i].codigoTarea = MOVER_POSICION;

                while (data[cantParametros] != NULL)
                    cantParametros++;

                if (cantParametros == 4)
                {
                    // Ya que estas tareas no tienen parametros
                    tareas[i].parametro = 0;
                    tareas[i].posX = atoi(data[1]);
                    tareas[i].posY = atoi(data[2]);
                    tareas[i].duracionTarea = atoi(data[3]);
                }
                else
                {
                    printf("La instruccion %d no fue reconocida\n", i + 1);
                }
            }
            else
            {
                //LA INSTRUCCION ES "GENERAR_OXIGENO", "CONSUMIR_OXIGENO", "GENERAR_COMIDA", "CONSUMIR_COMIDA" O "GENERAR_BASURA"

                //CARGO EL CODIGO DE TAREA
                if (strcmp(instruccion[0], "GENERAR_OXIGENO") == 0)
                    tareas[i].codigoTarea = GENERAR_OXIGENO;
                else if (strcmp(instruccion[0], "CONSUMIR_OXIGENO") == 0)
                    tareas[i].codigoTarea = CONSUMIR_OXIGENO;
                else if (strcmp(instruccion[0], "GENERAR_COMIDA") == 0)
                    tareas[i].codigoTarea = GENERAR_COMIDA;
                else if (strcmp(instruccion[0], "CONSUMIR_COMIDA") == 0)
                    tareas[i].codigoTarea = CONSUMIR_COMIDA;
                else if (strcmp(instruccion[0], "GENERAR_BASURA") == 0)
                    tareas[i].codigoTarea = GENERAR_BASURA;
                else
                    tareas[i].codigoTarea = MOVER_POSICION;

                data = string_split(instruccion[1], ";");
                while (data[cantParametros] != NULL)
                    cantParametros++;

                //CARGO EL RESTO DE LOS PARAMETROS
                if (cantParametros == 4)
                {
                    tareas[i].parametro = atoi(data[0]);
                    tareas[i].posX = atoi(data[1]);
                    tareas[i].posY = atoi(data[2]);
                    tareas[i].duracionTarea = atoi(data[3]);
                }
                else
                {
                    printf("La instruccion %d no fue reconocida\n", i + 1);
                }
            }
        }
        else
        {
            printf("La instruccion %d no fue reconocida\n", i + 1);
        }

        cantInstrucciones = 0;
        cantParametros = 0;

        free(instruccion);
        free(data);
    }
    
    free(line);

    return tareas;
}

int contar_caracteres_especiales(size_t read, char *line, char caracterEspecial)
{
    int contador = 0;
    for (int j = 0; j < read; ++j)
        if (line[j] == caracterEspecial)
            contador++;

    return contador;
=======
#include "proceso1.h"

t_tarea* leer_tareas(FILE *fpTareas)
{

    char *line = NULL;
    char **data;
    size_t len = 0;
    ssize_t read;
    int cantLineas = 0, cantInstrucciones = 0, cantParametros = 0;
    char **instruccion = NULL;
    t_tarea *tareas;

    //CHEQUEO LA CANTIDAD DE LINEAS PARA RESERVAR LA MEMORIA
    while ((read = getline(&line, &len, fpTareas)) != -1)
        cantLineas++;
    fseek(fpTareas, 0, SEEK_SET);

    //RESERVO LA MEMORIA
    tareas = malloc(sizeof(t_tarea) * cantLineas);

    //LEO LINEA A LINEA
    for (int i = 0; i < cantLineas; i++)
    {
        if ((read = getline(&line, &len, fpTareas)) != -1)
        {
            instruccion = string_split(line, " ");
            cantInstrucciones = 1 + contar_caracteres_especiales(read, line, ' ');

            if (cantInstrucciones != 2)
            {
                //LA INSTRUCCION ES DESCARTAR BASURA o ES UN ERROR

                data = string_split(instruccion[0], ";");

                if (strcmp(data[0], "DESCARTAR_BASURA") == 0)
                    tareas[i].codigoTarea = DESCARTAR_BASURA;
                else
                    tareas[i].codigoTarea = MOVER_POSICION;

                while (data[cantParametros] != NULL)
                    cantParametros++;

                if (cantParametros == 4)
                {
                    // Ya que estas tareas no tienen parametros
                    tareas[i].parametro = 0;
                    tareas[i].posX = atoi(data[1]);
                    tareas[i].posY = atoi(data[2]);
                    tareas[i].duracionTarea = atoi(data[3]);
                }
                else
                {
                    printf("La instruccion %d no fue reconocida\n", i + 1);
                }
            }
            else
            {
                //LA INSTRUCCION ES "GENERAR_OXIGENO", "CONSUMIR_OXIGENO", "GENERAR_COMIDA", "CONSUMIR_COMIDA" O "GENERAR_BASURA"

                //CARGO EL CODIGO DE TAREA
                if (strcmp(instruccion[0], "GENERAR_OXIGENO") == 0)
                    tareas[i].codigoTarea = GENERAR_OXIGENO;
                else if (strcmp(instruccion[0], "CONSUMIR_OXIGENO") == 0)
                    tareas[i].codigoTarea = CONSUMIR_OXIGENO;
                else if (strcmp(instruccion[0], "GENERAR_COMIDA") == 0)
                    tareas[i].codigoTarea = GENERAR_COMIDA;
                else if (strcmp(instruccion[0], "CONSUMIR_COMIDA") == 0)
                    tareas[i].codigoTarea = CONSUMIR_COMIDA;
                else if (strcmp(instruccion[0], "GENERAR_BASURA") == 0)
                    tareas[i].codigoTarea = GENERAR_BASURA;
                else
                    tareas[i].codigoTarea = MOVER_POSICION;

                data = string_split(instruccion[1], ";");
                while (data[cantParametros] != NULL)
                    cantParametros++;

                //CARGO EL RESTO DE LOS PARAMETROS
                if (cantParametros == 4)
                {
                    tareas[i].parametro = atoi(data[0]);
                    tareas[i].posX = atoi(data[1]);
                    tareas[i].posY = atoi(data[2]);
                    tareas[i].duracionTarea = atoi(data[3]);
                }
                else
                {
                    printf("La instruccion %d no fue reconocida\n", i + 1);
                }
            }
        }
        else
        {
            printf("La instruccion %d no fue reconocida\n", i + 1);
        }

        cantInstrucciones = 0;
        cantParametros = 0;

        free(instruccion);
        free(data);
    }
    
    free(line);

    return tareas;
}

int contar_caracteres_especiales(size_t read, char *line, char caracterEspecial)
{
    int contador = 0;
    for (int j = 0; j < read; ++j)
        if (line[j] == caracterEspecial)
            contador++;

    return contador;
>>>>>>> mi-ram-hq
}