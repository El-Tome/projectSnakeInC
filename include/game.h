#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include "grille.h"
#include "grid_display.h"
#include "main.h"
#include "snake.h"
#include "snake_display.h"
#include "food.h"

#define MAX_SPEED 60
#define MIN_SPEED 1

/* Chemin vers la sprite sheet du serpent */
#define SNAKE_SPRITE_SHEET "ressources/snake/snake_sprite.png"

/* Délai entre frames d'animation (plus petit = plus rapide) */
#define SNAKE_ANIM_DELAY 5


typedef struct {
    int width;
    int height;
    int speed;
    int is_two_players;
    int has_walls;
    int initial_length;
} GameSettings;

typedef struct {
    GameSettings    settings;
    Grid            grid;
    GameState       state;
    Snake           snake;
    FoodList        food_list;
    int             score;
    
    /* Système de sprites pour le serpent */
    SnakeSprites    snake_sprites;
    SnakeAnimation  snake_animation;

    /*
    GameOver game_over;
    GameState previous_state;
    GameState next_state;
    */
} Game;

void init_game(Game *game, WindowSize *window_size);

/* Libère les ressources du jeu (sprites, etc.) */
void free_game(Game *game);

/* Dessine le jeu complet (grille + serpent avec sprites) */
void draw_game(Game *game, WindowSize *window_size);


#endif
