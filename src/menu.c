#include "menu.h"

void display_menu(
    WindowSize window_size
) {
    /* Dimensions et positions relatives (en pourcentage) */
    int btn_width = window_size.width  / 4;   /* Largeur : 25% de l'écran */
    int gap_width = window_size.width  / 50;  /* Espace horizontal : 2% de l'écran */
    int btn_height = window_size.height / 10;  /* Hauteur : 10% de l'écran */
    int gap_height = window_size.height / 50;  /* Espace vertical : 2% de l'écran */

    /* Calcul du point X horizontal pour centrer le bouton avec le point à gauche du bouton de chaque rectangle */
    int current_x = (window_size.width - btn_width) / 2;
    
    /* Calcul du point de départ Y pour centrer tout les blocs verticalement */
    int total_menu_h = (4 * btn_height) + (3 * gap_height); /* Taille totale du menu */
    int current_y    = (window_size.height - total_menu_h) / 2;


    /* Affiche les boutons du menu */
    /* Bouton Nouvelle Partie */
    MLV_draw_filled_rectangle(current_x, current_y, btn_width, btn_height, MLV_COLOR_WHITE);
    MLV_draw_text(current_x + gap_width, current_y + (btn_height / 4), "Nouvelle Partie", MLV_COLOR_BLACK);
    
    current_y += btn_height + gap_height;

    /* Bouton Charger une partie */
    MLV_draw_filled_rectangle(current_x, current_y, btn_width, btn_height, MLV_COLOR_WHITE);
    MLV_draw_text(current_x + gap_width, current_y + (btn_height / 4), "Charger une Partie", MLV_COLOR_BLACK);
    
    current_y += btn_height + gap_height;

    /* Bouton Scores */
    MLV_draw_filled_rectangle(current_x, current_y, btn_width, btn_height, MLV_COLOR_WHITE);
    MLV_draw_text(current_x + gap_width, current_y + (btn_height / 4), "Scores", MLV_COLOR_BLACK);
    
    current_y += btn_height + gap_height;

    /* Bouton Quitter */
    MLV_draw_filled_rectangle(current_x, current_y, btn_width, btn_height, MLV_COLOR_WHITE);
    MLV_draw_text(current_x + gap_width, current_y + (btn_height / 4), "Quitter", MLV_COLOR_BLACK);
}

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
