#include "classic_mode.h"
#include "controls.h"
#include "menu_display.h"
#include <math.h>

void freeze_game_menu(Game *game, WindowSize *window_size, ButtonsList *buttons_list);
void playing(Game *game, WindowSize *window_size, ButtonsList *buttons_list, int *nb_frames, ScoreBoard *score_board);

void process_classic_mode(
    Game         *game, 
    WindowSize   *window_size, 
    ButtonsList  *buttons_list, 
    int          *nb_frames,
    MenuState    *menu_state, 
    ScoreBoard   *score_board, 
    SaveSlotList *save_slots
) {
    switch (game->state) {
        case FREEZE_GAME_MENU:
            freeze_game_menu(game, window_size, buttons_list);
            reset_snake_animation(&game->snake_animation);
            break;

        case PLAYING:
            (*nb_frames)++;
            playing(game, window_size, buttons_list, nb_frames, score_board);
            break;

        case GAME_OVER_MENU:
            display_game_over_menu(*window_size, buttons_list, game);
            process_game_over_menu_actions(buttons_list, game, menu_state, window_size);
            break;
        case PAUSE_MENU:
            display_pause_menu(*window_size, buttons_list, 0);
            process_pause_menu_actions(buttons_list, game, menu_state, save_slots, 0);
            break;
        case SAVE_MENU:
            display_save_menu(*window_size, buttons_list, save_slots);
            process_save_menu_actions(buttons_list, game, menu_state, save_slots);
            break;
        default:
            break;
    
    }
}

void freeze_game_menu(Game *game, WindowSize *window_size, ButtonsList *buttons_list) {
    ToucheClavier touche;

    draw_game(game, window_size);
    MLV_draw_text(window_size->width / 2, window_size->height / 2, "Appuyez sur une touche pour commencer", MLV_COLOR_WHITE);

    touche = convert_key_to_enum(get_key_pressed());

    switch (touche) {
        case UP:
        case DOWN:
        case LEFT:
        case RIGHT:
            game->state = PLAYING;
            break;
        case ESCAPE:
            game->state = PAUSE_MENU;
            buttons_list->selected_button = 0;
            break;
        default:
            break;
    }
}

void process_playing_key(Game *game, ButtonsList *buttons_list) {
    ToucheClavier touche;

    touche = convert_key_to_enum(get_key_pressed());

    switch (touche) {
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
            set_snake_direction(&game->snake, DIR_NONE);
            break;
    }
}

/* Calcule le score avec les multiplicateurs */
int calculate_score(Game *game) {
    double score = (double)(game->snake.length - game->settings.initial_length) * game->settings.speed;
    
    /* Multiplicateur x1.5 si les murs sont activés */
    if (game->settings.has_walls) {
        score = score * 1.5;
    }
    /* Multiplicateur x2 si les pierres sont activées */
    if (game->settings.spawn_obstacle_on_eat) {
        score = score * 2;
    }
    
    return trunc(score);
}

void playing(Game *game, WindowSize *window_size, ButtonsList *buttons_list, int *nb_frames, ScoreBoard *score_board) {
    ScoreEntry    score_entry;

    process_playing_key(game, buttons_list);

    /* FPS+1 car on veux se déplacer au maximun 1 fois par frame */
    if (*nb_frames % (FPS+1 - game->settings.speed) == 0) {
        switch (get_next_cell_value(&game->grid, &game->snake)) {
            case CELL_EMPTY:
                move_snake(&game->grid, &game->snake);
                reset_snake_animation(&game->snake_animation);
                game->snake.has_next_direction = 0;
                break;

            case CELL_FOOD:
                grow_snake(&game->grid, &game->snake);
                game->snake.has_next_direction = 0;
                spawn_food(&game->grid, &game->food_list, 1, 1);
                /* Fait apparaître un obstacle si l'option est activée */
                if (game->settings.spawn_obstacle_on_eat) {
                    spawn_obstacle(&game->grid, 1);
                }
                reset_snake_animation(&game->snake_animation);
                break;

            case CELL_WALL:
                /* Murs de bordure : le serpent meurt si has_walls est activé */
                if (game->settings.has_walls) {
                    game->state = GAME_OVER_MENU;
                    buttons_list->selected_button = 0;

                    /* Calcul et sauvegarde du score avec multiplicateurs */
                    game->score = calculate_score(game);
                    score_entry = create_score_entry(game->score, &game->settings);
                    add_score(score_board, &score_entry);
                    save_scores(score_board);
                } else {
                    /* Murs désactivés : le serpent traverse */
                    move_snake(&game->grid, &game->snake);
                    reset_snake_animation(&game->snake_animation);
                    game->snake.has_next_direction = 0;
                }
                break;

            case CELL_OBSTACLE:
                /* Pierres : le serpent meurt toujours */
                game->state = GAME_OVER_MENU;
                buttons_list->selected_button = 0;

                /* Calcul et sauvegarde du score avec multiplicateurs */
                game->score = calculate_score(game);
                score_entry = create_score_entry(game->score, &game->settings);
                add_score(score_board, &score_entry);
                save_scores(score_board);
                break;

            case CELL_SNAKE:
                game->state = GAME_OVER_MENU;
                buttons_list->selected_button = 0;

                /* Calcul et sauvegarde du score avec multiplicateurs */
                game->score = calculate_score(game);
                score_entry = create_score_entry(game->score, &game->settings);
                add_score(score_board, &score_entry);
                save_scores(score_board);
                break;
            default:
                break;
        }


        *nb_frames = 0;
        reset_snake_animation(&game->snake_animation);
    }

    /* Mise à jour de l'animation à chaque frame */
    update_snake_animation(&game->snake_animation);
    draw_game(game, window_size);
}
