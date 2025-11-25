#include "menu.h"

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


int handle_main_menu_navigation(ButtonsList *buttons_list) {
    ToucheClavier touche;
    int selected_button;

    touche = convert_key_to_enum(get_key_pressed());
    switch (touche) {
        /* Cas où on appuie sur la touche haut */
        case UP:
            buttons_list->selected_button = (buttons_list->selected_button - 1 + buttons_list->nb_buttons) % buttons_list->nb_buttons;
            selected_button = -1;
            break;

        /* Cas où on appuie sur la touche bas */
        case DOWN:
        buttons_list->selected_button = (buttons_list->selected_button + 1 + buttons_list->nb_buttons) % buttons_list->nb_buttons;
            selected_button = -1;
            break;

        /* Cas où on appuie sur la touche entrée */
        case ENTER:
            selected_button = buttons_list->selected_button;
            break;

        /* Cas où on appuie sur une autre touche */
        default:
            selected_button = -1;
            break;
    }

    return selected_button;
    
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

int handle_new_game_menu_navigation(ButtonsList *buttons_list) {
    ToucheClavier touche;
    int action = -1;

    touche = convert_key_to_enum(get_key_pressed());
    switch (touche) {
        case UP:
            if (buttons_list->selected_button >= 8) { /* Depuis Retour ou Lancer */
                buttons_list->selected_button -= 2; /* Remonte à la grille */
            } else if (buttons_list->selected_button >= 2) {
                buttons_list->selected_button -= 2;
            }
            break;

        case DOWN:
            if (buttons_list->selected_button < 6) {
                buttons_list->selected_button += 2;
            } else if (buttons_list->selected_button < 8) {
                /* Depuis la dernière ligne de la grille, va vers Retour ou Lancer */
                /* Le bouton de gauche (6) va vers Retour (8), le droit (7) vers Lancer (9) */
                buttons_list->selected_button += 2;
            }
            break;
            
        case LEFT:
            if (buttons_list->selected_button == 9) {
                 buttons_list->selected_button = 8; /* Lancer -> Retour */
            } else if (buttons_list->selected_button % 2 == 1) {
                buttons_list->selected_button -= 1;
            }
            break;
            
        case RIGHT:
            if (buttons_list->selected_button == 8) {
                 buttons_list->selected_button = 9; /* Retour -> Lancer */
            } else if (buttons_list->selected_button % 2 == 0 && buttons_list->selected_button < 8) {
                buttons_list->selected_button += 1;
            }
            break;

        case ENTER:
            action = buttons_list->selected_button;
            break;

        default:
            action = -1;
            break;
    }

    return action;
}


void create_new_game();       /* lance la suite d'éxecution pour générer une nouvelle partie */
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
