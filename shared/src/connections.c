#include "connections.h"

int connect_to(char *ip, int port)
{
    struct sockaddr_in server_dir;
    server_dir.sin_family = AF_INET;
    server_dir.sin_addr.s_addr = inet_addr(ip);
    server_dir.sin_port = htons(port);

    int client = socket(AF_INET, SOCK_STREAM, 0);
    int connection = connect(client, (void *)&server_dir, sizeof(server_dir));

    return connection == 0 ? client : connection;
}