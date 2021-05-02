#include "proceso1.h"

int main()
{
    t_cpu_conf *config = get_cpu_config("../Discordiador/cfg/config.cfg");

    int store_connection = connect_to(config->ip_mongo, config->puerto_mongo);

    // Verificar conexión
    if (store_connection == -1)
    {
        perror("Algo malió sal");
        return 1;
    }

    while (1)
    {
        char msg[100];
        scanf("%s", msg);
        send(store_connection, msg, strlen(msg), 0);
    }

    return 0;
}