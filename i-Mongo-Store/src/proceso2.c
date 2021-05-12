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

    // Escuchar
    printf("Servidor escuchando en puerto %d\n", config->puerto);
    listen(server_instance, 100);

    pthread_t thread_id;
    while (1)
    {
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

        // Crear hilo
        pthread_create(&thread_id, NULL, connection_handler, (void *)&client);
    }

    free(config);

    return EXIT_SUCCESS;
}

void *connection_handler(void *arg)
{
    int client = *(int *)arg;

    printf("Recibí una conexión en el socket %d\n", client);

    char *buffer = malloc(100);
    while (1)
    {
        int msg_size = recv(client, buffer, 100, 0);

        if (msg_size <= 0)
        {
            printf("El cliente del socket %d se desconectó\n", client);
            return EXIT_FAILURE;
        }

        buffer[msg_size] = '\0';
        printf("Socket %d > %s\n", client, buffer);
    }

    free(buffer);
}