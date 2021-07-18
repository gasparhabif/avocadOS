#include "proceso2.h"

void create_recurso(char *recurso_file_path, char *caracter_llenado)
{
    int recurso_fd = open(recurso_file_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    t_config *recurso_config = config_create(recurso_file_path);
    config_set_value(recurso_config, "SIZE", "0");
    config_set_value(recurso_config, "BLOCK_COUNT", "0");
    config_set_value(recurso_config, "BLOCKS", "[]");
    config_set_value(recurso_config, "CARACTER_LLENADO", caracter_llenado);

    // TODO: Generar MD5 por primera vez
    // ...

    config_set_value(recurso_config, "MD5_ARCHIVO", "");
    config_save(recurso_config);

    config_destroy(recurso_config);
    close(recurso_fd);
}

t_recurso *load_recurso(char *recurso_file_path)
{

    t_recurso *recurso_aux = malloc(sizeof(t_recurso));
    recurso_aux->path = string_new();
    string_append(&(recurso_aux->path), recurso_file_path);
    t_config *recurso_config = config_create(recurso_file_path);
    recurso_aux->size = config_get_int_value(recurso_config, "SIZE");
    recurso_aux->block_count = config_get_int_value(recurso_config, "BLOCK_COUNT");
    char **blocks = config_get_array_value(recurso_config, "BLOCKS");
    recurso_aux->blocks = list_create();
    for (int i = 0; blocks[i] != NULL; i++)
    {
        list_add(recurso_aux->blocks, atoi(blocks[i]));
    }
    recurso_aux->caracter_llenado = string_new();
    string_append(&(recurso_aux->caracter_llenado), config_get_string_value(recurso_config, "CARACTER_LLENADO"));
    recurso_aux->md5_archivo = string_new();
    string_append(&(recurso_aux->md5_archivo), config_get_string_value(recurso_config, "MD5_ARCHIVO"));
    config_destroy(recurso_config);

    return recurso_aux;
}

void print_recurso(t_recurso *recurso)
{
    printf("PATH: %s\n", recurso->path);
    printf("SIZE: %d\n", recurso->size);
    printf("BLOCK_COUNT: %d\n", recurso->block_count);
    printf("BLOCKS: %s\n", blocks_list_to_string(recurso->blocks));
    printf("CARACTER_LLENADO: %s\n", recurso->caracter_llenado);
    printf("MD5_ARCHIVO: %s\n", recurso->md5_archivo);
}

void update_recurso(t_recurso *recurso)
{
    t_config *recurso_config = config_create(recurso->path);

    config_set_value(recurso_config, "SIZE", string_itoa(recurso->size));
    config_set_value(recurso_config, "BLOCK_COUNT", string_itoa(recurso->block_count));
    config_set_value(recurso_config, "BLOCKS", blocks_list_to_string(recurso->blocks));
    config_set_value(recurso_config, "MD5_ARCHIVO", recurso->md5_archivo);
    config_save(recurso_config);

    config_destroy(recurso_config);
}

void agregar_recurso(t_recurso *recurso, int cantidad)
{
    int bytes_to_write = cantidad;
    char caracter_llenado = recurso->caracter_llenado[0];

    while (bytes_to_write != 0)
    {
        int last_block;
        int last_block_offset = recurso->size % superbloque->block_size;
        int free_bytes_last_block = superbloque->block_size - last_block_offset;

        if (!list_is_empty(recurso->blocks) && last_block_offset > 0)
        {
            last_block = list_get(recurso->blocks, list_size(recurso->blocks) - 1);
        }
        else
        {
            last_block = get_free_block();
            set_block(last_block);
            list_add(recurso->blocks, last_block);
            recurso->block_count++;
        }

        int blocks_file_offset = last_block * superbloque->block_size + last_block_offset;

        for (int i = 0; i < free_bytes_last_block && bytes_to_write > 0; i++)
        {
            blocks_file_copy[blocks_file_offset + i] = caracter_llenado;
            bytes_to_write--;
            recurso->size++;
        }
    }

    // TODO: Actualizar MD5
    // ...

    update_recurso(recurso);
}

void eliminar_recurso(t_recurso *recurso, int cantidad)
{
    int bytes_to_delete = cantidad < recurso->size ? cantidad : recurso->size;

    while (bytes_to_delete != 0)
    {
        int last_block = list_get(recurso->blocks, list_size(recurso->blocks) - 1);
        int block_bytes_count = recurso->size % superbloque->block_size;
        int byte_to_delete = last_block * superbloque->block_size - 1;
        byte_to_delete += block_bytes_count == 0 ? superbloque->block_size : block_bytes_count;

        blocks_file_copy[byte_to_delete] = '-';
        bytes_to_delete--;
        recurso->size--;

        if (recurso->size % superbloque->block_size == 0)
        {
            int block_to_free = list_remove(recurso->blocks, list_size(recurso->blocks) - 1);
            clean_block(block_to_free);
            recurso->block_count--;
        }
    }

    // TODO: Actualizar MD5
    // ...

    update_recurso(recurso);
}