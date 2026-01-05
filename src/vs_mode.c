#include "vs_mode.h"
#include "controls.h"
#include "menu_display.h"

/* Prototypes des fonctions internes */
void freeze_vs_game_menu(Game *game, WindowSize *window_size, ButtonsList *buttons_list);
void playing_vs(Game *game, WindowSize *window_size, ButtonsList *buttons_list, int *nb_frames);


void freeze_vs_game_menu(Game *game, WindowSize *window_size, ButtonsList *buttons_list) {
    ToucheClavier p1_key, p2_key;

    draw_vs_game(game, window_size);
    MLV_draw_text(
        window_size->width / 2 - 150,
        window_size->height / 2,
        "Appuyez sur une touche pour commencer",
        MLV_COLOR_WHITE
    );
    MLV_draw_text(
        window_size->width / 2 - 100,
        window_size->height / 2 + 30,
        "J1: Fleches | J2: ZQSD",
        MLV_COLOR_YELLOW
    );

    get_event_two_players(&p1_key, &p2_key);

    if (p1_key == UP || p1_key == DOWN || p1_key == LEFT || p1_key == RIGHT ||
        p2_key == P2_UP || p2_key == P2_DOWN || p2_key == P2_LEFT || p2_key == P2_RIGHT) {
        game->state = PLAYING;
    } else if (p1_key == ESCAPE) {
        game->state = PAUSE_MENU;
        buttons_list->selected_button = 0;
    }
}

void process_playing_key_vs(Game *game, ButtonsList *buttons_list) {
    ToucheClavier p1_key, p2_key;

    get_event_two_players(&p1_key, &p2_key);

    /* Joueur 1 - Flèches directionnelles */
    switch (p1_key) {
        case UP:
            set_snake_direction(&game->snake, DIR_UP);
            break;
        case DOWN:
            set_snake_direction(&game->snake, DIR_DOWN);
            break;
        case LEFT:
            set_snake_direction(&game->snake, DIR_LEFT);
            break;
        case RIGHT:
            set_snake_direction(&game->snake, DIR_RIGHT);
            break;
        case ESCAPE:
            game->state = PAUSE_MENU;
            buttons_list->selected_button = 0;
            break;
        default:
            break;
    }

    /* Joueur 2 - ZQSD */
    switch (p2_key) {
        case P2_UP:
            set_snake_direction(&game->snake2, DIR_UP);
            break;
        case P2_DOWN:
            set_snake_direction(&game->snake2, DIR_DOWN);
            break;
        case P2_LEFT:
            set_snake_direction(&game->snake2, DIR_LEFT);
            break;
        case P2_RIGHT:
            set_snake_direction(&game->snake2, DIR_RIGHT);
            break;
        default:
            break;
    }
}

void playing_vs(Game *game, WindowSize *window_size, ButtonsList *buttons_list, int *nb_frames) {
    CellType next_cell_p1, next_cell_p2;
    int p1_dies = 0;
    int p2_dies = 0;

    process_playing_key_vs(game, buttons_list);

    /* Mise à jour à la vitesse définie */
    if (*nb_frames % (FPS + 1 - game->settings.speed) == 0) {
        /* Vérifier les prochaines cellules pour chaque serpent */
        next_cell_p1 = get_next_cell_value(&game->grid, &game->snake);
        next_cell_p2 = get_next_cell_value(&game->grid, &game->snake2);

        /* Déterminer si un joueur meurt */
        /* Joueur 1 : collision avec mur ou serpent */
        if (next_cell_p1 == CELL_WALL && game->settings.has_walls) {
            p1_dies = 1;
        } else if (next_cell_p1 == CELL_SNAKE) {
            p1_dies = 1;
        }

        /* Joueur 2 : collision avec mur ou serpent */
        if (next_cell_p2 == CELL_WALL && game->settings.has_walls) {
            p2_dies = 1;
        } else if (next_cell_p2 == CELL_SNAKE) {
            p2_dies = 1;
        }

        /* Traiter les mouvements si personne ne meurt */
        if (!p1_dies && !p2_dies) {
            /* Joueur 1 */
            if (next_cell_p1 == CELL_FOOD) {
                grow_snake(&game->grid, &game->snake);
                game->snake.has_next_direction = 0;
                spawn_food(&game->grid, &game->food_list, 1, 1);
                /* Fait apparaître un obstacle si l'option est activée */
                if (game->settings.spawn_obstacle_on_eat) {
                    spawn_obstacle(&game->grid, 1);
                }
                reset_snake_animation(&game->snake_animation);
            } else {
                move_snake(&game->grid, &game->snake);
                game->snake.has_next_direction = 0;
                reset_snake_animation(&game->snake_animation);
            }

            /* Joueur 2 */
            if (next_cell_p2 == CELL_FOOD) {
                grow_snake(&game->grid, &game->snake2);
                game->snake2.has_next_direction = 0;
                spawn_food(&game->grid, &game->food_list, 1, 1);
                /* Fait apparaître un obstacle si l'option est activée */
                if (game->settings.spawn_obstacle_on_eat) {
                    spawn_obstacle(&game->grid, 1);
                }
                reset_snake_animation(&game->snake2_animation);
            } else {
                move_snake(&game->grid, &game->snake2);
                game->snake2.has_next_direction = 0;
                reset_snake_animation(&game->snake2_animation);
            }

            /* Vérifier collision entre les deux serpents après mouvement */
            if (check_snake_collision(&game->snake, &game->snake2)) {
                p1_dies = 1;
            }
            if (check_snake_collision(&game->snake2, &game->snake)) {
                p2_dies = 1;
            }
        }

        /* Déterminer le résultat */
        if (p1_dies && p2_dies) {
            game->vs_result = VS_RESULT_DRAW;
            game->state = GAME_OVER_MENU;
            buttons_list->selected_button = 0;
        } else if (p1_dies) {
            game->vs_result = VS_RESULT_P2_WINS;
            game->state = GAME_OVER_MENU;
            buttons_list->selected_button = 0;
        } else if (p2_dies) {
            game->vs_result = VS_RESULT_P1_WINS;
            game->state = GAME_OVER_MENU;
            buttons_list->selected_button = 0;
        }

        *nb_frames = 0;
    }

    /* Mise à jour de l'animation à chaque frame */
    update_snake_animation(&game->snake_animation);
    update_snake_animation(&game->snake2_animation);
    draw_vs_game(game, window_size);
}

void process_vs_mode(
    Game         *game,
    WindowSize   *window_size,
    ButtonsList  *buttons_list,
    int          *nb_frames,
    MenuState    *menu_state
) {
    switch (game->state) {
        case FREEZE_GAME_MENU:
            freeze_vs_game_menu(game, window_size, buttons_list);
            reset_snake_animation(&game->snake_animation);
            reset_snake_animation(&game->snake2_animation);
            break;

        case PLAYING:
            (*nb_frames)++;
            playing_vs(game, window_size, buttons_list, nb_frames);
            break;

        case GAME_OVER_MENU:
            display_vs_game_over_menu(*window_size, buttons_list, game);
            process_vs_game_over_menu_actions(buttons_list, game, menu_state, window_size);
            break;

        case PAUSE_MENU:
            display_pause_menu(*window_size, buttons_list, 1);
            process_pause_menu_actions(buttons_list, game, menu_state, NULL, 1);
            break;

        default:
            break;
    }
}

