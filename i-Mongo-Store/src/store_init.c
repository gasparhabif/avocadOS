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

    // Archivo Oxigeno.ims
    oxigeno_file_path = malloc(strlen(files_dir_path) + strlen("/Oxigeno.ims"));
    strcpy(oxigeno_file_path, files_dir_path);
    strcat(oxigeno_file_path, "/Oxigeno.ims");

    // Archivo Comida.ims
    comida_file_path = malloc(strlen(files_dir_path) + strlen("/Comida.ims"));
    strcpy(comida_file_path, files_dir_path);
    strcat(comida_file_path, "/Comida.ims");

    // Archivo Basura.ims
    basura_file_path = malloc(strlen(files_dir_path) + strlen("/Basura.ims"));
    strcpy(basura_file_path, files_dir_path);
    strcat(basura_file_path, "/Basura.ims");
}

bool file_exists(char *path)
{
    return access(path, F_OK) == 0;
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
    // Crear bitarray
    void *bit_pointer = malloc(blocks / 8);
    t_bitarray *bitmap = bitarray_create_with_mode(bit_pointer, blocks / 8, LSB_FIRST);
    clean_bitarray(bitmap);

    // Crear archivo SuperBloque.ims
    int superbloque_fd = open(superbloque_file_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    int superbloque_file_size = 2 * sizeof(uint32_t) + blocks / 8;
    ftruncate(superbloque_fd, superbloque_file_size);

    // Mapear archivo
    void *superbloque_file = mmap(NULL, superbloque_file_size, PROT_READ | PROT_WRITE, MAP_SHARED, superbloque_fd, SEEK_SET);

    // Copiar valores en archivo mapeado
    memcpy(superbloque_file, &block_size, sizeof(uint32_t));
    memcpy(superbloque_file + sizeof(uint32_t), &blocks, sizeof(uint32_t));
    memcpy(superbloque_file + 2 * sizeof(uint32_t), bitmap->bitarray, bitmap->size);

    // Liberar memoria
    free(bit_pointer);
    free(bitmap);
    munmap(superbloque_file, superbloque_file_size);
    close(superbloque_fd);
}

void create_blocks(uint32_t blocks_count)
{
    // Crear archivo Blocks.ims
    int blocks_fd = open(blocks_file_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    ftruncate(blocks_fd, blocks_count);

    close(blocks_fd);
}

void load_superbloque()
{
    // Abrir archivo SuperBloque.ims
    int superbloque_fd = open(superbloque_file_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    // Obtener tamaño
    struct stat sb;
    fstat(superbloque_fd, &sb);

    // Mapear archivo
    void *superbloque_file = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, superbloque_fd, SEEK_SET);

    // Copiar valores de archivo mapeado
    memcpy(&(superbloque->block_size), superbloque_file, sizeof(uint32_t));
    memcpy(&(superbloque->blocks), superbloque_file + sizeof(uint32_t), sizeof(uint32_t));
    void *bit_pointer = malloc(superbloque->blocks / 8);
    memcpy(bit_pointer, superbloque_file + 2 * sizeof(uint32_t), superbloque->blocks / 8);
    superbloque->bitmap = bitarray_create_with_mode(bit_pointer, superbloque->blocks / 8, LSB_FIRST);

    munmap(superbloque_file, sb.st_size);
    close(superbloque_fd);
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
    log_info(logger, "FS cargado exitosamente.");

    // Imprimir SuperBloque
    // print_superbloque();
}
