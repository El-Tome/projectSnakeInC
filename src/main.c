#include "main.h"
#include "controls.h"
#include "menu_handler.h"
#include "menu_display.h"
#include <time.h>
#include "game.h"
#include "snake.h"
#include "score.h"
#include "save.h"
#include "classic_mode.h"

/* initialisation des variables */
void init_game_defaults(Game *game) {
    game->settings.width  = 20;
    game->settings.height = 15;
    game->settings.speed  = 15; 
    game->settings.initial_length  = 3;
    game->settings.is_two_players = 0;
    game->settings.has_walls      = 0;
    game->food_list.food_count    = 0;
}

/* initialisation de la fenetre */
void init_window(WindowSize *window_size) {
    MLV_get_desktop_size(&window_size->width, &window_size->height);
    MLV_create_full_screen_window("Snake", "Snake", window_size->width, window_size->height);
}

void init_score_board_and_save_slots(ScoreBoard *score_board, SaveSlotList *save_slots) {
    init_score_board(score_board);
    load_scores(score_board);
    load_save_slot_list(save_slots);
}


int main() {
    /* Déclaration des variables */
    WindowSize  window_size;
    ButtonsList buttons_list;
    MenuState   menu_state;
    Game        game;
    int         quitter;
    long        elapsed_ns;
    int         nb_frames;
    ScoreBoard score_board;
    SaveSlotList save_slots;

    struct timespec debut, fin;

    srand(time(NULL));

    /* Initialisation des variables */
    menu_state = MAIN_MENU;
    quitter    = 0;
    nb_frames  = 0;
    buttons_list.nb_buttons = 0;
    buttons_list.selected_button = 0;

    init_game_defaults(&game);
    init_score_board_and_save_slots(&score_board, &save_slots);
    init_window(&window_size);

    

    while (!quitter) {
        clock_gettime(CLOCK_REALTIME, &debut );
        /* 1. Mis à jour de l'image */
        MLV_update_window();
        /* 2. Effacement de l'écran */
        MLV_clear_window(MLV_COLOR_BLACK);

        /* 3. Défini dans quel endroit on est dans le jeu et affiche le menu correspondant */
        switch (menu_state) {
            case MAIN_MENU:
                display_menu(window_size, &buttons_list);

                /* Gestion des actions des boutons */
                process_main_menu_actions(&buttons_list, &menu_state, &quitter);
                break;


            case NEW_GAME_MENU:
                display_new_game_menu(window_size, &buttons_list, &game.settings);

                /* Gestion des actions des boutons */
                process_new_game_menu_actions(&buttons_list, &game, &menu_state, &window_size);
                break;

                
            case LOAD_GAME_MENU:
                load_save_slot_list(&save_slots);
                display_load_menu(window_size, &buttons_list, &save_slots);
                process_load_menu_actions(&buttons_list, &game, &menu_state, &save_slots);
                break;


            case SCORES_MENU:
                display_scores_menu(window_size, &buttons_list, &score_board);
                process_scores_menu_actions(&buttons_list, &menu_state, &score_board);
                break;


            case IN_GAME_CLASSIC:
                process_classic_mode(
                    &game, 
                    &window_size, 
                    &buttons_list, 
                    &nb_frames, 
                    &menu_state, 
                    &score_board, 
                    &save_slots
                );
                break;
            default:
                quitter = 1;
                break;
        }

        clock_gettime(CLOCK_REALTIME, &fin);

        elapsed_ns = (fin.tv_nsec - debut.tv_nsec);
        if (elapsed_ns < FRAME_TIME_NS && fin.tv_nsec > debut.tv_nsec) {
            MLV_wait_milliseconds((FRAME_TIME_NS - elapsed_ns) / NANOS_TO_MILLIS);
        }
    }

    free_game(&game);
    MLV_free_window ();
    exit(EXIT_SUCCESS);
}
