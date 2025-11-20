#ifndef SNAKE_MAIN_H
#define SNAKE_MAIN_H

#include <MLV/MLV_all.h>

typedef enum {
    MAIN_MENU      = 0,
    NEW_GAME_MENU  = 1,
    LOAD_GAME_MENU = 2,
    SCORES_MENU    = 3,
    IN_GAME        = 4
} MenuState;

typedef enum {
    FREEZE_GAME_MENU = 0,
    PLAYING          = 1,
    GAME_OVER_MENU   = 2,
    PAUSE_MENU       = 3,
    SAVE_MENU        = 4
} GameState;

typedef struct {
    unsigned int height;
    unsigned int width;
} WindowSize;


#endif
