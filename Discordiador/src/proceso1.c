#include "proceso1.h"

int main()
{
    t_cpu_conf *config = get_cpu_config("../Discordiador/cfg/config.cfg");
    printf("En el proceso 1 llega: %i", config->ip_ram);
    free(config);
}