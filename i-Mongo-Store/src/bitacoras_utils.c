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
    bitacora_aux->path = string_new();
    string_append(&(bitacora_aux->path), bitacora_file_path);
    t_config *bitacora_config = config_create(bitacora_file_path);
    bitacora_aux->size = config_get_int_value(bitacora_config, "SIZE");
    char **blocks = config_get_array_value(bitacora_config, "BLOCKS");
    bitacora_aux->blocks = list_create();
    for (int i = 0; blocks[i] != NULL; i++)
    {
        list_add(bitacora_aux->blocks, atoi(blocks[i]));
    }
    config_destroy(bitacora_config);

    return bitacora_aux;
}

void update_bitacora(t_bitacora *bitacora)
{
    t_config *bitacora_config = config_create(bitacora->path);

    config_set_value(bitacora_config, "SIZE", string_itoa(bitacora->size));
    config_set_value(bitacora_config, "BLOCKS", blocks_list_to_string(bitacora->blocks));
    config_save(bitacora_config);

    config_destroy(bitacora_config);
}

void registrar_bitacora(t_bitacora *bitacora, char *msg)
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
            last_block = list_get(bitacora->blocks, list_size(bitacora->blocks) - 1);
        }
        else
        {
            last_block = get_free_block();
            set_block(last_block);
            list_add(bitacora->blocks, last_block);
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

    update_bitacora(bitacora);
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
