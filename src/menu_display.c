#include "menu_display.h"
#include <stdio.h>

void draw_button(Button button, int selected) {
    int text_width, text_height;
    
    if (selected) {
        MLV_draw_filled_rectangle(
            button.top_left_x - (button.gap_width/2), 
            button.top_left_y - (button.gap_height/2),
            button.width + button.gap_width, 
            button.height + button.gap_height, 
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
    
    MLV_get_size_of_text(button.text, &text_width, &text_height);
    MLV_draw_text(
        button.top_left_x + (button.width - text_width) / 2,
        button.top_left_y + (button.height - text_height) / 2,
        button.text, MLV_COLOR_BLACK
    );
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
    buttons_list->nb_buttons = 0;


    /* Affiche les boutons du menu */
    /* Bouton Nouvelle Partie */
    strcpy(btn.text, "Nouvelle Partie");
    draw_button(btn, buttons_list->selected_button == 0);
    /* Ajout du bouton à la liste des boutons */
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;

    /* Changement des position du prochain bouton */
    btn.top_left_y += btn.height + btn.gap_height;
    /* Bouton Charger une partie */
    strcpy(btn.text, "Charger une partie");
    draw_button(btn, buttons_list->selected_button == 1);
    /* Ajout du bouton à la liste des boutons */
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;

    /* Changement des position du prochain bouton */
    btn.top_left_y += btn.height + btn.gap_height;
    /* Bouton Scores */
    strcpy(btn.text, "Scores");
    draw_button(btn, buttons_list->selected_button == 2);
    /* Ajout du bouton à la liste des boutons */
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;

    /* Changement des position du prochain bouton */
    btn.top_left_y += btn.height + btn.gap_height;
    /* Bouton Quitter */
    strcpy(btn.text, "Quitter");
    draw_button(btn, buttons_list->selected_button == 3);
    /* Ajout du bouton à la liste des boutons */
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;
}



void display_new_game_menu(
    WindowSize window_size,
    ButtonsList *buttons_list,
    GameSettings *settings
) {
    Button btn;
    int total_menu_h;
    int total_menu_w;
    int start_x, start_y;

    char size_text[100];
    char speed_text[100];
    int text_width, text_height;
    int text_x, text_y;

    /* Dimensions et positions relatives (en pourcentage) */
    btn.width      = window_size.width  / 6;   /* Largeur : 16% de l'écran */
    btn.gap_width  = window_size.width  / 50;  /* Espace horizontal : 2% de l'écran */
    btn.height     = window_size.height / 15;  /* Hauteur : 6.6% de l'écran */
    btn.gap_height = window_size.height / 40;  /* Espace vertical : 2.5% de l'écran */

    /* Calcul pour centrer le groupe de boutons */
    total_menu_h = (4 * btn.height) + (3 * btn.gap_height); /* 4 lignes de boutons */
    total_menu_w = (2 * btn.width) + (2 * btn.gap_width);   /* 2 colonnes de boutons */
    
    start_y = (window_size.height - total_menu_h) / 2;
    start_x = (window_size.width - total_menu_w) / 2;
    
    /* Réinitialiser le compteur de boutons */
    buttons_list->nb_buttons = 0;

    /* LIGNE 1 : Mode 2 joueurs et Murs */
    btn.top_left_x = start_x;
    btn.top_left_y = start_y;
    strcpy(btn.text, settings->is_two_players ? "2 Joueurs: Oui" : "2 Joueurs: Non");
    draw_button(btn, buttons_list->selected_button == 0);
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;

    btn.top_left_x += btn.width + btn.gap_width * 2;
    strcpy(btn.text, settings->has_walls ? "Murs: Oui" : "Murs: Non");
    draw_button(btn, buttons_list->selected_button == 1);
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;

    /* LIGNE 2 : Taille X de la grille */
    /* Affichage de la taille actuelle de la grille */
    
    sprintf(size_text, "Taille: %d x %d", settings->width, settings->height);
    MLV_get_size_of_text(size_text, &text_width, &text_height);
    
    text_x = start_x + total_menu_w / 2 - text_width / 2;
    text_y = start_y + btn.height + btn.gap_height / 2 - text_height / 2;
    
    MLV_draw_text(text_x, text_y, size_text, MLV_COLOR_WHITE);

    btn.top_left_x = start_x;
    btn.top_left_y += btn.height + btn.gap_height;
    strcpy(btn.text, "Largeur -");
    draw_button(btn, buttons_list->selected_button == 2);
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;

    btn.top_left_x += btn.width + btn.gap_width * 2;
    strcpy(btn.text, "Largeur +");
    draw_button(btn, buttons_list->selected_button == 3);
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;

    /* LIGNE 3 : Taille Y de la grille */
    btn.top_left_x = start_x;
    btn.top_left_y += btn.height + btn.gap_height;
    strcpy(btn.text, "Hauteur -");
    draw_button(btn, buttons_list->selected_button == 4);
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;

    btn.top_left_x += btn.width + btn.gap_width * 2;
    strcpy(btn.text, "Hauteur +");
    draw_button(btn, buttons_list->selected_button == 5);
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;

    /* LIGNE 4 : Vitesse (centrée) */
    sprintf(speed_text, "Vitesse: %d", settings->speed);
    MLV_get_size_of_text(speed_text, &text_width, &text_height);
    
    text_x = start_x + total_menu_w / 2 - text_width / 2;
    text_y = start_y + btn.height * 3 + btn.gap_height * 2 + btn.gap_height / 2 - text_height / 2;
    
    MLV_draw_text(text_x, text_y, speed_text, MLV_COLOR_WHITE);
    
    btn.top_left_x = start_x;
    btn.top_left_y += btn.height + btn.gap_height;
    strcpy(btn.text, "Vitesse -");
    draw_button(btn, buttons_list->selected_button == 6);
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;

    btn.top_left_x += btn.width + btn.gap_width * 2;
    strcpy(btn.text, "Vitesse +");
    draw_button(btn, buttons_list->selected_button == 7);
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;

    /* BOUTON RETOUR : En bas à gauche */
    btn.top_left_x = btn.gap_width * 2;
    btn.top_left_y = window_size.height - btn.height - (btn.gap_height * 2);
    strcpy(btn.text, "Retour");
    draw_button(btn, buttons_list->selected_button == 8);
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;

    /* BOUTON LANCER : En bas à droite */
    btn.top_left_x = window_size.width - btn.width - (btn.gap_width * 2);
    btn.top_left_y = window_size.height - btn.height - (btn.gap_height * 2);
    strcpy(btn.text, "Lancer");
    draw_button(btn, buttons_list->selected_button == 9);
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;
}

void display_game_over_menu(
    WindowSize window_size,
    ButtonsList *buttons_list,
    Game *game
) {
    Button btn;
    int total_menu_h;
    char score_text[50];
    int text_width, text_height;

    game->score = (game->snake.length - game->settings.initial_length) * game->settings.speed;

    /* Affichage du titre "GAME OVER" */
    MLV_get_size_of_text("GAME OVER", &text_width, &text_height);
    MLV_draw_text(
        (window_size.width - text_width) / 2,
        window_size.height / 4,
        "GAME OVER", MLV_COLOR_RED
    );

    /* Affichage du score */
    sprintf(score_text, "Score: %d", game->score);
    MLV_get_size_of_text(score_text, &text_width, &text_height);
    MLV_draw_text(
        (window_size.width - text_width) / 2,
        window_size.height / 3,
        score_text, MLV_COLOR_WHITE
    );

    /* Dimensions et positions relatives (en pourcentage) */
    btn.width      = window_size.width  / 4;   /* Largeur : 25% de l'écran */
    btn.gap_width  = window_size.width  / 50;  /* Espace horizontal : 2% de l'écran */
    btn.height     = window_size.height / 10;  /* Hauteur : 10% de l'écran */
    btn.gap_height = window_size.height / 50;  /* Espace vertical : 2% de l'écran */

    /* Calcul du point X horizontal pour centrer le bouton */
    btn.top_left_x = (window_size.width - btn.width) / 2;
    
    /* Centrage vertical des deux boutons */
    total_menu_h   = (2 * btn.height) + (1 * btn.gap_height);
    btn.top_left_y = (window_size.height - total_menu_h) / 2 + btn.height;

    buttons_list->nb_buttons = 0;

    /* Bouton Rejouer */
    strcpy(btn.text, "Rejouer");
    draw_button(btn, buttons_list->selected_button == 0);
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;

    /* Bouton Retour au Menu */
    btn.top_left_y += btn.height + btn.gap_height;
    strcpy(btn.text, "Retour au Menu");
    draw_button(btn, buttons_list->selected_button == 1);
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;
}

void display_scores_menu(
    WindowSize window_size,
    ButtonsList *buttons_list,
    ScoreBoard *score_board
) {
    Button btn;
    int title_width, title_height;
    int i;
    int start_y;
    int line_height;
    char line_text[150];
    int text_width, text_height;
    const ScoreEntry *entry;
    MLV_Color color;

    /* Affichage du titre */
    MLV_get_size_of_text("Meilleurs Scores", &title_width, &title_height);
    MLV_draw_text(
        (window_size.width - title_width) / 2,
        window_size.height / 10,
        "Meilleurs Scores", MLV_COLOR_WHITE
    );

    /* Parametres d'affichage de la liste */
    line_height = window_size.height / 18;
    start_y = window_size.height / 5;

    buttons_list->nb_buttons = 0;

    /* Affichage des scores (chaque score est selectionnable) */
    if (score_board->count == 0) {
        MLV_get_size_of_text("Aucun score enregistre", &text_width, &text_height);
        MLV_draw_text(
            (window_size.width - text_width) / 2,
            start_y + line_height * 2,
            "Aucun score enregistre", MLV_COLOR_GREY
        );
    } else {
        for (i = 0; i < score_board->count; i++) {
            entry = get_score_entry(score_board, i);
            if (entry != NULL) {
                sprintf(line_text, "%d. Score: %d  |  %dx%d  |  Vitesse: %d  |  Murs: %s",
                    i + 1,
                    entry->score,
                    entry->width,
                    entry->height,
                    entry->speed,
                    entry->has_walls ? "Oui" : "Non"
                );
                MLV_get_size_of_text(line_text, &text_width, &text_height);
                
                /* Surbrillance si selectionne */
                color = (buttons_list->selected_button == i) ? MLV_COLOR_RED : MLV_COLOR_WHITE;
                
                MLV_draw_text(
                    (window_size.width - text_width) / 2,
                    start_y + line_height * i,
                    line_text, color
                );
                
                /* Enregistre comme bouton cliquable */
                btn.top_left_x = (window_size.width - text_width) / 2;
                btn.top_left_y = start_y + line_height * i;
                btn.width = text_width;
                btn.height = text_height;
                buttons_list->buttons[buttons_list->nb_buttons] = btn;
                buttons_list->nb_buttons++;
            }
        }
    }

    /* Bouton Retour en bas */
    btn.width      = window_size.width  / 6;
    btn.gap_width  = window_size.width  / 50;
    btn.height     = window_size.height / 15;
    btn.gap_height = window_size.height / 40;

    btn.top_left_x = (window_size.width - btn.width) / 2;
    btn.top_left_y = window_size.height - btn.height - (btn.gap_height * 4);
    
    strcpy(btn.text, "Retour");
    draw_button(btn, buttons_list->selected_button == score_board->count);
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;
}
