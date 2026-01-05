#ifndef VS_MODE_H
#define VS_MODE_H

#include "main.h"
#include "game.h"
#include "menu_handler.h"
#include "menu_display.h"


/* Boucle principale du mode VS */
void process_vs_mode(
    Game         *game, 
    WindowSize   *window_size, 
    ButtonsList  *buttons_list, 
    int          *nb_frames,
    MenuState    *menu_state
);

/* Libère les ressources du mode VS */
void free_vs_game(Game *game);

/* Dessine le jeu en mode VS (les 2 serpents) */
void draw_vs_game(Game *game, WindowSize *window_size);

#endif

