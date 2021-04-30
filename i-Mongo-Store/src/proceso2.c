#include "proceso2.h"

int main()
{
    t_store_conf *config = get_store_config("../i-Mongo-Store/cfg/config.cfg");
    printf("En el proceso 2 llega con config: %s\n", config->punto_montaje);
    free(config);
}