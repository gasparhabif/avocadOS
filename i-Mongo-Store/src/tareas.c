#include "proceso2.h"

// Selector de tareas
void ejecutarTarea(t_ejecutar_tarea *tarea_a_ejecutar)
{
    switch (tarea_a_ejecutar->codigoTarea)
    {
    case GENERAR_OXIGENO:
        generarOxigeno(tarea_a_ejecutar->parametro);
        break;
    case CONSUMIR_OXIGENO:
        consumirOxigeno(tarea_a_ejecutar->parametro);
        break;
    case GENERAR_COMIDA:
        generarComida(tarea_a_ejecutar->parametro);
        break;
    case CONSUMIR_COMIDA:
        consumirComida(tarea_a_ejecutar->parametro);
        break;
    case GENERAR_BASURA:
        generarBasura(tarea_a_ejecutar->parametro);
        break;
    case DESCARTAR_BASURA:
        descartarBasura();
        break;
    default:
        break;
    }
}

// FIN DE TAREAS
void finTareas()
{
    log_info(logger, "El tripulante finalizó sus tareas");
    // ...
}

// TAREAS DE ENTRADA/SALIDA
void generarOxigeno(int cantidad)
{
    if (!file_exists(oxigeno_file_path))
    {
        create_recurso(oxigeno_file_path, "O");
        log_info(logger, "Se creó Oxigeno.ims");
        last_oxigeno = load_recurso(oxigeno_file_path);
    }

    // t_recurso *oxigeno = load_recurso(oxigeno_file_path);
    // agregar_recurso(oxigeno, cantidad);

    agregar_recurso(last_oxigeno, cantidad);
}

void consumirOxigeno(int cantidad)
{
    if (!file_exists(oxigeno_file_path))
    {
        log_info(logger, "El archivo Oxigeno.ims no existe");
        return;
    }

    // t_recurso *oxigeno = load_recurso(oxigeno_file_path);
    // eliminar_recurso(oxigeno, cantidad);

    eliminar_recurso(last_oxigeno, cantidad);
}

void generarComida(int cantidad)
{
    if (!file_exists(comida_file_path))
    {
        create_recurso(comida_file_path, "C");
        log_info(logger, "Se creó Comida.ims");
        last_comida = load_recurso(comida_file_path);
    }

    // t_recurso *comida = load_recurso(comida_file_path);
    // agregar_recurso(comida, cantidad);

    agregar_recurso(last_comida, cantidad);
}

void consumirComida(int cantidad)
{
    if (!file_exists(comida_file_path))
    {
        log_info(logger, "El archivo Comida.ims no existe");
        return;
    }

    // t_recurso *comida = load_recurso(comida_file_path);
    // eliminar_recurso(comida, cantidad);

    eliminar_recurso(last_comida, cantidad);
}

void generarBasura(int cantidad)
{
    if (!file_exists(basura_file_path))
    {
        create_recurso(basura_file_path, "B");
        log_info(logger, "Se creó Basura.ims");
        last_basura = load_recurso(basura_file_path);
    }

    // t_recurso *basura = load_recurso(basura_file_path);
    // agregar_recurso(basura, cantidad);

    agregar_recurso(last_basura, cantidad);
}

void descartarBasura()
{
    if (!file_exists(basura_file_path))
    {
        log_info(logger, "El archivo Basura.ims no existe");
        return;
    }

    // t_recurso *basura = load_recurso(basura_file_path);
    // eliminar_recurso(basura, basura->size);

    eliminar_recurso(last_basura, last_basura->size);

    // Eliminar archivo
    if (file_exists(basura_file_path))
    {
        remove(basura_file_path);
        liberar_recurso(last_basura);
    }
}
