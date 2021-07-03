#include "proceso2.h"

void create_bitacora(char *bitacora_file_path)
{
    int bitacora_fd = open(bitacora_file_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    t_config *bitacora_config = config_create(bitacora_file_path);
    config_set_value(bitacora_config, "SIZE", "0");
    config_set_value(bitacora_config, "BLOCKS", "[]");
    config_save(bitacora_config);

    config_destroy(bitacora_config);
    close(bitacora_fd);
}

t_bitacora *load_bitacora(char *bitacora_file_path)
{
    t_bitacora *bitacora_aux = malloc(sizeof(t_bitacora));

    t_config *bitacora_config = config_create(bitacora_file_path);
    bitacora_aux->size = config_get_int_value(bitacora_config, "SIZE");
    char *bitacora_blocks_aux = config_get_string_value(bitacora_config, "BLOCKS");
    bitacora_aux->blocks = string_new();
    strcpy(bitacora_aux->blocks, bitacora_blocks_aux);
    config_destroy(bitacora_config);

    return bitacora_aux;
}