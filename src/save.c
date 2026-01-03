#include "save.h"
#include "grille.h"
#include <string.h>
#include <stdio.h>

void get_save_filepath(int slot, char *filepath) {
    switch (slot) {
        case 0:
            strcpy(filepath, "ressources/saves/save_0.dat");
            break;
        case 1:
            strcpy(filepath, "ressources/saves/save_1.dat");
            break;
        case 2:
            strcpy(filepath, "ressources/saves/save_2.dat");
            break;
        default:
            strcpy(filepath, "ressources/saves/save_3.dat");
            break;
    }
}

int is_save_valid(const SaveData *save) {
    /* 1. On verifie si c'est bien un fichier du jeu */
    if (save->magic != SAVE_MAGIC_NUMBER) {
        return 0;
    }
    
    /* 2. On verifie que les dimensions sont logiques */
    if (
        save->settings.width < MIN_GRID_WIDTH   || 
        save->settings.width > MAX_GRID_WIDTH   ||
        save->settings.height < MIN_GRID_HEIGHT || 
        save->settings.height > MAX_GRID_HEIGHT
    ) {
        return 0;
    }

    /* 3 on verifie que le serpent est valide */
    if (save->snake.length < 1 || save->snake.length > MAX_SNAKE_LENGTH) {
        return 0;
    }
    
    return 1;
}

int save_game(const Game *game, int slot) {
    SaveData save;
    char filepath[256];
    
    /* On remplit la structure manuellement */
    save.magic     = SAVE_MAGIC_NUMBER;
    save.version   = SAVE_VERSION;   /* si on fait des modifs plutard sur les saves */
    save.timestamp = time(NULL);
    save.settings  = game->settings;
    save.grid      = game->grid;
    save.snake     = game->snake;
    save.food_list = game->food_list;
    save.score     = game->score;
    
    get_save_filepath(slot, filepath);
    
    /* On utilise la fonction utilitaire pour ecrire tout d'un coup dans le fichier en binaire*/
    return write_binary_file(filepath, &save, sizeof(SaveData));
}

int load_game(Game *game, int slot) {
    SaveData save;
    char filepath[256];
    int retour;
    
    /* On recupere le chemin du fichier de sauvegarde */
    get_save_filepath(slot, filepath);
    
    /* On lit le fichier en binaire */
    retour = read_binary_file(filepath, &save, sizeof(SaveData));
    
    if (retour != FILE_UTILS_SUCCESS) {
        return retour;
    }
    
    /* On verifie si la sauvegarde est "propre" */
    if (is_save_valid(&save) == 0) {
        return FILE_UTILS_ERR_READ;
    }
    
    /* On remet les donnees dans le jeu */
    game->settings  = save.settings;
    game->grid      = save.grid;
    game->snake     = save.snake;
    game->food_list = save.food_list;
    game->score     = save.score;
    game->state     = FREEZE_GAME_MENU;
    
    return FILE_UTILS_SUCCESS;
}

int delete_save(int slot) {
    char filepath[256];
    get_save_filepath(slot, filepath);

     /* On supprime le fichier de sauvegarde */
    if (remove(filepath) == 0) {
        return FILE_UTILS_SUCCESS;
    }
    return FILE_UTILS_ERR_WRITE;
}

void load_save_slot_list(SaveSlotList *list) {
    int i;
    char filepath[256];
    SaveData save;
    
    for (i = 0; i < MAX_SAVE_SLOTS; i++) {
        get_save_filepath(i, filepath);
        
        list->slot_used[i] = 0;

        if (
            file_exists(filepath) &&
            read_binary_file(filepath, &save, sizeof(SaveData)) == FILE_UTILS_SUCCESS &&
            is_save_valid(&save)
        ) {
            list->slot_used[i] = 1;
            list->previews[i]  = save;
        }
    }
}
