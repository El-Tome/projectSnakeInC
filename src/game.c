#include "game.h"

void init_game(Game *game, WindowSize *window_size) {
    game->state = FREEZE_GAME_MENU;
    game->grid  = init_grid(game->settings.width, game->settings.height, game->settings.has_walls);

    clear_grid(&game->grid);

    init_snake(&game->grid, &game->snake, 3);

    /* placement de la nourriture */

    draw_grid(&game->grid, *window_size);
}

