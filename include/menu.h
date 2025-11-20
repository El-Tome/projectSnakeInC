#ifndef SNAKE_MENU_H
#define SNAKE_MENU_H

#include "main.h"

void display_menu(
    WindowSize window_size
);

void create_new_game();       // lance la suite d'éxecution pour générer une nouvelle partie
void display_new_game_menu(); // affiche les options de jeu pour la nouvelle partie
void init_new_game();         // initialise partie

void load_game();              // lance la suite d'éxecution pour charger une partie enregistrée
void display_load_game_menu(); // affiche la liste des parties enregistrées
void init_game_from_slot();    // charge la partie enregistrée depuis le slot sélectionné
void delete_game_from_slot();  // supprime la partie enregistrée depuis le slot sélectionné

void start_game();             // lance la suite d'éxecution pour démarrer la partie
void pause_game();             // pause la partie
void resume_game();            // reprend la partie
void quit_game();              // quitte la partie
void save_game();              // sauvegarde la partie
void display_game_over_menu(); // affiche le menu de fin de partie
// a voir si on le fait      void display_settings();     // affiche les paramètres

void display_scores_menu();
void delete_score();



#endif
