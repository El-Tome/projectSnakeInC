#ifndef SNAKE_MENU_H
#define SNAKE_MENU_H

#define MAX_BUTTONS 10
#define MAX_TEXT_LENGTH 100

#include "main.h"
#include <string.h>
#include <MLV/MLV_all.h>

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

void change_selected_button(ButtonsList *buttons_list, int selected_button);

void create_new_game();       /* lance la suite d'éxecution pour générer une nouvelle partie */
void display_new_game_menu(); /* affiche les options de jeu pour la nouvelle partie */
void init_new_game();         /* initialise partie */

void load_game();              /* lance la suite d'éxecution pour charger une partie enregistrée */
void display_load_game_menu(); /* affiche la liste des parties enregistrées */
void init_game_from_slot();    /* charge la partie enregistrée depuis le slot sélectionné */
void delete_game_from_slot();  /* supprime la partie enregistrée depuis le slot sélectionné */

void start_game();             /* lance la suite d'éxecution pour démarrer la partie */
void pause_game();             /* pause la partie */
void resume_game();            /* reprend la partie */
void quit_game();              /* quitte la partie */
void save_game();              /* sauvegarde la partie */
void display_game_over_menu(); /* affiche le menu de fin de partie */
/* a voir si on le fait      void display_settings(); */  /*  affiche les paramètres */

void display_scores_menu();
void delete_score();



#endif
