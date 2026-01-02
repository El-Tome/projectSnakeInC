#include "file_utils.h"

int write_binary_file(const char *filepath, const void *data, size_t size) {
    /* Le const void *data c'est pour un pointeur qui ne sera pas modifié et générique pour pouvoir le réutiliser avec différentes structures */
    
    FILE *file;
    size_t written;
    
    file = fopen(filepath, "wb"); /* Ouverture en mode écriture binaire */
    if (file == NULL) {
        return FILE_UTILS_ERR_OPEN;
    }
    
    written = fwrite(data, 1, size, file);
    fclose(file);
    
    if (written != size) {
        return FILE_UTILS_ERR_WRITE;
    }
    
    return FILE_UTILS_SUCCESS;
}

int read_binary_file(const char *filepath, void *data, size_t size) {
    /* Le void *data c'est pour un pointeur générique pour pouvoir le réutiliser avec différentes structures */
    
    FILE *file;
    size_t bytes_read;
    
    file = fopen(filepath, "rb"); /* Ouverture en mode lecture binaire */
    if (file == NULL) {
        return FILE_UTILS_ERR_OPEN;
    }
    
    bytes_read = fread(data, 1, size, file);
    fclose(file);
    
    if (bytes_read != size) {
        return FILE_UTILS_ERR_READ;
    }
    
    return FILE_UTILS_SUCCESS;
}

int file_exists(const char *filepath) {
    FILE *file = fopen(filepath, "rb"); /* Ouverture en mode lecture binaire */
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    return 0;
}
