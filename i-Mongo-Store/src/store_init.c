#include "proceso2.h"

void init_paths()
{
    // Directorios
    files_dir_path = string_from_format("%s/Files", config->punto_montaje);
    bitacoras_dir_path = string_from_format("%s/Bitacoras", files_dir_path);

    // Archivos
    superbloque_file_path = string_from_format("%s/SuperBloque.ims", config->punto_montaje);
    blocks_file_path = string_from_format("%s/Blocks.ims", config->punto_montaje);
    oxigeno_file_path = string_from_format("%s/Oxigeno.ims", files_dir_path);
    comida_file_path = string_from_format("%s/Comida.ims", files_dir_path);
    basura_file_path = string_from_format("%s/Basura.ims", files_dir_path);
}

bool file_exists(char *path)
{
    return access(path, F_OK) == 0;
}

void create_dirs()
{
    mkdir(config->punto_montaje, 0777);
    mkdir(files_dir_path, 0777);
    mkdir(bitacoras_dir_path, 0777);
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
    void *bit_pointer = malloc(blocks / 8);
    t_bitarray *bitmap = bitarray_create_with_mode(bit_pointer, blocks / 8, LSB_FIRST);
    clean_bitarray(bitmap);

    // Crear archivo SuperBloque.ims
    int superbloque_fd = open(superbloque_file_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    int superbloque_file_size = 2 * sizeof(uint32_t) + bitmap->size;
    ftruncate(superbloque_fd, superbloque_file_size);

    // Mapear archivo
    void *superbloque_file = mmap(NULL, superbloque_file_size, PROT_READ | PROT_WRITE, MAP_SHARED, superbloque_fd, SEEK_SET);

    // Copiar valores en archivo mapeado
    memcpy(superbloque_file, &(block_size), sizeof(uint32_t));
    memcpy(superbloque_file + sizeof(uint32_t), &(blocks), sizeof(uint32_t));
    memcpy(superbloque_file + 2 * sizeof(uint32_t), bitmap->bitarray, bitmap->size);

    munmap(superbloque_file, superbloque_file_size);
    close(superbloque_fd);
}

t_superbloque *load_superbloque()
{
    // Abrir archivo SuperBloque.ims
    superbloque_fd = open(superbloque_file_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    // Obtener tamaño
    struct stat superbloque_stat;
    fstat(superbloque_fd, &superbloque_stat);
    superbloque_file_size = superbloque_stat.st_size;

    // Mapear archivo
    superbloque_file = mmap(NULL, superbloque_file_size, PROT_READ | PROT_WRITE, MAP_SHARED, superbloque_fd, SEEK_SET);

    // Copiar valores de archivo mapeado
    t_superbloque *superbloque_aux = malloc(sizeof(t_superbloque));
    memcpy(&(superbloque_aux->block_size), superbloque_file, sizeof(uint32_t));
    memcpy(&(superbloque_aux->blocks), superbloque_file + sizeof(uint32_t), sizeof(uint32_t));
    void *bit_pointer = malloc(superbloque_aux->blocks / 8);
    memcpy(bit_pointer, superbloque_file + 2 * sizeof(uint32_t), superbloque_aux->blocks / 8);
    superbloque_aux->bitmap = bitarray_create_with_mode(bit_pointer, superbloque_aux->blocks / 8, LSB_FIRST);

    return superbloque_aux;
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

void create_blocks()
{
    uint32_t blocks_file_size = BLOCK_SIZE * BLOCKS;

    // Crear archivo Blocks.ims
    int blocks_fd = open(blocks_file_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    ftruncate(blocks_fd, blocks_file_size);

    // Mapear archivo
    blocks_file = mmap(NULL, blocks_file_size, PROT_READ | PROT_WRITE, MAP_SHARED, blocks_fd, SEEK_SET);

    for (int i = 0; i < blocks_file_size; i++)
    {
        blocks_file[i] = i % BLOCK_SIZE == 0 ? '|' : '-';
    }

    munmap(blocks_file, blocks_file_size);
    close(blocks_fd);
}

char *load_blocks()
{
    // Abrir archivo Blocks.ims
    blocks_fd = open(blocks_file_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    // Obtener tamaño
    struct stat blocks_stat;
    fstat(blocks_fd, &blocks_stat);
    blocks_file_size = blocks_stat.st_size;

    // Mapear archivo
    blocks_file = mmap(NULL, blocks_file_size, PROT_READ | PROT_WRITE, MAP_SHARED, blocks_fd, SEEK_SET);

    // Copiar valores de archivo mapeado
    char *blocks_aux = malloc(blocks_file_size);
    memcpy(blocks_aux, blocks_file, blocks_file_size);

    return blocks_aux;
}

void sync_blocks()
{
    for (int i = 0; i < blocks_file_size; i++)
    {
        blocks_file[i] = blocks_file_copy[i];
    }
}
