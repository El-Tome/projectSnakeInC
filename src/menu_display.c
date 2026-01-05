#include "menu_display.h"
#include <stdio.h>
#include <time.h>

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
    int  total_menu_h;
    char score_text[50];
    int  text_width, text_height;

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
    start_y     = window_size.height / 5;

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
            /* Accès direct au tableau sans pointeur intermediaire */
            sprintf(
                line_text, 
                "%d. Score: %d  |  %dx%d  |  Vitesse: %d  |  Murs: %s",
                i + 1,
                score_board->entries[i].score,
                score_board->entries[i].width,
                score_board->entries[i].height,
                score_board->entries[i].speed,
                score_board->entries[i].has_walls ? "Oui" : "Non"
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
            btn.width      = text_width;
            btn.height     = text_height;
            buttons_list->buttons[buttons_list->nb_buttons] = btn;
            buttons_list->nb_buttons++;
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

void display_pause_menu(
    WindowSize window_size,
    ButtonsList *buttons_list
) {
    Button btn;
    int total_menu_h;
    int text_width, text_height;

    /* Affichage du titre "PAUSE" */
    MLV_get_size_of_text("PAUSE", &text_width, &text_height);
    MLV_draw_text(
        (window_size.width - text_width) / 2,
        window_size.height / 4,
        "PAUSE", MLV_COLOR_YELLOW
    );

    /* Dimensions et positions relatives */
    btn.width      = window_size.width  / 4;
    btn.gap_width  = window_size.width  / 50;
    btn.height     = window_size.height / 10;
    btn.gap_height = window_size.height / 50;

    /* Calcul du point X horizontal pour centrer le bouton */
    btn.top_left_x = (window_size.width - btn.width) / 2;
    
    /* Centrage vertical des trois boutons */
    total_menu_h   = (3 * btn.height) + (2 * btn.gap_height);
    btn.top_left_y = (window_size.height - total_menu_h) / 2;

    buttons_list->nb_buttons = 0;

    /* Bouton Reprendre */
    strcpy(btn.text, "Reprendre");
    draw_button(btn, buttons_list->selected_button == 0);
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;

    /* Bouton Enregistrer */
    btn.top_left_y += btn.height + btn.gap_height;
    strcpy(btn.text, "Enregistrer");
    draw_button(btn, buttons_list->selected_button == 1);
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;

    /* Bouton Menu Principal */
    btn.top_left_y += btn.height + btn.gap_height;
    strcpy(btn.text, "Menu Principal");
    draw_button(btn, buttons_list->selected_button == 2);
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;
}

void display_load_menu(
    WindowSize window_size,
    ButtonsList *buttons_list,
    SaveSlotList *save_slots
) {
    Button btn;
    Button delete_btn;
    int title_width, title_height;
    int i;
    int start_y;
    int line_height;
    char line_text[200];
    char date_text[50];
    int text_width, text_height;
    int slot_index;

    /* Affichage du titre */
    MLV_get_size_of_text("Charger une Partie", &title_width, &title_height);
    MLV_draw_text(
        (window_size.width - title_width) / 2,
        window_size.height / 10,
        "Charger une Partie", MLV_COLOR_WHITE
    );

    /* Dimensions des boutons */
    btn.width      = window_size.width  / 3;
    btn.gap_width  = window_size.width  / 50;
    btn.height     = window_size.height / 12;
    btn.gap_height = window_size.height / 40;

    delete_btn.width  = window_size.width / 10;
    delete_btn.height = btn.height;
    delete_btn.gap_width  = btn.gap_width;
    delete_btn.gap_height = btn.gap_height;

    /* Parametres d'affichage de la liste */
    line_height = btn.height + btn.gap_height;
    start_y = window_size.height / 4;

    buttons_list->nb_buttons = 0;
    slot_index = 0;

    /* Affichage des 4 slots de sauvegarde */
    for (i = 0; i < MAX_SAVE_SLOTS; i++) {
        /* Bouton de chargement du slot */
        btn.top_left_x = (window_size.width - btn.width - delete_btn.width - btn.gap_width) / 2;
        btn.top_left_y = start_y + line_height * i;

        if (save_slots->slot_used[i]) {
            /* Format de la date - on utilise directement le resultat de localtime */
            strftime(
                date_text, /* Date formatée */
                sizeof(date_text), /* Taille du buffer */
                "%d/%m/%Y %H:%M", /* Format de la date */
                localtime(&save_slots->previews[i].timestamp) /* Timestamp */
            );

            sprintf(line_text, "Slot %d: %dx%d | Vit: %d | Score: %d | %s",
                i + 1,
                save_slots->previews[i].settings.width,
                save_slots->previews[i].settings.height,
                save_slots->previews[i].settings.speed,
                save_slots->previews[i].score,
                date_text
            );
        } else {
            sprintf(line_text, "Slot %d: Vide", i + 1);
        }

        strcpy(btn.text, line_text);
        
        /* Dessiner le bouton de slot */
        if (buttons_list->selected_button == slot_index) {
            MLV_draw_filled_rectangle(
                btn.top_left_x - (btn.gap_width/2), 
                btn.top_left_y - (btn.gap_height/2),
                btn.width + btn.gap_width, 
                btn.height + btn.gap_height, 
                MLV_COLOR_RED
            );
        }
        MLV_draw_filled_rectangle(
            btn.top_left_x, btn.top_left_y,
            btn.width, btn.height, 
            save_slots->slot_used[i] ? MLV_COLOR_WHITE : MLV_COLOR_DIM_GREY
        );
        
        MLV_get_size_of_text(btn.text, &text_width, &text_height);
        MLV_draw_text(
            btn.top_left_x + (btn.width - text_width) / 2,
            btn.top_left_y + (btn.height - text_height) / 2,
            btn.text, MLV_COLOR_BLACK
        );
        
        buttons_list->buttons[buttons_list->nb_buttons] = btn;
        buttons_list->nb_buttons++;
        slot_index++;

        /* Bouton supprimer (seulement si le slot est utilise) */
        if (save_slots->slot_used[i]) {
            delete_btn.top_left_x = btn.top_left_x + btn.width + btn.gap_width;
            delete_btn.top_left_y = btn.top_left_y;
            strcpy(delete_btn.text, "X");
            
            if (buttons_list->selected_button == slot_index) {
                MLV_draw_filled_rectangle(
                    delete_btn.top_left_x - (delete_btn.gap_width/2), 
                    delete_btn.top_left_y - (delete_btn.gap_height/2),
                    delete_btn.width + delete_btn.gap_width, 
                    delete_btn.height + delete_btn.gap_height, 
                    MLV_COLOR_RED
                );
            }
            MLV_draw_filled_rectangle(
                delete_btn.top_left_x, delete_btn.top_left_y,
                delete_btn.width, delete_btn.height, 
                MLV_COLOR_DARK_RED
            );
            
            MLV_get_size_of_text(delete_btn.text, &text_width, &text_height);
            MLV_draw_text(
                delete_btn.top_left_x + (delete_btn.width - text_width) / 2,
                delete_btn.top_left_y + (delete_btn.height - text_height) / 2,
                delete_btn.text, MLV_COLOR_WHITE
            );
            
            buttons_list->buttons[buttons_list->nb_buttons] = delete_btn;
            buttons_list->nb_buttons++;
            slot_index++;
        }
    }

    /* Bouton Retour en bas */
    btn.width      = window_size.width  / 6;
    btn.height     = window_size.height / 15;
    btn.top_left_x = (window_size.width - btn.width) / 2;
    btn.top_left_y = window_size.height - btn.height - (btn.gap_height * 4);
    
    strcpy(btn.text, "Retour");
    draw_button(btn, buttons_list->selected_button == slot_index);
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;
}

void display_save_menu(
    WindowSize window_size,
    ButtonsList *buttons_list,
    SaveSlotList *save_slots
) {
    Button btn;
    int title_width, title_height;
    int i;
    int start_y;
    int line_height;
    char line_text[200];
    char date_text[50];
    int text_width, text_height;

    /* Affichage du titre */
    MLV_get_size_of_text("Choisir un slot a ecraser", &title_width, &title_height);
    MLV_draw_text(
        (window_size.width - title_width) / 2,
        window_size.height / 10,
        "Choisir un slot a ecraser", MLV_COLOR_ORANGE
    );

    /* Sous-titre d'avertissement */
    MLV_get_size_of_text("Tous les slots sont pleins!", &text_width, &text_height);
    MLV_draw_text(
        (window_size.width - text_width) / 2,
        window_size.height / 10 + title_height + 10,
        "Tous les slots sont pleins!", MLV_COLOR_RED
    );

    /* Dimensions des boutons */
    btn.width      = window_size.width  / 3;
    btn.gap_width  = window_size.width  / 50;
    btn.height     = window_size.height / 12;
    btn.gap_height = window_size.height / 40;

    /* Parametres d'affichage de la liste */
    line_height = btn.height + btn.gap_height;
    start_y = window_size.height / 4;

    buttons_list->nb_buttons = 0;

    /* Affichage des 4 slots de sauvegarde */
    for (i = 0; i < MAX_SAVE_SLOTS; i++) {
        btn.top_left_x = (window_size.width - btn.width) / 2;
        btn.top_left_y = start_y + line_height * i;

        /* Format de la date - on utilise directement le resultat de localtime */
        strftime(
            date_text, /* Date formatée */
            sizeof(date_text), /* Taille du buffer */
            "%d/%m/%Y %H:%M", /* Format de la date */
            localtime(&save_slots->previews[i].timestamp) /* Timestamp */
        );
        
        sprintf(line_text, "Slot %d: %dx%d | Vit: %d | Score: %d | %s",
            i + 1,
            save_slots->previews[i].settings.width,
            save_slots->previews[i].settings.height,
            save_slots->previews[i].settings.speed,
            save_slots->previews[i].score,
            date_text
        );

        strcpy(btn.text, line_text);
        draw_button(btn, buttons_list->selected_button == i);
        
        buttons_list->buttons[buttons_list->nb_buttons] = btn;
        buttons_list->nb_buttons++;
    }

    /* Bouton Annuler en bas */
    btn.width      = window_size.width  / 6;
    btn.height     = window_size.height / 15;
    btn.top_left_x = (window_size.width - btn.width) / 2;
    btn.top_left_y = window_size.height - btn.height - (btn.gap_height * 4);
    
    strcpy(btn.text, "Annuler");
    draw_button(btn, buttons_list->selected_button == MAX_SAVE_SLOTS);
    buttons_list->buttons[buttons_list->nb_buttons] = btn;
    buttons_list->nb_buttons++;
}

void display_vs_game_over_menu(WindowSize window_size, ButtonsList *buttons_list, Game *game) {
    Button btn;
    int total_menu_h;
    char result_text[100];
    int text_width, text_height;

    /* Déterminer le texte du résultat */
    switch (game->vs_result) {
        case VS_RESULT_P1_WINS:
            strcpy(result_text, "JOUEUR 1 GAGNE !");
            break;
        case VS_RESULT_P2_WINS:
            strcpy(result_text, "JOUEUR 2 GAGNE !");
            break;
        case VS_RESULT_DRAW:
            strcpy(result_text, "EGALITE !");
            break;
        default:
            strcpy(result_text, "FIN DE PARTIE");
            break;
    }

    /* Affichage du résultat (titre) */
    MLV_get_size_of_text(result_text, &text_width, &text_height);
    MLV_draw_text(
        (window_size.width - text_width) / 2,
        window_size.height / 4,
        result_text, MLV_COLOR_YELLOW
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
