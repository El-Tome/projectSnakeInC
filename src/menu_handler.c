#include "menu_handler.h"

MainMenuAction handle_main_menu_navigation(ButtonsList *buttons_list) {
    ToucheClavier touche;
    MainMenuAction selected_button;

    touche = convert_key_to_enum(get_key_pressed());
    switch (touche) {
        /* Cas où on appuie sur la touche haut */
        case UP:
            buttons_list->selected_button = (buttons_list->selected_button - 1 + buttons_list->nb_buttons) % buttons_list->nb_buttons;
            selected_button = ACTION_NONE_MAIN;
            break;

        /* Cas où on appuie sur la touche bas */
        case DOWN:
        buttons_list->selected_button = (buttons_list->selected_button + 1 + buttons_list->nb_buttons) % buttons_list->nb_buttons;
            selected_button = ACTION_NONE_MAIN;
            break;

        /* Cas où on appuie sur la touche entrée */
        case ENTER:
            selected_button = (MainMenuAction)buttons_list->selected_button;
            break;

        /* Cas où on appuie sur une autre touche */
        default:
            selected_button = ACTION_NONE_MAIN;
            break;
    }

    return selected_button;
    
}

NewGameMenuAction handle_new_game_menu_navigation(ButtonsList *buttons_list) {
    ToucheClavier touche;
    NewGameMenuAction action = ACTION_NONE_NEW_GAME;

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
            action = (NewGameMenuAction)buttons_list->selected_button;
            break;

        default:
            action = ACTION_NONE_NEW_GAME;
            break;
    }

    return action;
}

void process_main_menu_actions(ButtonsList *buttons_list, MenuState *menu_state, int *quitter) {
    switch (handle_main_menu_navigation(buttons_list)) {
        case ACTION_NEW_GAME:
            *menu_state = NEW_GAME_MENU;
            buttons_list->selected_button = 1;
            break;
        case ACTION_LOAD_GAME:
            *menu_state = LOAD_GAME_MENU;
            break;
        case ACTION_SCORES:
            *menu_state = SCORES_MENU;
            break;
        case ACTION_QUIT:
            *quitter = 1;
            break;
        default:
            break;
    }
}

void process_new_game_menu_actions(ButtonsList *buttons_list, Game *game, MenuState *menu_state, WindowSize *window_size) {
    switch(handle_new_game_menu_navigation(buttons_list)) {
        case ACTION_TOGGLE_2_PLAYERS:
            game->settings.is_two_players = !game->settings.is_two_players;
            break;
        case ACTION_TOGGLE_WALLS:
            game->settings.has_walls = !game->settings.has_walls;
            break;
        case ACTION_DECR_WIDTH:
            if (game->settings.width > MIN_GRID_WIDTH) {
                game->settings.width--;
            }
            break;
        case ACTION_INCR_WIDTH:
            if (game->settings.width < MAX_GRID_WIDTH) {
                game->settings.width++;
            }
            break;
        case ACTION_DECR_HEIGHT:
            if (game->settings.height > MIN_GRID_HEIGHT) {
                game->settings.height--;
            }
            break;
        case ACTION_INCR_HEIGHT:
            if (game->settings.height < MAX_GRID_HEIGHT) {
                game->settings.height++;
            }
            break;
        case ACTION_DECR_SPEED:
            if (game->settings.speed > MIN_SPEED) {
                game->settings.speed--;
            }
            break;
        case ACTION_INCR_SPEED:
            if (game->settings.speed < MAX_SPEED) {
                game->settings.speed++;
            }
            break;
        case ACTION_BACK_MAIN:
            *menu_state = MAIN_MENU;
            break;
        case ACTION_START_GAME:
            *menu_state = IN_GAME;
            init_game(game, window_size);
            break;
        default:
            break;
    }
}
