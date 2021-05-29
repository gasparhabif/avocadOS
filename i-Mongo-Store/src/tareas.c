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