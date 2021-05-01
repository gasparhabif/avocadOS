#include "proceso2.h"

int main()
{
    t_store_conf *config = get_store_config("../i-Mongo-Store/cfg/config.cfg");
    printf("PUNTO_MONTAJE: %s\nPUERTO: %d\nTIEMPO_SINCRONIZACION: %d\n", config->punto_montaje, config->puerto, config->tiempo_sincronizacion);
    free(config);
}