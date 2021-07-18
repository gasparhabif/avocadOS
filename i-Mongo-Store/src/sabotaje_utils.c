#include "proceso2.h"

void sabotaje_handler(int signal)
{
    if (signal != SIGUSR1)
    {
        return;
    }

    log_info(logger, "Hubo bardo en el FS!");

    // Enviar posición de sabotaje al Discordiador
    int tamanioSerializacion;
    void *posicion_sabotaje = serializar_envioSabotaje(1, 1, &tamanioSerializacion);
    send(discordiador_cxn, posicion_sabotaje, tamanioSerializacion, 0);

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

bool file_check(char *path)
{
    t_recurso *recurso = load_recurso(path);

    // Reconstruir archivo
    // ...

    // Calcular MD5 del archivo reconstruído y comparar
    // ...

    // TODO: Liberar recurso
    // ...

    return true;
}

void repair_file(char *path) {}

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
        return;
    }

    // Verificar bitmap
    log_info(logger, "Verificando bitmap...");
    if (!bitmap_check())
    {
        log_error(logger, "Hubo un sabotaje en el bitmap");
        log_info(logger, "Reparando bitmap...");
        repair_bitmap();
        log_info(logger, "Reparación de bitmap finalizada");
        return;
    }

    // Verificar files
    log_info(logger, "Verificando Oxigeno.ims...");
    if (!file_check(oxigeno_file_path))
    {
        log_error(logger, "Hubo un sabotaje en Oxigeno.ims");
        log_info(logger, "Reparando Oxigeno.ims...");
        repair_file(oxigeno_file_path);
        log_info(logger, "Reparación de Oxigeno.ims finalizada");
        return;
    }

    log_info(logger, "Verificando Comida.ims...");
    if (!file_check(comida_file_path))
    {
        log_error(logger, "Hubo un sabotaje en Comida.ims");
        log_info(logger, "Reparando Comida.ims...");
        repair_file(comida_file_path);
        log_info(logger, "Reparación de Comida.ims finalizada");
        return;
    }

    log_info(logger, "Verificando Basura.ims...");
    if (!file_check(basura_file_path))
    {
        log_error(logger, "Hubo un sabotaje en Basura.ims");
        log_info(logger, "Reparando Basura.ims...");
        repair_file(basura_file_path);
        log_info(logger, "Reparación de Basura.ims finalizada");
        return;
    }
}