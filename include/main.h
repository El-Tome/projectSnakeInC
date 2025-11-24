#ifndef SNAKE_MAIN_H
#define SNAKE_MAIN_H

#define FPS 60
#define FRAME_TIME_NS (1000000000 / FPS)
#define NANOS_TO_MILLIS 1000000


#include <MLV/MLV_all.h>
#include <time.h>
#include "controls.h"

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
