#include "proceso2.h"

bool file_exists(char *path)
{
    return access(path, F_OK) == 0;
}

void init_fs()
{
    // Verificar existencia de Superbloque.ims
    if file_exists ()
    {
        // Recuperar FS
        // ...
    }
    else
    {
        // Generar FS desde cero
        //...
    }
}
