#include "proceso2.h"

bool file_exists(char *path)
{
    return access(path, F_OK) == 0;
}

void init_fs()
{
    // Crear directorio para FS
    if (mkdir(config->punto_montaje, 0777) == -1)
    {
        perror("Algo malió sal con la función mkdir()");
    }

    // Crear directorio para Files
    if (mkdir("/home/utnso/fs/Files", 0777) == -1)
    {
        perror("Algo malió sal con la función mkdir()");
    }

    // Crear directorio para Bitacoras
    if (mkdir("/home/utnso/fs/Files/Bitacoras", 0777) == -1)
    {
        perror("Algo malió sal con la función mkdir()");
    }

    // Crear SuperBloque.ims vacío
    int superbloque_fd = open("/home/utnso/fs/SuperBloque.ims", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    if (superbloque_fd == -1)
    {
        perror("Algo malió sal con la función open()");
        return EXIT_FAILURE;
    }

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

    close(superbloque_fd);
    close(oxigeno_fd);

    return EXIT_SUCCESS;
}
