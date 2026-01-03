#include "menu_handler.h"
#include "score.h"
#include "save.h"
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

PauseMenuAction handle_pause_menu_navigation(ButtonsList *buttons_list) {
    ToucheClavier touche;
    PauseMenuAction action = ACTION_NONE_PAUSE;

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
            action = (PauseMenuAction)buttons_list->selected_button;
            break;
        case ESCAPE:
            /* Echap pour reprendre rapidement */
            action = ACTION_RESUME;
            break;
        default:
            break;
    }

    position_souris(buttons_list);

    return action;
}

void process_pause_menu_actions(
    ButtonsList   *buttons_list,
    Game          *game,
    MenuState     *menu_state,
    SaveSlotList  *save_slots
) {
    PauseMenuAction action = handle_pause_menu_navigation(buttons_list);
    int slot_to_save;
    int i;
    int all_slots_full;
    
    switch (action) {
        case ACTION_RESUME:
            game->state = FREEZE_GAME_MENU;
            buttons_list->selected_button = 0;
            break;
        case ACTION_SAVE_GAME:
            /* Recharge la liste des slots pour etre a jour */
            load_save_slot_list(save_slots);
            
            /* Trouve le premier slot libre */
            slot_to_save = -1;
            all_slots_full = 1;
            for (i = 0; i < MAX_SAVE_SLOTS; i++) {
                if (!save_slots->slot_used[i]) {
                    slot_to_save = i;
                    all_slots_full = 0;
                    break;
                }
            }
            
            /* Si tous les slots sont utilises, on affiche le menu de choix */
            if (all_slots_full) {
                game->state = SAVE_MENU;
                buttons_list->selected_button = 0;
            } else {
                /* Sauvegarde dans le slot libre */
                if (save_game(game, slot_to_save) == FILE_UTILS_SUCCESS) {
                    load_save_slot_list(save_slots);
                }
                *menu_state = MAIN_MENU;
                buttons_list->selected_button = 0;
            }
            break;
        case ACTION_PAUSE_TO_MENU:
            *menu_state = MAIN_MENU;
            buttons_list->selected_button = 0;
            break;
        default:
            break;
    }
}

LoadMenuAction handle_load_menu_navigation(ButtonsList *buttons_list, SaveSlotList *save_slots) {
    ToucheClavier touche;
    LoadMenuAction action = ACTION_NONE_LOAD;
    int max_index = buttons_list->nb_buttons - 1; /* Dernier bouton = Retour */

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
            if (buttons_list->selected_button == max_index) {
                action = ACTION_LOAD_BACK;
            } else {
                /* Determine quel bouton a ete clique */
                /* La structure est : Slot0, [Delete0], Slot1, [Delete1], ... */
                /* Il faut determiner si c'est un slot ou un bouton supprimer */
                int btn_index = 0;
                int i;
                for (i = 0; i < MAX_SAVE_SLOTS; i++) {
                    if (buttons_list->selected_button == btn_index) {
                        /* C'est un bouton de slot */
                        if (save_slots->slot_used[i]) {
                            action = (LoadMenuAction)i; /* ACTION_LOAD_SLOT_x */
                        }
                        break;
                    }
                    btn_index++;
                    
                    if (save_slots->slot_used[i]) {
                        if (buttons_list->selected_button == btn_index) {
                            /* C'est un bouton supprimer */
                            action = (LoadMenuAction)(10 + i); /* ACTION_DELETE_SLOT_x */
                            break;
                        }
                        btn_index++;
                    }
                }
            }
            break;
        case ESCAPE:
            action = ACTION_LOAD_BACK;
            break;
        default:
            break;
    }

    position_souris(buttons_list);

    return action;
}

void process_load_menu_actions(
    ButtonsList   *buttons_list,
    Game          *game,
    MenuState     *menu_state,
    SaveSlotList  *save_slots
) {
    LoadMenuAction action = handle_load_menu_navigation(buttons_list, save_slots);
    int slot;
    
    if (action == ACTION_LOAD_BACK) {
        *menu_state = MAIN_MENU;
        buttons_list->selected_button = 0;
    } else if (action >= ACTION_LOAD_SLOT_0 && action <= ACTION_LOAD_SLOT_3) {
        /* Chargement d'une sauvegarde */
        slot = action - ACTION_LOAD_SLOT_0;
        if (save_slots->slot_used[slot]) {
            if (load_game(game, slot) == FILE_UTILS_SUCCESS) {
                *menu_state = IN_GAME;
                game->state = FREEZE_GAME_MENU;
            }
        }
        buttons_list->selected_button = 0;
    } else if (action >= ACTION_DELETE_SLOT_0 && action <= ACTION_DELETE_SLOT_3) {
        /* Suppression d'une sauvegarde */
        slot = action - ACTION_DELETE_SLOT_0;
        if (save_slots->slot_used[slot]) {
            delete_save(slot);
            load_save_slot_list(save_slots);
            /* Repositionne la selection */
            if (buttons_list->selected_button >= buttons_list->nb_buttons) {
                buttons_list->selected_button = buttons_list->nb_buttons - 1;
            }
        }
    }
}

SaveMenuAction handle_save_menu_navigation(ButtonsList *buttons_list) {
    ToucheClavier touche;
    SaveMenuAction action = ACTION_NONE_SAVE;

    touche = get_event();
    switch (touche) {
        case UP:
            if (buttons_list->selected_button > 0) {
                buttons_list->selected_button--;
            }
            break;
        case DOWN:
            if (buttons_list->selected_button < MAX_SAVE_SLOTS) {
                buttons_list->selected_button++;
            }
            break;
        case ENTER:
        case MOUSE_LEFT_CLICK:
            if (buttons_list->selected_button == MAX_SAVE_SLOTS) {
                action = ACTION_SAVE_CANCEL;
            } else {
                action = (SaveMenuAction)buttons_list->selected_button;
            }
            break;
        case ESCAPE:
            action = ACTION_SAVE_CANCEL;
            break;
        default:
            break;
    }

    position_souris(buttons_list);

    return action;
}

void process_save_menu_actions(
    ButtonsList   *buttons_list,
    Game          *game,
    MenuState     *menu_state,
    SaveSlotList  *save_slots
) {
    SaveMenuAction action = handle_save_menu_navigation(buttons_list);
    int slot;
    
    if (action == ACTION_SAVE_CANCEL) {
        /* Retour au menu pause */
        game->state = PAUSE_MENU;
        buttons_list->selected_button = 0;
    } else if (action >= ACTION_SAVE_SLOT_0 && action <= ACTION_SAVE_SLOT_3) {
        /* Sauvegarde dans le slot choisi (ecrase) */
        slot = action - ACTION_SAVE_SLOT_0;
        if (save_game(game, slot) == FILE_UTILS_SUCCESS) {
            load_save_slot_list(save_slots);
        }
        *menu_state = MAIN_MENU;
        buttons_list->selected_button = 0;
    }
}
