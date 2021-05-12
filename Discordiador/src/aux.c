#include "proceso1.h"

void leer_tareas(t_tareas **tareas, FILE *fpTareas)
{

    char *line = NULL;
    char **data;
    size_t len = 0;
    ssize_t read;
    int cantLineas = 0, cantInstrucciones = 0, cantParametros = 0;
    char **instruccion = NULL;

    //CHEQUEO LA CANTIDAD DE LINEAS PARA RESERVAR LA MEMORIA
    while ((read = getline(&line, &len, fpTareas)) != -1)
        cantLineas++;
    fseek(fpTareas, 0, SEEK_SET);

    //RESERVO LA MEMORIA
    for (int m = 0; m < cantLineas; ++m)
        tareas[m] = malloc(sizeof(t_tareas));

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
                    tareas[i]->codigoTarea = DESCARTAR_BASURA;
                else
                    tareas[i]->codigoTarea = MOVER_POSICION;

                while (data[cantParametros] != NULL)
                    cantParametros++;

                if (cantParametros == 4)
                {
                    // Ya que estas tareas no tienen parametros
                    tareas[i]->parametro = 0;
                    tareas[i]->posX = atoi(data[1]);
                    tareas[i]->posY = atoi(data[2]);
                    tareas[i]->duracionTarea = atoi(data[3]);
                }
                else
                {
                    printf("La instruccion %d no fue reconocida\n", i + 1);
                }
            }
            else
            {
                if (strcmp(instruccion[0], "GENERAR_OXIGENO") == 0)
                    tareas[i]->codigoTarea = GENERAR_OXIGENO;
                else if (strcmp(instruccion[0], "CONSUMIR_OXIGENO") == 0)
                    tareas[i]->codigoTarea = CONSUMIR_OXIGENO;
                else if (strcmp(instruccion[0], "GENERAR_COMIDA") == 0)
                    tareas[i]->codigoTarea = GENERAR_COMIDA;
                else if (strcmp(instruccion[0], "CONSUMIR_COMIDA") == 0)
                    tareas[i]->codigoTarea = CONSUMIR_COMIDA;
                else if (strcmp(instruccion[0], "GENERAR_BASURA") == 0)
                    tareas[i]->codigoTarea = GENERAR_BASURA;
                else
                    tareas[i]->codigoTarea = MOVER_POSICION;

                data = string_split(instruccion[1], ";");
                while (data[cantParametros] != NULL)
                    cantParametros++;

                if (cantParametros == 4)
                {
                    tareas[i]->parametro = atoi(data[0]);
                    tareas[i]->posX = atoi(data[1]);
                    tareas[i]->posY = atoi(data[2]);
                    tareas[i]->duracionTarea = atoi(data[3]);
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

        free(instruccion);
        free(data);
    }

    for (int i = 0; i < 6; i++)
    {
        printf("INST: %d\n", tareas[i]->codigoTarea);
        printf("PARA: %d\n", tareas[i]->parametro);
        printf("POSX: %d\n", tareas[i]->posX);
        printf("POSY: %d\n", tareas[i]->posY);
        printf("TIME: %d\n\n", tareas[i]->duracionTarea);
    }
    printf("\n++++++++++++++++++++++++\n");

    fflush(fpTareas);
    fclose(fpTareas);
    free(line);
}

int contar_caracteres_especiales(size_t read, char *line, char caracterEspecial)
{
    int contador = 0;
    for (int j = 0; j < read; ++j)
        if (line[j] == caracterEspecial)
            contador++;

    return contador;
}