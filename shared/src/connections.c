#include "connections.h"

int connect_to(char *ip, int port)
{
    struct sockaddr_in server_dir;
    server_dir.sin_family = AF_INET;
    server_dir.sin_addr.s_addr = inet_addr(ip);
    server_dir.sin_port = htons(port);

    int client = socket(AF_INET, SOCK_STREAM, 0);
    int connection = connect(client, (void *)&server_dir, sizeof(server_dir));

    // TODO: Hacer close en caso de error
    return connection == 0 ? client : connection;
}

int init_server(int port)
{
    struct sockaddr_in server_dir;
    server_dir.sin_family = AF_INET;
    server_dir.sin_addr.s_addr = INADDR_ANY;
    server_dir.sin_port = htons(port);

    int server = socket(AF_INET, SOCK_STREAM, 0);
    int flag = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    int boundary = bind(server, (void *)&server_dir, sizeof(server_dir));

    return boundary == 0 ? server : boundary;
}

void *recibir_paquete(int socket)
{
    int codOp;
    return recibir_paquete_cCOP(socket, &codOp);
}

void *recibir_paquete_cCOP(int sockfd, int *codigo_operacion)
{
    t_paquete *paquete = malloc(sizeof(t_paquete));
    paquete->buffer = malloc(sizeof(t_buffer));

    //RECIBO EL CODIGO DE OPERACION
    if (recv(sockfd, &(paquete->codigo_operacion), sizeof(uint8_t), MSG_WAITALL) <= 0)
    {
        return CLIENT_DISCONNECTED;
    }
    //printf("Recibido el COP: %d\n", paquete->codigo_operacion);
    *codigo_operacion = paquete->codigo_operacion;
    //RECIBO EL TAMAÑO DEL STREAM
    recv(sockfd, &(paquete->buffer->size), sizeof(uint32_t), MSG_WAITALL);
    //printf("Recibido el buffer->size %zu\n", paquete->buffer->size);
    //RESERVO LA MEMORIA PARA RECIBIR AL STREAM
    paquete->buffer->stream = malloc(paquete->buffer->size);
    //RECIBO EL STREAM
    recv(sockfd, paquete->buffer->stream, paquete->buffer->size, MSG_WAITALL);
    //printf("Recibi %d bytes de stream\n", bStream);

    void *dRecibidos;

    switch (paquete->codigo_operacion)
    {
    case COMENZAR_PATOTA:
        dRecibidos = deserializarTareas_cPID(paquete->buffer);
        break;
    case PUNTERO_PCB:
        dRecibidos = deserializarInt(paquete->buffer);
        break;
    case INICIAR_TRIPULANTE:
        dRecibidos = deserializarTCB(paquete->buffer);
        break;
    case SOLICITAR_TAREA:
        dRecibidos = deserializarInt(paquete->buffer);
        break;
    case ENVIAR_PROXIMA_TAREA:
        dRecibidos = deserializarTarea(paquete->buffer);
        break;
    case ACTUALIZAR_ESTADO:
        dRecibidos = deserializar_ActulizacionEstado(paquete->buffer);
        break;
    case MOVER_TRIPULANTE:
        dRecibidos = deserializar_envioPosicion(paquete->buffer);
        break;
    default:
        //NO ENCONTRE NINGUN COP
        break;
    }

    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);

    return dRecibidos;
}