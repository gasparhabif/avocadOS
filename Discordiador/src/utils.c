#include "proceso1.h"

t_tarea* leer_tareas(FILE *fpTareas, int *cantTareas){
    
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int cantLineas = 0;
    t_tarea *tareas;

    //CHEQUEO LA CANTIDAD DE LINEAS PARA RESERVAR LA MEMORIA
    while ((read = getline(&line, &len, fpTareas)) != -1)
        cantLineas++;

    *cantTareas = cantLineas;
    fseek(fpTareas, 0, SEEK_SET);

    //RESERVO LA MEMORIA
    tareas = malloc(sizeof(t_tarea) * cantLineas);

    //LEO LINEA A LINEA
    for (int i = 0; i < cantLineas; i++)
    {
        if ((read = getline(&line, &len, fpTareas)) != -1)
        {
            tareas[i].tamanio_tarea = strlen(line);
            tareas[i].tarea = malloc(strlen(line));
            strcpy(tareas[i].tarea, line);
        }
    }

    for (int i = 0; i < cantLineas; i++)
    {
        printf("Tarea N°%d\n", i+1);
        printf("Len tarea: %d\n", tareas[i].tamanio_tarea);
        printf("Tarea: %s\n", tareas[i].tarea);
    }

    return tareas;
}

/*
t_tarea *leer_tareas(FILE *fpTareas, int *cantTareas, int *error)
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

    *cantTareas = cantLineas;
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

            if (cantInstrucciones == 2)
            {
                //ES UNA TAREA DE E/S

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
                else if (strcmp(instruccion[0], "DESCARTAR_BASURA") == 0)
                    tareas[i].codigoTarea = DESCARTAR_BASURA;
                else
                {
                    //INSTRUCCION NO RECONOCIDA
                    log_info(logger, "Tarea %d no reconocida\n", i + 1);
                    *error = 1;
                    free(instruccion);
                    free(data);
                    free(line);
                    return tareas;
                }

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
                    //FALTAN PARAMETROS
                    log_info(logger, "Faltan parametros en la instruccion %d\n", i + 1);
                    *error = 1;
                    free(instruccion);
                    free(data);
                    free(line);
                    return tareas;
                }
            }
            else
            {
                //ES UNA TAREA NORMAL

                data = string_split(instruccion[0], ";");

                while (data[cantParametros] != NULL)
                    cantParametros++;

                if (cantParametros == 4)
                {
                    tareas[i].codigoTarea = MOVER_POSICION;
                    // Ya que estas tareas no tienen parametros
                    tareas[i].parametro = 0;
                    tareas[i].posX = atoi(data[1]);
                    tareas[i].posY = atoi(data[2]);
                    tareas[i].duracionTarea = atoi(data[3]);
                }
                else
                {
                    //INSTRUCCION NO RECONOCIDA, FALTAN PARAMETROS
                    log_info(logger, "Faltan parametros en la instruccion %d\n", i + 1);
                    *error = 1;
                    free(instruccion);
                    free(data);
                    free(line);
                    return tareas;
                }
            }
        }

        cantInstrucciones = 0;
        cantParametros = 0;

        free(instruccion);
        free(data);
    }

    free(line);

    // for (int i = 0; i < *cantTareas; i++)
    // {
    //     printf("---------Tarea: %d---------\n", i + 1);
    //     printf("CODI: %d\n", tareas[i].codigoTarea);
    //     printf("PARA: %d\n", tareas[i].parametro);
    //     printf("POSX: %d\n", tareas[i].posX);
    //     printf("POSY: %d\n", tareas[i].posY);
    //     printf("DURA: %d\n", tareas[i].duracionTarea);
    //     printf("---------------------------\n\n");
    // }

    return tareas;
}
*/

t_tarea_descomprimida* descomprimir_tarea(t_tarea* tarea_recibida, int* len_tarea, char* nom_tarea){

    t_tarea_descomprimida *tarea_descomprimida = malloc(sizeof(t_tarea_descomprimida));
    
    if(tarea_recibida->tamanio_tarea == -1){

        tarea_descomprimida->codigoTarea   = FIN_TAREAS;
        tarea_descomprimida->duracionTarea = 0;
        tarea_descomprimida->parametro     = 0;
        tarea_descomprimida->posX          = 0;
        tarea_descomprimida->posY          = 0;
    }
    else{

        int cantEspacios = 0;

        for (int i = 0; i < strlen(tarea_recibida->tarea); i++)
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

            char **parametros = string_split(tarea[1], ";");

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
    }

    return tarea_descomprimida;
}


int contar_caracteres_especiales(size_t read, char *line, char caracterEspecial)
{
    int contador = 0;
    for (int j = 0; j < read; ++j)
        if (line[j] == caracterEspecial)
            contador++;

    return contador;
}

