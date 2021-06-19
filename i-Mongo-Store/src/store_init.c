#include "proceso2.h"

void init_paths()
{
    // Directorio de Files
    files_dir_path = malloc(strlen(config->punto_montaje) + strlen("/Files"));
    strcpy(files_dir_path, config->punto_montaje);
    strcat(files_dir_path, "/Files");

    // Directorio de Bitacoras
    bitacoras_dir_path = malloc(strlen(files_dir_path) + strlen("/Bitacoras"));
    strcpy(bitacoras_dir_path, files_dir_path);
    strcat(bitacoras_dir_path, "/Bitacoras");

    // Archivo SuperBloque.ims
    superbloque_file_path = malloc(strlen(config->punto_montaje) + strlen("/SuperBloque.ims"));
    strcpy(superbloque_file_path, config->punto_montaje);
    strcat(superbloque_file_path, "/SuperBloque.ims");

    // Archivo Blocks.ims
    blocks_file_path = malloc(strlen(config->punto_montaje) + strlen("/Blocks.ims"));
    strcpy(blocks_file_path, config->punto_montaje);
    strcat(blocks_file_path, "/Blocks.ims");
}

bool file_exists(char *path)
{
    return access(path, F_OK) == 0;
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

void clean_bitarray(t_bitarray *bitarray)
{
    int bitarray_size = bitarray_get_max_bit(bitarray);
    for (int i = 0; i < bitarray_size; i++)
    {
        if (bitarray_test_bit(bitarray, i))
        {
            bitarray_clean_bit(bitarray, i);
        }
    }
}

void create_superbloque(uint32_t block_size, uint32_t blocks)
{
    t_superbloque *new_superbloque = malloc(sizeof(t_superbloque));

    // Inicializar estructura
    new_superbloque->block_size = block_size;
    new_superbloque->blocks = blocks;
    void *bit_pointer = malloc(1);
    new_superbloque->bitmap = bitarray_create_with_mode(bit_pointer, blocks / 8, LSB_FIRST);
    clean_bitarray(new_superbloque->bitmap);

    // Crear archivo SuperBloque.ims
    int superbloque_fd = open(superbloque_file_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    // Crear t_config para SuperBloque.ims e inicializar valores
    t_config *superbloque_config = config_create(superbloque_file_path);
    config_set_value(superbloque_config, "BLOCK_SIZE", string_itoa(new_superbloque->block_size));
    config_set_value(superbloque_config, "BLOCKS", string_itoa(new_superbloque->blocks));
    config_set_value(superbloque_config, "BITMAP", bitarray_to_string(new_superbloque->bitmap));
    config_save(superbloque_config);

    config_destroy(superbloque_config);
    close(superbloque_fd);
    // TODO: Preguntar por qué no se puede liberar
    // free(bit_pointer);
    free(new_superbloque);
}

void create_blocks(uint32_t blocks_count)
{
    // Crear archivo Blocks.ims
    int blocks_fd = open(blocks_file_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    // Fijar tamaño
    ftruncate(blocks_fd, blocks_count);

    close(blocks_fd);
}

void load_superbloque()
{
    // Crear t_config para obtener valores de SuperBloque.ims
    t_config *superbloque_config = config_create(superbloque_file_path);

    superbloque->block_size = (uint32_t)config_get_int_value(superbloque_config, "BLOCK_SIZE");
    superbloque->blocks = (uint32_t)config_get_int_value(superbloque_config, "BLOCKS");
    void *bit_pointer = malloc(1);
    superbloque->bitmap = bitarray_create_with_mode(bit_pointer, superbloque->blocks / 8, LSB_FIRST);

    char **loaded_bitmap = config_get_array_value(superbloque_config, "BITMAP");
    for (int i = 0; i < superbloque->blocks; i++)
    {
        strcmp(loaded_bitmap[i], "1") == 0 ? bitarray_set_bit(superbloque->bitmap, i) : bitarray_clean_bit(superbloque->bitmap, i);
    }

    // TODO: Preguntar por qué no se puede destruir
    // config_destroy(superbloque_config);
    // TODO: Preguntar por qué no se puede liberar
    // free(bit_pointer);
}

void print_superbloque()
{
    printf("Tamaño de bloque: %d\n", superbloque->block_size);
    printf("Cantidad de bloques: %d\n", superbloque->blocks);
    printf("Estado de bloques: ");
    for (int i = 0; i < superbloque->blocks; i++)
    {
        printf("%d", bitarray_test_bit(superbloque->bitmap, i));
    }
    printf("\n");
}

void init_fs()
{
    // Inicializar paths de archivos y directorios
    init_paths();

    // Verificar existencia de FS
    if (!file_exists(superbloque_file_path))
    {
        log_info(logger, "El FS no existe.");
        log_info(logger, "Creando FS desde cero...");
        // Crear directorios
        mkdir(config->punto_montaje, 0777);
        mkdir(files_dir_path, 0777);
        mkdir(bitacoras_dir_path, 0777);

        // Crear SuperBloque.ims
        create_superbloque(BLOCK_SIZE, BLOCKS);

        // Crear archivo Blocks.ims
        create_blocks(BLOCK_SIZE * BLOCKS);

        log_info(logger, "FS creado exitosamente.");
    }
    else
    {
        log_info(logger, "El FS ya existe.");
    }

    // Cargar SuperBloque desde archivo
    log_info(logger, "Cargando FS...");
    load_superbloque();
    log_info(logger, "FS cargado exitosamente en %s", config->punto_montaje);

    print_superbloque();
}
