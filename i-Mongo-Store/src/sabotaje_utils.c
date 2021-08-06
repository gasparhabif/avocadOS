#include "proceso2.h"

t_posicion *next_pos_sabotaje()
{
    t_posicion *first_pos = list_remove(config->posiciones_sabotaje, 0);
    list_add(config->posiciones_sabotaje, first_pos);

    return first_pos;
}

void sabotaje_handler(int signal)
{
    if (signal != SIGUSR1)
    {
        return;
    }

    t_posicion *next_pos = next_pos_sabotaje();
    log_error(logger, "Se armó bardo en la posición %d|%d de la nave", next_pos->posX, next_pos->posY);

    // Enviar posicion del sabotaje
    int tamano_paquete;
    void *posicion_sabotaje = serializar_envioSabotaje(next_pos->posX, next_pos->posY, &tamano_paquete);
    send(discordiador_cxn_sabotajes, posicion_sabotaje, tamano_paquete, 0);
    log_info(logger, "Se envió la posición %d|%d del sabotaje al Discordiador", next_pos->posX, next_pos->posY);

    free(posicion_sabotaje);
}

bool blocks_count_check()
{
    uint32_t blocks_aux;
    memcpy(&(blocks_aux), superbloque_file + sizeof(uint32_t), sizeof(uint32_t));

    return superbloque->blocks == blocks_aux;
}

void repair_blocks_count()
{
    memcpy(superbloque_file + sizeof(uint32_t), &(superbloque->blocks), sizeof(uint32_t));
}

bool bitmap_check()
{

    void *bit_pointer_aux = malloc(superbloque->blocks / 8);
    memcpy(bit_pointer_aux, superbloque_file + 2 * sizeof(uint32_t), superbloque->blocks / 8);
    t_bitarray *bitmap_aux = bitarray_create_with_mode(bit_pointer_aux, superbloque->blocks / 8, LSB_FIRST);

    for (int i = 0; i < superbloque->blocks; i++)
    {
        if (bitarray_test_bit(superbloque->bitmap, i) != bitarray_test_bit(bitmap_aux, i))
        {
            bitarray_destroy(bitmap_aux);
            return false;
        }
    }

    bitarray_destroy(bitmap_aux);
    return true;
}

void repair_bitmap()
{
    memcpy(superbloque_file + 2 * sizeof(uint32_t), superbloque->bitmap->bitarray, superbloque->bitmap->size);
}

char *reconstruir_recurso(t_recurso *recurso)
{
    char *recurso_string = string_new();

    for (int i = 0; i < list_size(recurso->blocks); i++)
    {
        char *block_info = malloc(superbloque->block_size);
        memcpy(block_info, blocks_file + (int)list_get(recurso->blocks, i) * superbloque->block_size, superbloque->block_size);
        string_append(&recurso_string, block_info);
        free(block_info);
    }

    return string_substring_until(recurso_string, recurso->size);
}

bool file_check(char *path)
{
    t_recurso *recurso = load_recurso(path);

    // Reconstruir archivo
    char *recurso_string = reconstruir_recurso(recurso);

    // Calcular MD5 del archivo reconstruído y comparar
    char *md5_recurso_string = generate_md5(recurso_string);
    bool result = string_equals_ignore_case(md5_recurso_string, recurso->md5_archivo);

    free(md5_recurso_string);
    free(recurso_string);
    liberar_recurso(recurso);

    return result;
}

bool recurso_size_check(t_recurso *last_recurso, t_recurso *recurso)
{
    return last_recurso->size == recurso->size;
}

bool recurso_block_count_check(t_recurso *recurso)
{
    return recurso->block_count == list_size(recurso->blocks);
}

bool recurso_blocks_check(t_recurso *last_recurso, t_recurso *recurso)
{
    if (list_size(last_recurso->blocks) != list_size(recurso->blocks))
    {
        return false;
    }

    for (int i = 0; i < list_size(last_recurso->blocks); i++)
    {
        if (list_get(last_recurso->blocks, i) != list_get(recurso->blocks, i))
        {
            return false;
        }
    }

    return true;
}

