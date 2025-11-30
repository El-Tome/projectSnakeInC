#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include "grille.h"
#include "main.h"
#include "snake.h"

#define MAX_SPEED 60
#define MIN_SPEED 1



typedef struct {
    int width;
    int height;
    int speed;
    int is_two_players;
    int has_walls;
    
} GameSettings;

typedef struct {
    GameSettings settings;
    Grid         grid;
    GameState    state;
    Snake        snake;

    /*Food food;
    GameOver game_over;
    GameState previous_state;
    GameState next_state;
    */
} Game;

void init_game(Game *game);


#endif
