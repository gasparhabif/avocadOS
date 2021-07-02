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