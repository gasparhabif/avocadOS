#include "proceso2.h"

void generarOxigeno(int cantidad)
{
    log_info(logger, "Se solicitó GENERAR_OXIGENO. Cantidad: %d\n", cantidad);
    // ...
}

void consumirOxigeno(int cantidad)
{
    log_info(logger, "Se solicitó CONSUMIR_OXIGENO. Cantidad: %d\n", cantidad);
    // ...
}

void generarComida(int cantidad)
{
    log_info(logger, "Se solicitó GENERAR_COMIDA. Cantidad: %d\n", cantidad);
    // ...
}

void consumirComida(int cantidad)
{
    log_info(logger, "Se solicitó CONSUMIR_COMIDA. Cantidad: %d\n", cantidad);
    // ...
}

void generarBasura(int cantidad)
{
    log_info(logger, "Se solicitó GENERAR_BASURA. Cantidad: %d\n", cantidad);
    // ...
}

void descartarBasura()
{
    log_info(logger, "Se solicitó DESCARTAR_BASURA\n");
    // ...
}