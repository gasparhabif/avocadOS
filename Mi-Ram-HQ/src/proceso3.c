#include "proceso3.h"

int main()
{
    t_log *logger = log_create("./logs/proceso3.log", "PROCESO3", true, LOG_LEVEL_INFO);
    log_info(logger, "Soy el proceso 3! %s", mi_funcion_compartida());
    log_destroy(logger);
}