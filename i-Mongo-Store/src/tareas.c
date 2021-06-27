#include "proceso2.h"

// TAREAS DE ENTRADA/SALIDA
void generarOxigeno(int cantidad)
{
    log_info(logger, "Se solicitó GENERAR_OXIGENO. Cantidad: %d", cantidad);
    // ...
}

void consumirOxigeno(int cantidad)
{
    log_info(logger, "Se solicitó CONSUMIR_OXIGENO. Cantidad: %d", cantidad);
    // ...
}

void generarComida(int cantidad)
{
    log_info(logger, "Se solicitó GENERAR_COMIDA. Cantidad: %d", cantidad);
    // ...
}

void consumirComida(int cantidad)
{
    log_info(logger, "Se solicitó CONSUMIR_COMIDA. Cantidad: %d", cantidad);
    // ...
}

void generarBasura(int cantidad)
{
    log_info(logger, "Se solicitó GENERAR_BASURA. Cantidad: %d", cantidad);
    // ...
}

void descartarBasura()
{
    log_info(logger, "Se solicitó DESCARTAR_BASURA");
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

void ejecutarTarea(t_tarea *tarea_a_ejecutar) {
    switch (tarea_a_ejecutar->codigoTarea)
    {
                case FIN_TAREAS:
                    // TODO: Registrar en bitácora
                    log_info(logger, "El tripulante finalizó sus tareas");
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
                    log_error(logger, "Código de tarea desconocido");
                    break;
    }
}