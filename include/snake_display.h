#ifndef SNAKE_DISPLAY_H
#define SNAKE_DISPLAY_H

#include <MLV/MLV_all.h>
#include "snake.h"
#include "main.h"
#include "grid_display.h"

/* Taille de base des sprites */
#define SPRITE_SIZE 32

/* Nombre max de frames d'animation */
#define MAX_FRAMES 4

/* Type de partie du serpent */
typedef enum {
    SNAKE_PART_HEAD,
    SNAKE_PART_BODY_HEAD,
    SNAKE_PART_BODY,
    SNAKE_PART_TAIL
} SnakePartType;

/* Sprites animés pour une direction (plusieurs frames) */
typedef struct {
    MLV_Image *frames[MAX_FRAMES];
    int frame_count;
} AnimatedSprite;

/* Sprites pour toutes les directions d'une partie */
typedef struct {
    AnimatedSprite up;
    AnimatedSprite right;
    AnimatedSprite down;
    AnimatedSprite left;
} DirectionalSprites;

/* Sprites pour les coins (droite et gauche) */
typedef struct {
    DirectionalSprites turn_right;  /* Quand on tourne à droite */
    DirectionalSprites turn_left;   /* Quand on tourne à gauche */
} CornerSprites;

/* Tous les sprites du serpent */
typedef struct {
    /* Lignes droites */
    DirectionalSprites head;
    DirectionalSprites body_head;    /* Transition tête-corps */
    DirectionalSprites body;
    DirectionalSprites tail;
    
    /* Coins */
    CornerSprites head_corner;
    CornerSprites body_corner;
    CornerSprites tail_corner;
    
    int is_loaded;
    int current_size;
} SnakeSprites;

/* Animation simple */
typedef struct {
    int current_frame;
    int frame_delay;
    int tick_counter;
    int frame_to_play;
} SnakeAnimation;

/* Initialise les sprites */
int init_snake_sprites(SnakeSprites *sprites);

/* Libère les sprites */
void free_snake_sprites(SnakeSprites *sprites);

/* Redimensionne les sprites */
void resize_snake_sprites(SnakeSprites *sprites, int cell_size);

/* Initialise l'animation */
void init_snake_animation(SnakeAnimation *anim, int frame_delay, int frame_count);

/* Met à jour l'animation */
void update_snake_animation(SnakeAnimation *anim);

/* Réinitialise l'animation */
void reset_snake_animation(SnakeAnimation *anim);

/* Dessine le serpent complet */
void draw_snake(SnakeSprites sprites, Snake snake, SnakeAnimation anim, 
                int cell_size, int grid_ox, int grid_oy);

/* Fallback en rectangles */
void draw_snake_fallback(Snake *snake, CellSize cell_size, GridOffset offset);

/* Fallback en rectangles avec couleur personnalisée */
void draw_snake_fallback_color(Snake *snake, CellSize cell_size, GridOffset offset, MLV_Color color);

#endif
