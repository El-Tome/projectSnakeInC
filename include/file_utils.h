#ifndef SNAKE_FILE_UTILS_H
#define SNAKE_FILE_UTILS_H

#include <stdio.h>
#include <stdlib.h>

/*
 * Module utilitaire pour la gestion de fichiers binaires.
 * Fournit des fonctions generiques pour lire et ecrire des fichiers binaires.
 */

#define FILE_UTILS_SUCCESS   0
#define FILE_UTILS_ERR_OPEN -1
#define FILE_UTILS_ERR_READ -2
#define FILE_UTILS_ERR_WRITE -3

/*
 * Ecrit des donnees binaires dans un fichier.
 * Retourne FILE_UTILS_SUCCESS en cas de succes, un code d'erreur sinon.
 */
int write_binary_file(const char *filepath, const void *data, size_t size);

/*
 * Lit des donnees binaires depuis un fichier.
 * Retourne FILE_UTILS_SUCCESS en cas de succes, un code d'erreur sinon.
 */
int read_binary_file(const char *filepath, void *data, size_t size);

/*
 * Verifie si un fichier existe.
 * Retourne 1 si le fichier existe, 0 sinon.
 */
int file_exists(const char *filepath);

#endif
