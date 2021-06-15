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

void init_fs()
{
    // Inicializar directorios
    init_dirs();

    // Inicializar SuperBloque
    init_superbloque(BLOCK_SIZE, BLOCKS);

    // Crear Oxigeno.ims de prueba
    int oxigeno_fd = open("/home/utnso/fs/Files/Oxigeno.ims", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    if (oxigeno_fd == -1)
    {
        perror("Algo malió sal con la función open()");
        return EXIT_FAILURE;
    }

    // Inicializar archivo de prueba Oxigeno.ims
    char *init_data = "SIZE=0\nBLOCK_COUNT=0\nBLOCKS=[]\nCARACTER_LLENADO=O\nMD5_ARCHIVO=UNSET";

    if (write(oxigeno_fd, init_data, strlen(init_data)) == -1)
    {
        perror("Algo malió sal con la función write()");
        return EXIT_FAILURE;
    }

    // Obtener tamaño del archivo de prueba Oxigeno.ims
    struct stat oxigeno_stats;

    if (fstat(oxigeno_fd, &oxigeno_stats) == -1)
    {
        perror("Algo malió sal con la función fstat()");
        return EXIT_FAILURE;
    }

    // Mapear archivo de prueba Oxigeno.ims en memoria
    char *oxigeno_mapped_file = mmap(NULL, oxigeno_stats.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, oxigeno_fd, SEEK_SET);

    // Imprimir información del archivo de prueba Oxigeno.ims
    printf("El archivo pesa %d bytes\n", (int)oxigeno_stats.st_size);

    // Split de líneas del archivo de prueba Oxigeno.ims
    char **oxigeno_lines = string_split(oxigeno_mapped_file, "\n");

    // Split de key/value de cada línea del archivo de prueba Oxigeno.ims
    printf("#\tTamaño\tContenido\n");
    for (int i = 0; oxigeno_lines[i] != '\0'; i++)
    {
        char **line_values = string_split(oxigeno_lines[i], "=");
        printf("%d\t%d\t%s --> %s\n", i, strlen(oxigeno_lines[i]), line_values[0], line_values[1]);
    }

    munmap(oxigeno_mapped_file, oxigeno_stats.st_size);

    close(oxigeno_fd);

    return EXIT_SUCCESS;
}
