#ifndef SNAKE_MENU_HANDLER_H
#define SNAKE_MENU_HANDLER_H


#include "main.h"
#include "controls.h"
#include "game.h"
#include "menu_display.h"
#include "save.h"


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

typedef enum {
    ACTION_NONE_SCORES    = -1,
    ACTION_SCORES_BACK    = -2,
    ACTION_SCORES_DELETE  = 0  /* >= 0 = index du score a supprimer */
} ScoresMenuAction;

typedef enum {
    ACTION_NONE_PAUSE   = -1,
    ACTION_RESUME       = 0,
    ACTION_SAVE_GAME    = 1,
    ACTION_PAUSE_TO_MENU = 2
} PauseMenuAction;

typedef enum {
    ACTION_NONE_LOAD     = -1,
    ACTION_LOAD_BACK     = -2,
    ACTION_LOAD_SLOT_0   = 0,
    ACTION_LOAD_SLOT_1   = 1,
    ACTION_LOAD_SLOT_2   = 2,
    ACTION_LOAD_SLOT_3   = 3,
    ACTION_DELETE_SLOT_0 = 10,
    ACTION_DELETE_SLOT_1 = 11,
    ACTION_DELETE_SLOT_2 = 12,
    ACTION_DELETE_SLOT_3 = 13
} LoadMenuAction;

typedef enum {
    ACTION_NONE_SAVE     = -1,
    ACTION_SAVE_CANCEL   = -2,
    ACTION_SAVE_SLOT_0   = 0,
    ACTION_SAVE_SLOT_1   = 1,
    ACTION_SAVE_SLOT_2   = 2,
    ACTION_SAVE_SLOT_3   = 3
} SaveMenuAction;

typedef enum {
    ACTION_VS_GAME_OVER_NONE = -1,
    ACTION_VS_GAME_OVER_REPLAY = 0,
    ACTION_VS_GAME_OVER_BACK_TO_MENU = 1
} VsGameOverMenuAction;

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

void process_scores_menu_actions(
    ButtonsList *buttons_list,
    MenuState   *menu_state,
    ScoreBoard  *score_board
);

void process_pause_menu_actions(
    ButtonsList   *buttons_list,
    Game          *game,
    MenuState     *menu_state,
    SaveSlotList  *save_slots
);

void process_load_menu_actions(
    ButtonsList   *buttons_list,
    Game          *game,
    MenuState     *menu_state,
    SaveSlotList  *save_slots
);

void process_save_menu_actions(
    ButtonsList   *buttons_list,
    Game          *game,
    MenuState     *menu_state,
    SaveSlotList  *save_slots
);

void process_vs_game_over_menu_actions(
    ButtonsList *buttons_list,
    Game        *game,
    MenuState   *menu_state,
    WindowSize  *window_size
);

#endif
