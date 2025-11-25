#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include "grille.h"
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
    Grid grid;
    /*Snake snake;
    Food food;
    */
    /*
    GameOver game_over;
    GameState state;
    GameState previous_state;
    GameState next_state;
    */
} Game;


#endif
