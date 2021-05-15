#include "proceso3.h"

int main()
{
    iniciar_logger();

    t_ram_conf *config = get_ram_config("../Mi-Ram-HQ/cfg/config.cfg");
    log_info(logger, "Se cargo la config de la RAM");
    memoria = malloc(config->tamanio_memoria);

    int server_instance;
    if ((server_instance = init_server(config->puerto)) == -1)
    {
        log_error(logger, "Algo malió sal al iniciar el servidor");
        return 1;
    }

    log_info(logger, "Servidor iniciado");
    listen(server_instance, 100);

    struct sockaddr_in client_dir;
    unsigned int dir_size = sizeof(socklen_t);
    int client;

    while(1){
        if ((client = accept(server_instance, (void *)&client_dir, &dir_size)) == -1)
        {
            log_error(logger, "Algo malió sal al aceptar al cliente");
            return 1;
        }
        
        log_info(logger, "Recibí una conexión en el socket %d", client);
        //char *buffer = malloc(100);

        //int msg_size = recv(client, buffer, 100, 0);
        printf("Recibiendo...\n");
        t_tareas_cPID *tareas_cPID_recibidas = recibir_paquete(client);
        //printf("Recibi %d bytes\n", sizeof(tareas_cPID_recibidas));

        printf("INICIO DE PATOTA CON PID: %d\n", tareas_cPID_recibidas->PID);
        for(int i=0; i<tareas_cPID_recibidas->cantTareas; i++){
            printf("-----TAREA %d-----\n", i+1);
            printf("CODT: %d\n", tareas_cPID_recibidas->tareas[i].codigoTarea);
            printf("PARA: %d\n", tareas_cPID_recibidas->tareas[i].parametro);
            printf("POSX: %d\n", tareas_cPID_recibidas->tareas[i].posX);
            printf("POSY: %d\n", tareas_cPID_recibidas->tareas[i].posY);
            printf("DURA: %d\n", tareas_cPID_recibidas->tareas[i].duracionTarea);
            printf("------------------\n\n");
        }

        //log_info(logger, "Recibi una patota con PID: %d", tTareas->PID);
        //void* paquete = serializarInt(tTareas->PID, PUNTERO_PCB);
        //send(client, paquete, sizeof(paquete), 0);

        //free(buffer);
        free(config);
    }
}

void iniciar_logger()
{
    logger = log_create("../Mi-Ram-HQ/logs/proceso3.log", "Mi-Ram-HQ", 1, LOG_LEVEL_INFO);
	log_info(logger, "Se inicio el log del Proceso 3 [Mi-Ram-HQ]");
}