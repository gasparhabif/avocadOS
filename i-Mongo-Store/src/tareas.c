#include "proceso2.h"

// Selector de tareas
void ejecutarTarea(t_tarea *tarea_a_ejecutar)
{
    switch (tarea_a_ejecutar->codigoTarea)
    {
    case FIN_TAREAS:
        finTareas();
        break;
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
        tareaNormal();
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
    }

    t_recurso *oxigeno = load_recurso(oxigeno_file_path);
    log_info(logger, "Se cargó Oxigeno.ims");
    agregar_recurso(oxigeno, cantidad);

    // TODO: Liberar recurso
    // ...
}

void consumirOxigeno(int cantidad)
{
    if (!file_exists(oxigeno_file_path))
    {
        log_info(logger, "El archivo Oxigeno.ims no existe");
        return;
    }

    t_recurso *oxigeno = load_recurso(oxigeno_file_path);
    log_info(logger, "Se cargó Oxigeno.ims");
    eliminar_recurso(oxigeno, cantidad);

    // TODO: Liberar recurso
    // ...
}

void generarComida(int cantidad)
{
    if (!file_exists(comida_file_path))
    {
        create_recurso(comida_file_path, "C");
        log_info(logger, "Se creó Comida.ims");
    }

    t_recurso *comida = load_recurso(comida_file_path);
    log_info(logger, "Se cargó Comida.ims");
    agregar_recurso(comida, cantidad);

    // TODO: Liberar recurso
    // ...
}

void consumirComida(int cantidad)
{
    if (!file_exists(comida_file_path))
    {
        log_info(logger, "El archivo Comida.ims no existe");
        return;
    }

    t_recurso *comida = load_recurso(comida_file_path);
    log_info(logger, "Se cargó Comida.ims");
    eliminar_recurso(comida, cantidad);

    // TODO: Liberar recurso
    // ...
}

void generarBasura(int cantidad)
{
    if (!file_exists(basura_file_path))
    {
        create_recurso(basura_file_path, "B");
        log_info(logger, "Se creó Basura.ims");
    }

    t_recurso *basura = load_recurso(basura_file_path);
    log_info(logger, "Se cargó Basura.ims");
    agregar_recurso(basura, cantidad);

    // TODO: Liberar recurso
    // ...
}

void descartarBasura()
{
    if (!file_exists(basura_file_path))
    {
        log_info(logger, "El archivo Basura.ims no existe");
        return;
    }

    t_recurso *basura = load_recurso(basura_file_path);
    log_info(logger, "Se cargó Basura.ims");
    eliminar_recurso(basura, basura->size);
    remove(basura->path);

    // TODO: Liberar recurso
    // ...
}

// TAREA NORMAL
void tareaNormal()
{
    log_info(logger, "Se solicitó realizar una tarea normal");
    // ...
}

// REGISTRO DE BITÁCORA
void registrarDesplazamiento()
{
    log_info(logger, "Registrando DESPLAZAMIENTO...");
    // ...
}

void registrarInicioTarea()
{
    log_info(logger, "Registrando INICIO DE TAREA...");
    // ...
}

void registrarFinTarea()
{
    log_info(logger, "Registrando FIN DE TAREA...");
    // ...
}

void registrarAtencionSabotaje()
{
    log_info(logger, "Registrando ATENCIÓN DE SABOTAJE...");
    // ...
}

void registrarResolucionSabotaje()
{
    log_info(logger, "Registrando RESOLUCIÓN DE SABOTAJE...");
    // ...
}
