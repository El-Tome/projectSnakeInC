#include "main.h"
#include "controls.h"
#include "menu_handler.h"
#include "menu_display.h"
#include <time.h>
#include "game.h"
#include "snake.h"

int main() {
    /* Déclaration des variables */
    WindowSize  window_size;
    ButtonsList buttons_list;
    MenuState   menu_state;
    Game        game;
    int         quitter;
    long        elapsed_ns;
    int         nb_frames;
    ToucheClavier touche;

    struct timespec debut, fin;

    /* Initialisation des variables */
    menu_state = MAIN_MENU;
    quitter    = 0;
    nb_frames  = 0;
    buttons_list.nb_buttons = 0;
    buttons_list.selected_button = 0;

    game.settings.width  = 20;
    game.settings.height = 15;
    game.settings.speed  = 15; 
    game.settings.initial_length  = 3;
    game.settings.is_two_players = 0;
    game.settings.has_walls      = 0;
    game.food_list.food_count    = 0;

    srand(time(NULL));

    /* Création de la fenetre MLV */
    MLV_get_desktop_size(&window_size.width, &window_size.height);
    MLV_create_full_screen_window("Snake", "Snake", window_size.width, window_size.height);    

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
                /*load_game();*/
                break;
            case SCORES_MENU:
                /*display_scores_menu();*/
                break;
            case IN_GAME:
                switch (game.state) {
                    case FREEZE_GAME_MENU:
                        draw_grid(&game.grid, window_size);

                        touche = convert_key_to_enum(get_key_pressed());

                        switch (touche) {
                            case UP:
                            case DOWN:
                            case LEFT:
                            case RIGHT:
                                game.state = PLAYING;
                                break;
                            default:
                                break;
                        }
                        break;

                    case PLAYING:
                        nb_frames++;

                        touche = convert_key_to_enum(get_key_pressed());

                        switch (touche) {
                            case UP:
                                set_snake_direction(&game.snake, DIR_UP);
                                break;
                            case DOWN:
                                set_snake_direction(&game.snake, DIR_DOWN);
                                break;
                            case LEFT:
                                set_snake_direction(&game.snake, DIR_LEFT);
                                break;
                            case RIGHT:
                                set_snake_direction(&game.snake, DIR_RIGHT);
                                break;
                            default:
                                set_snake_direction(&game.snake, DIR_NONE);
                                break;
                        }

                        if (nb_frames % (31 - game.settings.speed) == 0) {
                            /* Check le prochain déplacement */

                            /* switch */
                            /* si en mode mur il y a collision alors game over */
                            /* il y a collision avec la nourriture alors grandir le serpent */
                            /* il y a collision avec soi-même alors game over */
                            /* Sinon déplacer le serpent */
                            switch (get_next_cell_value(&game.grid, &game.snake)) {
                                case CELL_EMPTY:
                                    move_snake(&game.grid, &game.snake);
                                    game.snake.has_next_direction = 0;
                                    break;
                                case CELL_FOOD:
                                    grow_snake(&game.grid, &game.snake);
                                    game.snake.has_next_direction = 0;
                                    spawn_food(&game.grid, &game.food_list, 1, 1);
                                    break;
                                case CELL_WALL:
                                    if (game.settings.has_walls) {
                                        game.state = GAME_OVER_MENU;
                                        buttons_list.selected_button = 0;
                                    } else {
                                        move_snake(&game.grid, &game.snake);
                                        game.snake.has_next_direction = 0;
                                    }
                                    break;
                                case CELL_SNAKE:
                                    game.state = GAME_OVER_MENU;
                                    buttons_list.selected_button = 0;
                                    break;
                                default:
                                    break;
                            }


                            nb_frames = 0;
                        }
                        draw_grid(&game.grid, window_size);

                        break;
                    case GAME_OVER_MENU:
                        display_game_over_menu(window_size, &buttons_list, &game);
                        process_game_over_menu_actions(&buttons_list, &game, &menu_state, &window_size);
                        break;
                    case PAUSE_MENU:
                        break;
                    case SAVE_MENU:
                        break;
                    default:
                        break;
                
                }
               


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
        /*
            
        //     // Affichage
        //     // Récupération événement clavier (1 seul par image)
        //     // Résolution événements
        //     // Déplacement objets
        //     // Résolution collisions
            

        */
    }

    MLV_free_window ();


/*
    // detecte les evenements clavier / souris pour savoir sur quelle bouton on a cliqué

    // si on a cliqué sur le bouton "nouveau jeu", on crée une nouvelle partie

    // si on a cliqué sur le bouton "charger une partie", on charge la partie enregistrée

    // si on a cliqué sur le bouton "quitter", on quitte le jeu

    // si on a cliqué sur le bouton "scores", on affiche les scores
*/
    exit(EXIT_SUCCESS);
}
