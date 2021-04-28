#include "proceso1.h"

int main()
{
    t_cpu_conf *config = get_cpu_config("../cfg/config.cfg");
    printf("Soy el proceso 1! %s", config->puerto_mongo);
    // printf("Soy el proceso 1! %s", &config.ip_mongo);
    free(config);
}