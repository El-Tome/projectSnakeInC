#ifndef CLASSIC_MODE_H
#define CLASSIC_MODE_H

#include "main.h"
#include "game.h"
#include "menu_handler.h"
#include "menu_display.h"
#include "save.h"

void process_classic_mode(
    Game         *game, 
    WindowSize   *window_size, 
    ButtonsList  *buttons_list, 
    int          *nb_frames,
    MenuState    *menu_state, 
    ScoreBoard   *score_board, 
    SaveSlotList *save_slots
);

#endif