int pausar(int pausarPlanificacion)
{

    if (planificando == 0 && pausarPlanificacion == 0)
    {
        for (int i = 0; i < config->grado_multitarea; i++)
        {
            sem_post(&pause_exec);
        }
        pthread_mutex_unlock(&pause_block);
        pausar_tripulantes(0);

        planificando = 1;
        return 1;
    }
    else if (planificando && pausarPlanificacion)
    {
        for (int i = 0; i < config->grado_multitarea; i++)
        {
            sem_wait(&pause_exec);
        }
        pthread_mutex_lock(&pause_block);
        pausar_tripulantes(1);
        ;

        planificando = 0;
        return 1;
    }
    else
    {
        return 0;
        printf("Se invoco al pausar erroneamente");
    }
}

void pausar_tripulantes(int hayQuePausar)
{
    pausar_lista(exec, hayQuePausar);
    pausar_lista(ready, hayQuePausar);
    pausar_lista(bloq, hayQuePausar);
    pausar_lista(bloq_IO, hayQuePausar);

    return;
}

void pausar_lista(t_list *lista, int hayQuePausar)
{

    t_admin_tripulantes *aux_admin;

    for (int i = 0; i < list_size(lista); i++)
    {
        aux_admin = list_get(lista, i);

        if (hayQuePausar)
        {
            pthread_mutex_trylock(&(aux_admin->pausar_tripulante));
        }
        else
            pthread_mutex_unlock(&(aux_admin->pausar_tripulante));
    }

    return;
}

int eliminarTripulante(t_list *lista, int tid)
{

    t_admin_tripulantes *aux_admin;

    for (int i = 0; i < list_size(lista); i++)
    {
        aux_admin = list_get(lista, i);
        if (aux_admin->tid == tid)
        {
            list_remove(lista, i);
            return 1;
        }
    }

    return 0;
}

int menor_tid_list(t_list *lista)
{

    t_admin_tripulantes *aux_admin;

    int tid_minimo;
    int index = 0;

    aux_admin = list_get(lista, 0);
    tid_minimo = aux_admin->tid;

    for (int i = 0; i < list_size(lista); i++)
    {
        aux_admin = list_get(lista, i);
        if (aux_admin->tid < tid_minimo)
        {
            index = i;
            tid_minimo = aux_admin->tid;
        }
    }

    return index;
}

int mayor_tid_list(t_list *lista)
{

    t_admin_tripulantes *aux_admin;

    int tid_maximo = -1;
    int index;

    for (int i = 0; i < list_size(lista); i++)
    {
        aux_admin = list_get(lista, i);
        if (aux_admin->tid > tid_maximo)
        {
            index = i;
            tid_maximo = aux_admin->tid;
        }
    }

    return index;
}

int matarTripulante(int tid)
{

    t_admin_tripulantes *aux_admin;

    for (int i = 0; i < list_size(ready); i++)
    {
        aux_admin = list_get(ready, i);
        if (aux_admin->tid == tid)
        {
            list_remove(ready, i);
            return 1;
        }
    }

    for (int i = 0; i < list_size(exec); i++)
    {
        aux_admin = list_get(exec, i);
        if (aux_admin->tid == tid)
        {
            list_remove(exec, i);
            return 1;
        }
    }

    for (int i = 0; i < list_size(bloq_IO); i++)
    {
        aux_admin = list_get(bloq_IO, i);
        if (aux_admin->tid == tid)
        {
            list_remove(bloq_IO, i);
            return 1;
        }
    }

    return 0;
}

int existeTripulante(int tid)
{

    t_admin_tripulantes *aux_admin;

    for (int i = 0; i < list_size(ready); i++)
    {
        aux_admin = list_get(ready, i);
        if (aux_admin->tid == tid)
            //free(aux_admin);
            return 1;
    }

    for (int i = 0; i < list_size(exec); i++)
    {
        aux_admin = list_get(exec, i);
        if (aux_admin->tid == tid)
            //free(aux_admin);
            return 1;
    }

    for (int i = 0; i < list_size(bloq_IO); i++)
    {
        aux_admin = list_get(bloq_IO, i);
        if (aux_admin->tid == tid)
            //free(aux_admin);
            return 1;
    }

    return 0;
}

int avisoDeMuerte(int tid)
{

    if (revisarLista_avisoDeMuerte(ready, tid))
        return 1;
    else if (revisarLista_avisoDeMuerte(exec, tid))
        return 1;
    else if (revisarLista_avisoDeMuerte(bloq, tid))
        return 1;
    else if (revisarLista_avisoDeMuerte(bloq_IO, tid))
        return 1;
    else
        return 0;
}

int revisarLista_avisoDeMuerte(t_list *lista, int tid)
{

    t_admin_tripulantes *aux_admin;

    for (int i = 0; i < list_size(lista); i++)
    {
        aux_admin = list_get(lista, i);
        if (aux_admin->tid == tid)
        {
            aux_admin->debeMorir = 1;
            return 1;
        }
    }

    return 0;
}