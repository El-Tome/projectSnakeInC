#include "game.h"
#include "main.h"

void init_game(Game *game, WindowSize *window_size) {
    int frame_delay;

    game->state = FREEZE_GAME_MENU;
    game->grid  = init_grid(game->settings.width, game->settings.height, game->settings.has_walls);

    clear_grid(&game->grid);

    init_snake(&game->grid, &game->snake, 3);

    /* placement de la nourriture */
    spawn_food(&game->grid, &game->food_list, 1, 1);
    
    /* Initialisation des sprites du serpent sinon mode case verte */
    if (init_snake_sprites(&game->snake_sprites)) {
        /* Initialisation de l'animation */
        frame_delay = (FPS+1 - game->settings.speed)/MAX_FRAMES;
        /* Si le délai est inférieur à 1, on le met à 1 */
        if (frame_delay < 1) {
            frame_delay = 1;
        }
        init_snake_animation(&game->snake_animation, frame_delay);
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