void ejecutar_fsck()
{
    log_info(logger, "Iniciando protocolo FSCK...");

    // Verificar cantidad de bloques
    log_info(logger, "Verificando cantidad de bloques...");
    if (!blocks_count_check())
    {
        log_error(logger, "Hubo un sabotaje en la cantidad de bloques");
        log_info(logger, "Reparando cantidad de bloques...");
        repair_blocks_count();
        log_info(logger, "Reparación de cantidad de bloques finalizada");
    }

    // Verificar bitmap
    log_info(logger, "Verificando bitmap...");
    if (!bitmap_check())
    {
        log_error(logger, "Hubo un sabotaje en el bitmap");
        log_info(logger, "Reparando bitmap...");
        repair_bitmap();
        log_info(logger, "Reparación de bitmap finalizada");
    }

    // Verificar Oxigeno.ims
    if (file_exists(oxigeno_file_path))
    {
        log_info(logger, "Verificando Oxigeno.ims...");
        t_recurso *oxigeno_aux = load_recurso(oxigeno_file_path);

        // Verificar tamaño del archivo
        if (!recurso_size_check(backup_oxigeno, oxigeno_aux))
        {
            log_error(logger, "Hubo un sabotaje en SIZE de Oxigeno.ims");
            log_info(logger, "Reparando SIZE de Oxigeno.ims...");
            update_recurso_metadata(backup_oxigeno);
            log_info(logger, "Reparación de SIZE de Oxigeno.ims finalizada");
        }

        // Verificar cantidad de bloques y lista de bloques
        if (!recurso_block_count_check(oxigeno_aux))
        {
            log_error(logger, "Hubo un sabotaje en BLOCK_COUNT de Oxigeno.ims");
            log_info(logger, "Reparando BLOCK_COUNT de Oxigeno.ims...");
            update_recurso_metadata(backup_oxigeno);
            log_info(logger, "Reparación de BLOCK_COUNT de Oxigeno.ims finalizada");
        }

        // Verificar bloques
        if (!recurso_blocks_check(backup_oxigeno, oxigeno_aux))
        {
            log_error(logger, "Hubo un sabotaje en BLOCKS de Oxigeno.ims");
            log_info(logger, "Reparando BLOCKS de Oxigeno.ims...");
            update_recurso_metadata(backup_oxigeno);
            log_info(logger, "Reparación de BLOCKS de Oxigeno.ims finalizada");
        }

        liberar_recurso(oxigeno_aux);
    }

    // Verificar Comida.ims
    if (file_exists(comida_file_path))
    {
        log_info(logger, "Verificando Comida.ims...");
        t_recurso *comida_aux = load_recurso(comida_file_path);

        // Verificar tamaño del archivo
        if (!recurso_size_check(backup_comida, comida_aux))
        {
            log_error(logger, "Hubo un sabotaje en SIZE de Comida.ims");
            log_info(logger, "Reparando SIZE de Comida.ims...");
            update_recurso_metadata(backup_comida);
            log_info(logger, "Reparación de SIZE de Comida.ims finalizada");
        }

        // Verificar cantidad de bloques y lista de bloques
        if (!recurso_block_count_check(comida_aux))
        {
            log_error(logger, "Hubo un sabotaje en BLOCK_COUNT de Comida.ims");
            log_info(logger, "Reparando BLOCK_COUNT de Comida.ims...");
            update_recurso_metadata(backup_comida);
            log_info(logger, "Reparación de BLOCK_COUNT de Comida.ims finalizada");
        }

        // Verificar bloques
        if (!recurso_blocks_check(backup_comida, comida_aux))
        {
            log_error(logger, "Hubo un sabotaje en BLOCKS de Comida.ims");
            log_info(logger, "Reparando BLOCKS de Comida.ims...");
            update_recurso_metadata(backup_comida);
            log_info(logger, "Reparación de BLOCKS de Comida.ims finalizada");
        }

        liberar_recurso(comida_aux);
    }

    // Verificar Basura.ims
    if (file_exists(basura_file_path))
    {
        log_info(logger, "Verificando Basura.ims...");
        t_recurso *basura_aux = load_recurso(basura_file_path);

        // Verificar tamaño del archivo
        if (!recurso_size_check(backup_basura, basura_aux))
        {
            log_error(logger, "Hubo un sabotaje en SIZE de Basura.ims");
            log_info(logger, "Reparando SIZE de Basura.ims...");
            update_recurso_metadata(backup_basura);
            log_info(logger, "Reparación de SIZE de Basura.ims finalizada");
        }

        // Verificar cantidad de bloques y lista de bloques
        if (!recurso_block_count_check(basura_aux))
        {
            log_error(logger, "Hubo un sabotaje en BLOCK_COUNT de Basura.ims");
            log_info(logger, "Reparando BLOCK_COUNT de Basura.ims...");
            update_recurso_metadata(backup_basura);
            log_info(logger, "Reparación de BLOCK_COUNT de Basura.ims finalizada");
        }

        // Verificar bloques
        if (!recurso_blocks_check(backup_basura, basura_aux))
        {
            log_error(logger, "Hubo un sabotaje en BLOCKS de Basura.ims");
            log_info(logger, "Reparando BLOCKS de Basura.ims...");
            update_recurso_metadata(backup_basura);
            log_info(logger, "Reparación de BLOCKS de Basura.ims finalizada");
        }

        liberar_recurso(basura_aux);
    }
}