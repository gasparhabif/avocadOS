#include "proceso3.h"

int main()
{
    t_ram_conf *config = get_ram_config("../Mi-Ram-HQ/cfg/config.cfg");
    printf("En el proceso 3 llega con config: %s\n", config->algoritmo_reemplazo);
    free(config);
}