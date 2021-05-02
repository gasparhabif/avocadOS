#include "proceso2.h"

int main()
{
    t_store_conf *config = get_store_config("../i-Mongo-Store/cfg/config.cfg");

    // Inicializar i-Mongo-Store Server
    struct sockaddr_in server_dir;
    server_dir.sin_family = AF_INET;
    server_dir.sin_addr.s_addr = INADDR_ANY;
    server_dir.sin_port = htons(config->puerto);

    // Crear socket
    int server = socket(AF_INET, SOCK_STREAM, 0);

    // Fix
    int flag = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

    // Verificar socket
    if (bind(server, (void *)&server_dir, sizeof(server_dir)) != 0)
    {
        perror("Algo malió sal");
        return 1;
    }

    // Escuchar
    printf("Servidor escuchando en puerto %d\n", config->puerto);
    listen(server, 100);

    // Aceptar conexión
    struct sockaddr_in client_dir;
    unsigned int dir_size;
    int client = accept(server, (void *)&client_dir, &dir_size);

    // Verificar conexión recibida
    if (client == -1)
    {
        perror("Algo malió sal");
        return 1;
    }

    printf("Recibí una conexión en el socket %d\n", client);
    send(client, "Hola, cliente!", 15, 0);

    char *buffer = malloc(100);

    while (1)
    {
        int msg_size = recv(client, buffer, 100, 0);

        if (msg_size <= 0)
        {
            perror("Algo malió sal");
            return 1;
        }

        buffer[msg_size] = '\0';
        printf("Recibí %d bytes:\n> %s\n", msg_size, buffer);
    }

    free(buffer);
    free(config);

    return 0;
}