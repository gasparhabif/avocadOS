#include "proceso2.h"

char *generate_md5(char *msg)
{
    char *file_path = "./md5.tmp";
    int file = open(file_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    char *command = string_from_format("echo -n %s | md5sum > %s", msg, file_path);
    system(command);
    struct stat file_stat;
    fstat(file, &file_stat);

    char *mapped_file = mmap(NULL, file_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file, SEEK_SET);
    char *md5_result = string_substring_until(mapped_file, MD5_SIZE);

    munmap((void *)file, file_stat.st_size);
    close(file);
    remove(file_path);

    return md5_result;
}

void create_recurso(char *recurso_file_path, char *caracter_llenado)
{
    int recurso_fd = open(recurso_file_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    t_config *recurso_config = config_create(recurso_file_path);
    config_set_value(recurso_config, "SIZE", "0");
    config_set_value(recurso_config, "BLOCK_COUNT", "0");
    config_set_value(recurso_config, "BLOCKS", "[]");
    config_set_value(recurso_config, "CARACTER_LLENADO", caracter_llenado);
    char *md5_archivo = string_new();
    md5_archivo = generate_md5("");
    config_set_value(recurso_config, "MD5_ARCHIVO", md5_archivo);
    config_save(recurso_config);

    config_destroy(recurso_config);
    close(recurso_fd);
    free(md5_archivo);
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
        list_add(recurso_aux->blocks, (void *)atoi(blocks[i]));
    }
    recurso_aux->caracter_llenado = string_new();
    string_append(&(recurso_aux->caracter_llenado), config_get_string_value(recurso_config, "CARACTER_LLENADO"));
    recurso_aux->md5_archivo = string_new();
    string_append(&(recurso_aux->md5_archivo), config_get_string_value(recurso_config, "MD5_ARCHIVO"));
    config_destroy(recurso_config);

    log_info(logger, "Se cargó %s", recurso_file_path);

    return recurso_aux;
}

void print_recurso(t_recurso *recurso)
{
    printf("PATH: %s\n", recurso->path);
    printf("SIZE: %d\n", recurso->size);
    printf("BLOCK_COUNT: %d\n", recurso->block_count);
    char *blocks_aux = blocks_list_to_string(recurso->blocks);
    printf("BLOCKS: %s\n", blocks_aux);
    printf("CARACTER_LLENADO: %s\n", recurso->caracter_llenado);
    printf("MD5_ARCHIVO: %s\n", recurso->md5_archivo);
    free(blocks_aux);
}

void update_recurso_metadata(t_recurso *recurso)
{
    t_config *recurso_config = config_create(recurso->path);

    config_set_value(recurso_config, "SIZE", string_itoa(recurso->size));
    config_set_value(recurso_config, "BLOCK_COUNT", string_itoa(recurso->block_count));
    char *blocks_aux = blocks_list_to_string(recurso->blocks);
    config_set_value(recurso_config, "BLOCKS", blocks_aux);
    config_set_value(recurso_config, "MD5_ARCHIVO", recurso->md5_archivo);
    config_save(recurso_config);

    config_destroy(recurso_config);
    free(blocks_aux);
}

void liberar_recurso(t_recurso *recurso)
{
    free(recurso->path);
    free(recurso->caracter_llenado);
    free(recurso->md5_archivo);
    list_destroy(recurso->blocks);
    free(recurso);
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
            last_block = (int)list_get(recurso->blocks, list_size(recurso->blocks) - 1);
        }
        else
        {
            last_block = get_free_block();
            if (last_block == BLOCK_ASSIGN_ERROR)
            {
                log_error(logger, "No hay bloques disponibles. Se rompió todo");
                exit(EXIT_FAILURE);
            }
            set_block(last_block);
            list_add(recurso->blocks, (void *)last_block);
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

    char *recurso_file = string_new();
    for (int i = 0; i < recurso->size; i++)
    {
        string_append(&recurso_file, recurso->caracter_llenado);
    }
    recurso->md5_archivo = generate_md5(recurso_file);

    free(recurso_file);
    update_recurso_metadata(recurso);
    // update_last_recurso(recurso);
    // liberar_recurso(recurso);
}

void eliminar_recurso(t_recurso *recurso, int cantidad)
{
    int bytes_to_delete = cantidad < recurso->size ? cantidad : recurso->size;

    while (bytes_to_delete != 0)
    {
        int last_block = (int)list_get(recurso->blocks, list_size(recurso->blocks) - 1);
        int block_bytes_count = recurso->size % superbloque->block_size;
        int byte_to_delete = last_block * superbloque->block_size - 1;
        byte_to_delete += block_bytes_count == 0 ? superbloque->block_size : block_bytes_count;

        blocks_file_copy[byte_to_delete] = '-';
        bytes_to_delete--;
        recurso->size--;

        if (recurso->size % superbloque->block_size == 0)
        {
            int block_to_free = (int)list_remove(recurso->blocks, list_size(recurso->blocks) - 1);
            clean_block(block_to_free);
            recurso->block_count--;
        }
    }

    char *recurso_file = string_new();
    for (int i = 0; i < recurso->size; i++)
    {
        string_append(&recurso_file, recurso->caracter_llenado);
    }
    recurso->md5_archivo = generate_md5(recurso_file);

    free(recurso_file);
    update_recurso_metadata(recurso);
    // update_last_recurso(recurso);
    // liberar_recurso(recurso);
}
