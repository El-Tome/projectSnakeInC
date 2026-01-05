#include "game.h"
#include "main.h"
#include "snake_display.h"
#include <math.h>

void init_game(Game *game, WindowSize *window_size) {
    int frame_delay, move_duration, frames_to_play;

    game->state = FREEZE_GAME_MENU;
    game->grid  = init_grid(game->settings.width, game->settings.height, game->settings.has_walls);

    clear_grid(&game->grid);

    init_snake(&game->grid, &game->snake, 3);

    /* placement de la nourriture */
    spawn_food(&game->grid, &game->food_list, 1, 1);
    
    /* Initialisation des sprites du serpent sinon mode case verte */
    if (init_snake_sprites(&game->snake_sprites)) {
        move_duration = FPS - game->settings.speed;
        
        /* Décider du nombre de frames selon la durée disponible */
        if (move_duration < MAX_FRAMES) {
            /* Pas assez de temps pour toutes les frames */
            frames_to_play = move_duration;
            if (frames_to_play < 1) {
                frames_to_play = 1;
            }
            frame_delay = 1;
        } else {
            /* On joue toutes les frames */
            frames_to_play = MAX_FRAMES;
            frame_delay = ceil((double)move_duration / frames_to_play);
        }
        
        init_snake_animation(&game->snake_animation, frame_delay, frames_to_play);
    }
    
    draw_game(game, window_size);
}

void free_game(Game *game) {
    free_snake_sprites(&game->snake_sprites);
}

void draw_game(Game *game, WindowSize *window_size) {
    CellSize   cell_size;
    GridOffset grid_offset;
    
    /* Dessiner la grille de base */
    draw_grid(&game->grid, window_size);
    
    /* Calculer la taille des cellules et l'offset */
    cell_size   = get_cell_size(&game->grid, *window_size);
    grid_offset = get_grid_offset(&game->grid, *window_size, cell_size);
    
    /* Dessiner le serpent avec les sprites ou en fallback */
    if (game->snake_sprites.is_loaded) {
        /* Dessiner le serpent avec les sprites ou en fallback */
        draw_snake(
            game->snake_sprites,
            game->snake,
            game->snake_animation,
            cell_size.width,
            grid_offset.x,
            grid_offset.y
        );
    } else {
        /* Fallback: dessiner le serpent en rectangles verts */
        draw_snake_fallback(
            &game->snake,
            cell_size,
            grid_offset
        );
    }
}



void init_vs_game(Game *game, WindowSize *window_size) {
    int frame_delay, move_duration, frames_to_play;

    game->state     = FREEZE_GAME_MENU;
    game->vs_result = VS_RESULT_NONE;
    game->grid      = init_grid(game->settings.width, game->settings.height, game->settings.has_walls);

    clear_grid(&game->grid);

    /* Initialisation du serpent 1 (à gauche) */
    init_snake(&game->grid, &game->snake, 3);

    /* Initialisation du serpent 2 (à droite) */
    init_snake_player2(&game->grid, &game->snake2, 3);

    /* Placement de la nourriture */
    spawn_food(&game->grid, &game->food_list, 1, 1);

    /* Initialisation des sprites du serpent 1 */
    if (init_snake_sprites(&game->snake_sprites)) {
        move_duration = FPS - game->settings.speed;

        if (move_duration < MAX_FRAMES) {
            frames_to_play = move_duration;
            if (frames_to_play < 1) {
                frames_to_play = 1;
            }
            frame_delay = 1;
        } else {
            frames_to_play = MAX_FRAMES;
            frame_delay = ceil((double)move_duration / frames_to_play);
        }

        init_snake_animation(&game->snake_animation, frame_delay, frames_to_play);
    }

    /* Initialisation des sprites du serpent 2 (mêmes sprites) */
    if (init_snake_sprites(&game->snake2_sprites)) {
        move_duration = FPS - game->settings.speed;

        if (move_duration < MAX_FRAMES) {
            frames_to_play = move_duration;
            if (frames_to_play < 1) {
                frames_to_play = 1;
            }
            frame_delay = 1;
        } else {
            frames_to_play = MAX_FRAMES;
            frame_delay = ceil((double)move_duration / frames_to_play);
        }

        init_snake_animation(&game->snake2_animation, frame_delay, frames_to_play);
    }

    draw_vs_game(game, window_size);
}

void free_vs_game(Game *game) {
    free_snake_sprites(&game->snake_sprites);
    free_snake_sprites(&game->snake2_sprites);
}

void draw_vs_game(Game *game, WindowSize *window_size) {
    CellSize   cell_size;
    GridOffset grid_offset;

    /* Dessiner la grille de base */
    draw_grid(&game->grid, window_size);

    /* Calculer la taille des cellules et l'offset */
    cell_size = get_cell_size(&game->grid, *window_size);
    grid_offset = get_grid_offset(&game->grid, *window_size, cell_size);

    /* Dessiner le serpent 1 */
    if (game->snake_sprites.is_loaded) {
        draw_snake(
            game->snake_sprites,
            game->snake,
            game->snake_animation,
            cell_size.width,
            grid_offset.x,
            grid_offset.y
        );
    } else {
        draw_snake_fallback(
            &game->snake,
            cell_size,
            grid_offset
        );
    }

    /* Dessiner le serpent 2 (couleur différente en fallback) */
    if (game->snake2_sprites.is_loaded) {
        draw_snake(
            game->snake2_sprites,
            game->snake2,
            game->snake2_animation,
            cell_size.width,
            grid_offset.x,
            grid_offset.y
        );
    } else {
        draw_snake_fallback_color(
            &game->snake2,
            cell_size,
            grid_offset,
            MLV_COLOR_BLUE
        );
    }
}

