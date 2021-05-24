#include "proceso2.h"

int main()
{
    system("clear");
    t_store_conf *config = get_store_config("../i-Mongo-Store/cfg/config.cfg");

    int server_instance = init_server(config->puerto);

    // Verificar instancia
    if (server_instance == -1)
    {
        perror("Algo malió sal");
        return EXIT_FAILURE;
    }

    // Aceptar conexiones de los tripulantes
    printf("Servidor escuchando en puerto %d\n", config->puerto);
    pthread_t accept_connections_thread;
    pthread_create(&accept_connections_thread, NULL, (void *)accept_connections, (void *)server_instance);
    pthread_join(accept_connections_thread, NULL);

    // ...

    free(config);

    return EXIT_SUCCESS;
}

void accept_connections(void *arg)
{
    int server_instance = (int)arg;

    while (1)
    {
        listen(server_instance, SOMAXCONN);

        // Aceptar conexión
        struct sockaddr_in client_dir;
        unsigned int dir_size = sizeof(socklen_t);
        int client = accept(server_instance, (void *)&client_dir, &dir_size);

        // Verificar conexión recibida
        if (client == -1)
        {
            perror("Error al aceptar conexión");
            break;
        }

        pthread_t connection_thread;
        pthread_create(&connection_thread, NULL, (void *)connection_handler, (void *)client);
        // Acá no ejecuto pthread_join() porque sino no crea nuevos threads hasta que no termine el último
    }
}

void connection_handler(void *arg)
{
    int client = (int)arg;

    printf("Recibí una conexión en el socket %d\n", client);

    while (1)
    {
    }
}