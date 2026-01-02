#ifndef SNAKE_MENU_HANDLER_H
#define SNAKE_MENU_HANDLER_H


#include "main.h"
#include "controls.h"
#include "game.h"
#include "menu_display.h"


typedef enum {
    ACTION_NONE_MAIN = -1,
    ACTION_NEW_GAME  = 0,
    ACTION_LOAD_GAME = 1,
    ACTION_SCORES    = 2,
    ACTION_QUIT      = 3
} MainMenuAction;

typedef enum {
    ACTION_NONE_NEW_GAME    = -1,
    ACTION_TOGGLE_2_PLAYERS = 0,
    ACTION_TOGGLE_WALLS     = 1,
    ACTION_DECR_WIDTH       = 2,
    ACTION_INCR_WIDTH       = 3,
    ACTION_DECR_HEIGHT      = 4,
    ACTION_INCR_HEIGHT      = 5,
    ACTION_DECR_SPEED       = 6,
    ACTION_INCR_SPEED       = 7,
    ACTION_BACK_MAIN        = 8,
    ACTION_START_GAME       = 9
} NewGameMenuAction;

typedef enum {
    ACTION_NONE_GAME_OVER = -1,
    ACTION_REPLAY         = 0,
    ACTION_BACK_TO_MENU   = 1
} GameOverMenuAction;

void process_main_menu_actions(
    ButtonsList *buttons_list, 
    MenuState   *menu_state, 
    int         *quitter
);

void process_new_game_menu_actions(
    ButtonsList *buttons_list, 
    Game        *game, 
    MenuState   *menu_state, 
    WindowSize  *window_size
);

void process_game_over_menu_actions(
    ButtonsList *buttons_list,
    Game        *game,
    MenuState   *menu_state,
    WindowSize  *window_size
);


#endif
