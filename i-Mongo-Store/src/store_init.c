#include "proceso2.h"

bool file_exists(char *path)
{
    return access(path, F_OK) == 0;
}

void init_dirs()
{
    // Crear directorio para FS
    mkdir(config->punto_montaje, 0777);

    // Crear directorio para Files
    files_dir = malloc(strlen(config->punto_montaje) + strlen("/Files"));
    strcpy(files_dir, config->punto_montaje);
    strcat(files_dir, "/Files");
    mkdir("/home/utnso/fs/Files", 0777);

    // Crear directorio para Bitacoras
    bitacoras_dir = malloc(strlen(files_dir) + strlen("/Bitacoras"));
    strcpy(bitacoras_dir, files_dir);
    strcat(bitacoras_dir, "/Bitacoras");
    mkdir("/home/utnso/fs/Files/Bitacoras", 0777);
}

t_bitarray *init_bitmap(size_t blocks)
{
    char *puntero_a_bits = malloc(1);
    t_bitarray *bitmap = bitarray_create_with_mode(puntero_a_bits, blocks / 8, LSB_FIRST);

    for (int i = 0; i < blocks; i++)
    {
        bitarray_clean_bit(bitmap, i);
    }

    return bitmap;
}

char *bitarray_to_string(t_bitarray *bitarray)
{
    int bitarray_size = bitarray_get_max_bit(bitarray);
    char *bitarray_string = malloc(2 * bitarray_size + 1);

    for (int i = 0; i < bitarray_size; i++)
    {
        if (i == 0)
        {
            strcpy(bitarray_string, "[");
        }

        strcat(bitarray_string, bitarray_test_bit(bitarray, i) ? "1" : "0");
        strcat(bitarray_string, i < bitarray_size - 1 ? "," : "]");
    }

    return bitarray_string;
}

void init_superbloque(uint32_t block_size, uint32_t blocks)
{
    // Crear archivo SuperBloque.ims
    superbloque_path = malloc(strlen(config->punto_montaje) + strlen("/SuperBloque.ims"));
    strcpy(superbloque_path, config->punto_montaje);
    strcat(superbloque_path, "/SuperBloque.ims");
    int superbloque_fd = open(superbloque_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    // Crear t_config para SuperBloque.ims
    t_config *superbloque_config = config_create(superbloque_path);
    config_set_value(superbloque_config, "BLOCK_SIZE", string_itoa((int)block_size));
    config_set_value(superbloque_config, "BLOCKS", string_itoa((int)blocks));
    config_set_value(superbloque_config, "BITMAP", bitarray_to_string(init_bitmap((size_t)blocks)));
    config_save(superbloque_config);

    // Mover superbloque_config a un t_superbloque para mayor comodidad
    // ...

    close(superbloque_fd);
}

void init_blocks(uint32_t bytes_count)
{
    // Crear archivo Blocks.ims
    blocks_path = malloc(strlen(config->punto_montaje) + strlen("/Blocks.ims"));
    strcpy(blocks_path, config->punto_montaje);
    strcat(blocks_path, "/Blocks.ims");
    int blocks_fd = open(blocks_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    ftruncate(blocks_fd, bytes_count);

    // Mapear archivo Blocks.ims en memoria
    char *mapped_blocks = mmap(NULL, bytes_count, PROT_READ | PROT_WRITE, MAP_SHARED, blocks_fd, SEEK_SET);

    // Hacer algo con el archivo
    for (int i = 0; i < bytes_count; i++)
    {
        mapped_blocks[i] = i % 2 == 0 ? 'J' : 'A';
    }

    munmap(mapped_blocks, bytes_count);

    close(blocks_fd);
}

void init_fs()
{
    // Inicializar directorios
    init_dirs();
    log_info(logger, "Se inicializaron los directorios");

    // Inicializar SuperBloque.ims
    init_superbloque(BLOCK_SIZE, BLOCKS);
    log_info(logger, "Se inicializó SuperBloque.ims - Tamaño de bloque: %d - Cantidad de bloques: %d", BLOCK_SIZE, BLOCKS);

    // Inicializar Blocks.ims
    init_blocks(BLOCK_SIZE * BLOCKS);
    log_info(logger, "Se inicializó Blocks.ims - Tamaño de archivo: %d bytes", BLOCK_SIZE * BLOCKS);
}
