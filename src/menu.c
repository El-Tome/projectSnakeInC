#include "menu.h"

void draw_button(Button button, int selected) {
    if (selected) {
        MLV_draw_filled_rectangle(
            button.top_left_x + button.gap_width, 
            button.top_left_y + button.gap_height,
            button.width + 2 * button.gap_width, 
            button.height + 2 * button.gap_height, 
            MLV_COLOR_RED
        );
    }
    MLV_draw_filled_rectangle(
        button.top_left_x, 
        button.top_left_y,
        button.width, 
        button.height, 
        MLV_COLOR_WHITE
    );
    MLV_draw_text(
        button.top_left_x + button.gap_width, 
        button.top_left_y + (button.height / 4), 
        button.text, MLV_COLOR_BLACK);
}

void display_menu(
    WindowSize window_size,
    ButtonsList *buttons_list
) {
    Button btn;
    int total_menu_h;

    /* Dimensions et positions relatives (en pourcentage) */
    btn.width      = window_size.width  / 4;   /* Largeur : 25% de l'écran */
    btn.gap_width  = window_size.width  / 50;  /* Espace horizontal : 2% de l'écran */
    btn.height     = window_size.height / 10;  /* Hauteur : 10% de l'écran */
    btn.gap_height = window_size.height / 50;  /* Espace vertical : 2% de l'écran */

    /* Calcul du point X horizontal pour centrer le bouton avec le point à gauche du bouton de chaque rectangle */
    btn.top_left_x = (window_size.width - btn.width) / 2;
    /* Calcul du point de départ Y pour centrer tout les blocs verticalement */
    total_menu_h   = (4 * btn.height) + (3 * btn.gap_height); /* Taille totale du menu */
    btn.top_left_y = (window_size.height - total_menu_h) / 2;


    /* Affiche les boutons du menu */
    /* Bouton Nouvelle Partie */
    strcpy(btn.text, "Nouvelle Partie");
    draw_button(btn, buttons_list->selected_button == 0);
    /* Ajout du bouton à la liste des boutons */
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;

    /* Bouton Charger une partie */
    strcpy(btn.text, "Charger une partie");
    draw_button(btn, buttons_list->selected_button == 1);
    /* Ajout du bouton à la liste des boutons */
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;

    /* Bouton Scores */
    strcpy(btn.text, "Scores");
    draw_button(btn, buttons_list->selected_button == 2);
    /* Ajout du bouton à la liste des boutons */
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;

    /* Bouton Quitter */
    strcpy(btn.text, "Quitter");
    draw_button(btn, buttons_list->selected_button == 3);
    /* Ajout du bouton à la liste des boutons */
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;
}

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
