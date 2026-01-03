#ifndef SAVE_H
#define SAVE_H

#include "game.h"
#include "file_utils.h"
#include <time.h>

#define MAX_SAVE_SLOTS 4
#define SAVE_MAGIC_NUMBER 0x534E414B  /* "SNAK" en hexadecimal */
#define SAVE_VERSION 1

/* Structure de sauvegarde */
typedef struct {
    unsigned int magic;           /* Numero magique pour verifier le format */
    unsigned int version;         /* Version du format de sauvegarde */
    time_t       timestamp;       /* Date de sauvegarde */
    GameSettings settings;        /* Parametres du jeu */
    Grid         grid;            /* Etat de la grille */
    Snake        snake;           /* Etat du serpent */
    FoodList     food_list;       /* Liste des nourritures */
    int          score;           /* Score actuel */
} SaveData;

/* Structure pour lister les sauvegardes disponibles */
typedef struct {
    int        slot_used[MAX_SAVE_SLOTS];  /* 1 si le slot est utilise, 0 sinon */
    SaveData   previews[MAX_SAVE_SLOTS];   /* Apercu des sauvegardes (pour afficher les infos) */
} SaveSlotList;

/*
 * Sauvegarde une partie dans un slot donne (0 a MAX_SAVE_SLOTS-1).
 * Retourne FILE_UTILS_SUCCESS en cas de succes, un code d'erreur sinon.
 */
int save_game(const Game *game, int slot);

/*
 * Charge une partie depuis un slot donne.
 * Retourne FILE_UTILS_SUCCESS en cas de succes, un code d'erreur sinon.
 */
int load_game(Game *game, int slot);

/*
 * Verifie si une sauvegarde est valide (magic, version).
 * Retourne 1 si valide, 0 sinon.
 */
int is_save_valid(const SaveData *save);

/*
 * Supprime une sauvegarde dans un slot donne.
 * Retourne FILE_UTILS_SUCCESS en cas de succes, un code d'erreur sinon.
 */
int delete_save(int slot);

/*
 * Charge la liste des sauvegardes disponibles.
 * Remplit la structure SaveSlotList avec les infos des saves existantes.
 */
void load_save_slot_list(SaveSlotList *list);

/*
 * Retourne le chemin du fichier de sauvegarde pour un slot donne.
 */
void get_save_filepath(int slot, char *filepath);

#endif
