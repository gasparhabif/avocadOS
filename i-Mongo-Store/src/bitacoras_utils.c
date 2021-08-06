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

t_bitacora_mongo *load_bitacora(char *bitacora_file_path)
{
    t_bitacora_mongo *bitacora_aux = malloc(sizeof(t_bitacora_mongo));
    bitacora_aux->path = string_new();
    string_append(&(bitacora_aux->path), bitacora_file_path);
    t_config *bitacora_config = config_create(bitacora_file_path);
    bitacora_aux->size = config_get_int_value(bitacora_config, "SIZE");
    char **blocks = config_get_array_value(bitacora_config, "BLOCKS");
    bitacora_aux->blocks = list_create();
    for (int i = 0; blocks[i] != NULL; i++)
    {
        list_add(bitacora_aux->blocks, (void *)atoi(blocks[i]));
    }
    config_destroy(bitacora_config);

    return bitacora_aux;
}

void update_bitacora_metadata(t_bitacora_mongo *bitacora)
{
    t_config *bitacora_config = config_create(bitacora->path);

    config_set_value(bitacora_config, "SIZE", string_itoa(bitacora->size));
    char *blocks_aux = blocks_list_to_string(bitacora->blocks);
    config_set_value(bitacora_config, "BLOCKS", blocks_aux);
    config_save(bitacora_config);

    config_destroy(bitacora_config);
    free(blocks_aux);
}

void liberar_bitacora(t_bitacora_mongo *bitacora)
{
    free(bitacora->path);
    list_destroy(bitacora->blocks);
    free(bitacora);
}

void registrar_bitacora(t_bitacora_mongo *bitacora, char *msg)
{
    int bytes_to_write = strlen(msg);
    int next_byte_to_write = 0;

    while (bytes_to_write != 0)
    {
        int last_block;
        int last_block_offset = bitacora->size % superbloque->block_size;
        int free_bytes_last_block = superbloque->block_size - last_block_offset;

        if (!list_is_empty(bitacora->blocks) && last_block_offset > 0)
        {
            last_block = (int)list_get(bitacora->blocks, list_size(bitacora->blocks) - 1);
        }
        else
        {
            last_block = get_free_block();
            set_block(last_block);
            list_add(bitacora->blocks, (void *)last_block);
        }

        int blocks_file_offset = last_block * superbloque->block_size + last_block_offset;

        for (int i = 0; i < free_bytes_last_block && bytes_to_write > 0; i++)
        {
            blocks_file_copy[blocks_file_offset + i] = msg[next_byte_to_write];
            next_byte_to_write++;
            bytes_to_write--;
            bitacora->size++;
        }
    }

    update_bitacora_metadata(bitacora);
    log_info(logger, msg);
}

char *blocks_list_to_string(t_list *blocks_list)
{
    char *blocks_string = string_new();
    string_append(&blocks_string, "[");
    for (int i = 0; i < list_size(blocks_list); i++)
    {
        char *list_item = string_itoa((int)list_get(blocks_list, i));
        i == 0 ? string_append(&blocks_string, list_item) : string_append_with_format(&blocks_string, ",%s", list_item);
    }
    string_append(&blocks_string, "]");

    return blocks_string;
}

char *pos_to_string(t_posicion *pos)
{
    return string_from_format("%s|%s", string_itoa(pos->posX), string_itoa(pos->posY));
}

void update_pos(t_posicion *current_pos, t_posicion *next_pos)
{
    current_pos->posX = next_pos->posX;
    current_pos->posY = next_pos->posY;
}

char *reconstruir_bitacora(t_bitacora_mongo *bitacora)
{
    char *bitacora_string = string_new();
    char *block_info = malloc(superbloque->block_size);

    for (int i = 0; i < list_size(bitacora->blocks); i++)
    {
        memcpy(block_info, blocks_file_copy + (int)list_get(bitacora->blocks, i) * superbloque->block_size, superbloque->block_size);
        string_append(&bitacora_string, string_substring_until(block_info, superbloque->block_size));
    }

    free(block_info);
    return string_substring_until(bitacora_string, bitacora->size - 1);
}