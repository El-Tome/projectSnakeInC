#include "menu_handler.h"
#include "score.h"
/*#include "grille.h"*/

void position_souris(ButtonsList *buttons_list){
    int x, y, n;
    MLV_get_mouse_position(&x, &y);
    for (n = 0; n < buttons_list->nb_buttons; n++) {
        if (
            (x >= buttons_list->buttons[n].top_left_x) &&
            (x <= (buttons_list->buttons[n].top_left_x + buttons_list->buttons[n].width)) &&
            
            (y >= buttons_list->buttons[n].top_left_y &&
             (y <= buttons_list->buttons[n].top_left_y+ buttons_list->buttons[n].height))
            ) {
            buttons_list->selected_button = n;
        }
    }
}

MainMenuAction handle_main_menu_navigation(ButtonsList *buttons_list) {
    ToucheClavier touche;
    MainMenuAction selected_button;

    /* fonct Hugo Position p j'utilise position present dans grille.h */
    
    touche = get_event();
    switch (touche) {
        /* Cas où on appuie sur la touche haut */
    case UP:
        buttons_list->selected_button = (buttons_list->selected_button - 1 + buttons_list->nb_buttons) % buttons_list->nb_buttons;
        selected_button = ACTION_NONE_MAIN;
        break;

        /* Cas où on appuie sur la touche bas */
    case DOWN:
        buttons_list->selected_button = (buttons_list->selected_button + 1) % buttons_list->nb_buttons;
        selected_button = ACTION_NONE_MAIN;
        break;

        /* Cas où on appuie sur la touche entrée ou clic souris */
    case ENTER:
    case MOUSE_LEFT_CLICK:
        selected_button = (MainMenuAction)buttons_list->selected_button;
        break;

        /* Cas où on appuie sur une autre touche */
    default:
        selected_button = ACTION_NONE_MAIN;
        break;
    }

    
    position_souris(buttons_list);
 
    return selected_button;
    
}

  

NewGameMenuAction handle_new_game_menu_navigation(ButtonsList *buttons_list) {
    ToucheClavier touche;
    NewGameMenuAction action = ACTION_NONE_NEW_GAME;

    touche = get_event();
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
        case MOUSE_LEFT_CLICK:
            action = (NewGameMenuAction)buttons_list->selected_button;
            break;

        default:
            action = ACTION_NONE_NEW_GAME;
            break;
    }

    position_souris(buttons_list);
 
    return action;
}

GameOverMenuAction handle_game_over_menu_navigation(ButtonsList *buttons_list) {
    ToucheClavier touche;
    GameOverMenuAction action = ACTION_NONE_GAME_OVER;

    touche = get_event();
    switch (touche) {
        case UP:
            buttons_list->selected_button = (buttons_list->selected_button - 1 + buttons_list->nb_buttons) % buttons_list->nb_buttons;
            break;
        case DOWN:
            buttons_list->selected_button = (buttons_list->selected_button + 1) % buttons_list->nb_buttons;
            break;
        case ENTER:
        case MOUSE_LEFT_CLICK:
            action = (GameOverMenuAction)buttons_list->selected_button;
            break;
        default:
            break;
    }

    position_souris(buttons_list);

    return action;
}

void process_main_menu_actions(ButtonsList *buttons_list, MenuState *menu_state, int *quitter) {
    switch (handle_main_menu_navigation(buttons_list)) {
        case ACTION_NEW_GAME:
            *menu_state = NEW_GAME_MENU;
            buttons_list->selected_button = 0;
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
            buttons_list->selected_button = 0;
            break;
        case ACTION_START_GAME:
            *menu_state = IN_GAME;
            game->state = FREEZE_GAME_MENU;
            init_game(game, window_size);
            break;
        default:
            break;
    }
}

void process_game_over_menu_actions(ButtonsList *buttons_list, Game *game, MenuState *menu_state, WindowSize *window_size) {
    switch(handle_game_over_menu_navigation(buttons_list)) {
        case ACTION_REPLAY:
            game->state = FREEZE_GAME_MENU;
            init_game(game, window_size);
            break;
        case ACTION_BACK_TO_MENU:
            *menu_state = MAIN_MENU;
            buttons_list->selected_button = 0;
            break;
        default:
            break;
    }
}

ScoresMenuAction handle_scores_menu_navigation(ButtonsList *buttons_list, int score_count) {
    ToucheClavier touche;
    ScoresMenuAction action = ACTION_NONE_SCORES;
    int max_index = score_count; /* score_count = index du bouton Retour */

    touche = get_event();
    switch (touche) {
        case UP:
            if (buttons_list->selected_button > 0) {
                buttons_list->selected_button--;
            }
            break;
        case DOWN:
            if (buttons_list->selected_button < max_index) {
                buttons_list->selected_button++;
            }
            break;
        case ENTER:
        case MOUSE_LEFT_CLICK:
            if (buttons_list->selected_button == score_count) {
                action = ACTION_SCORES_BACK;
            } else if (buttons_list->selected_button < score_count) {
                /* Retourne l'index du score a supprimer */
                action = (ScoresMenuAction)buttons_list->selected_button;
            }
            break;
        default:
            break;
    }

    position_souris(buttons_list);

    return action;
}

void process_scores_menu_actions(ButtonsList *buttons_list, MenuState *menu_state, ScoreBoard *score_board) {
    ScoresMenuAction action = handle_scores_menu_navigation(buttons_list, score_board->count);
    
    if (action == ACTION_SCORES_BACK) {
        *menu_state = MAIN_MENU;
        buttons_list->selected_button = 0;
    } else if (action >= 0 && action < score_board->count) {
        /* Supprime le score a l'index donne */
        remove_score(score_board, action);
        save_scores(score_board);
        /* Ajuste la selection si necessaire */
        if (buttons_list->selected_button >= score_board->count && score_board->count > 0) {
            buttons_list->selected_button = score_board->count - 1;
        } else if (score_board->count == 0) {
            buttons_list->selected_button = 0;
        }
    }
}
