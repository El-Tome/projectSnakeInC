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

/* Résultat du mode VS */
typedef enum {
    VS_RESULT_NONE,      /* Partie en cours */
    VS_RESULT_P1_WINS,   /* Joueur 1 gagne */
    VS_RESULT_P2_WINS,   /* Joueur 2 gagne */
    VS_RESULT_DRAW       /* Égalité (collision simultanée) */
} VSResult;

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

    /* Mode 2 joueurs */
    Snake           snake2;
    SnakeSprites    snake2_sprites;
    SnakeAnimation  snake2_animation;
    VSResult        vs_result;

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

void init_vs_game(Game *game, WindowSize *window_size);
void free_vs_game(Game *game);
void draw_vs_game(Game *game, WindowSize *window_size);

#endif
