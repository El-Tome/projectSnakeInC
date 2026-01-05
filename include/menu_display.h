#ifndef MENU_DISPLAY_H
#define MENU_DISPLAY_H

#define MAX_BUTTONS 20
#define MAX_TEXT_LENGTH 100

#include "main.h"
#include <string.h>
#include <MLV/MLV_all.h>
#include "game.h"
#include "score.h"
#include "save.h"


typedef struct {
    int top_left_x;
    int top_left_y;
    int width;
    int height;
    int gap_width;
    int gap_height;
    char text[MAX_TEXT_LENGTH];
} Button;

typedef struct {
    Button buttons[MAX_BUTTONS];
    int nb_buttons;
    int selected_button;
} ButtonsList;




void display_menu(
    WindowSize window_size,
    ButtonsList *buttons_list
);

void display_new_game_menu(
    WindowSize window_size,
    ButtonsList *buttons_list,
    GameSettings *settings
);

void display_game_over_menu(
    WindowSize window_size,
    ButtonsList *buttons_list,
    Game *game
);

void display_scores_menu(
    WindowSize window_size,
    ButtonsList *buttons_list,
    ScoreBoard *score_board
);

void display_pause_menu(
    WindowSize window_size,
    ButtonsList *buttons_list
);

void display_load_menu(
    WindowSize window_size,
    ButtonsList *buttons_list,
    SaveSlotList *save_slots
);

void display_save_menu(
    WindowSize window_size,
    ButtonsList *buttons_list,
    SaveSlotList *save_slots
);

void display_vs_game_over_menu(WindowSize window_size, ButtonsList *buttons_list, Game *game);

#endif
