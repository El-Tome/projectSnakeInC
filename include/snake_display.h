#ifndef SNAKE_DISPLAY_H
#define SNAKE_DISPLAY_H

#include <MLV/MLV_all.h>
#include "snake.h"
#include "main.h"
#include "grid_display.h"

/* Dimensions des sprites */
#define SPRITE_SIZE 32          /* Taille d'une case sprite (32x32) */
#define SPRITE_SHEET_WIDTH 32   /* Largeur de la sprite sheet */
#define SPRITE_SHEET_HEIGHT 224 /* Hauteur totale de la sprite sheet */

/* Offsets Y dans la sprite sheet (en pixels) */
#define SPRITE_EMPTY_TOP_Y    0   /* Case vide du haut: 0-32 */
#define SPRITE_HEAD_Y         32  /* Tête: 32-64 */
#define SPRITE_BODY_START_Y   64  /* Début corps: 64-160 (3 corps identiques) */
#define SPRITE_BODY_1_Y       64  /* Corps 1: 64-96 */
#define SPRITE_BODY_2_Y       96  /* Corps 2: 96-128 */
#define SPRITE_BODY_3_Y       128 /* Corps 3: 128-160 */
#define SPRITE_TAIL_Y         160 /* Queue: 160-192 */
#define SPRITE_EMPTY_BOTTOM_Y 192 /* Case vide du bas: 192-224 */

/* Type de partie du serpent pour le rendu */
typedef enum {
    SNAKE_PART_HEAD,
    SNAKE_PART_BODY,
    SNAKE_PART_TAIL
} SnakePartType;

/* Structure pour stocker les 4 rotations d'une partie */
typedef struct {
    MLV_Image *up;     /* Rotation 0° (image de base) */
    MLV_Image *right;  /* Rotation 90° */
    MLV_Image *down;   /* Rotation 180° */
    MLV_Image *left;   /* Rotation 270° */
} PartSprites;

/* Structure pour gérer les sprites du serpent avec toutes les rotations */
typedef struct {
    PartSprites head;   /* Sprites de la tête (4 rotations) */
    PartSprites body;   /* Sprites du corps (4 rotations) */
    PartSprites tail;   /* Sprites de la queue (4 rotations) */
    int is_loaded;      /* 1 si les sprites sont chargés, 0 sinon */
    int current_size;   /* Taille actuelle des sprites (pour éviter redimensionnements inutiles) */
} SnakeSprites;

/* 
 * Structure pour l'animation du serpent
 * animation_progress: valeur de 0.0 à 1.0 représentant la progression du mouvement
 * 0.0 = début du mouvement (juste après un déplacement logique)
 * 1.0 = fin du mouvement (juste avant le prochain déplacement logique)
 */
typedef struct {
    float animation_progress; /* Progression de 0.0 à 1.0 */
    int frames_per_cell;      /* Nombre de frames pour traverser une cellule */
    int current_frame;        /* Frame actuelle dans le mouvement */
} SnakeAnimation;

/* Initialise les sprites du serpent */
int init_snake_sprites(SnakeSprites *sprites, const char *sprite_path);

/* Redimensionne les sprites à la taille des cases */
void resize_snake_sprites(SnakeSprites *sprites, int cell_size);

/* Libère la mémoire des sprites */
void free_snake_sprites(SnakeSprites *sprites);

/* Initialise l'animation du serpent */
void init_snake_animation(SnakeAnimation *anim, int fps, int game_speed);

/* Met à jour l'offset d'animation */
void update_snake_animation(SnakeAnimation *anim);

/* Réinitialise l'animation après un mouvement logique */
void reset_snake_animation(SnakeAnimation *anim);

/* 
 * Dessine un segment du serpent à une position donnée
 * part_type: HEAD, BODY ou TAIL
 * direction: direction du segment pour la rotation
 * animation_progress: progression de l'animation (0.0 à 1.0)
 * grid_offset: décalage de la grille pour le centrage
 */
void draw_snake_part(
    SnakeSprites *sprites,
    SnakePartType part_type,
    Position *grid_pos,
    Direction direction,
    float animation_progress,
    int cell_size,
    int grid_offset_x,
    int grid_offset_y
);

/* 
 * Dessine le serpent complet avec animation
 * Le serpent visuellement est décalé de moitié par rapport à sa position logique
 */
void draw_snake_animated(
    SnakeSprites *sprites,
    Snake *snake,
    SnakeAnimation *anim,
    int cell_size,
    int grid_offset_x,
    int grid_offset_y
);

/* 
 * Calcule le sprite Y offset en fonction de la partie et de l'animation
 * Pour le corps, utilise animation_offset pour "glisser" sur les 3 corps identiques
 */
int get_sprite_y_offset(SnakePartType part_type, int animation_offset);

/* 
 * Fallback: dessine le serpent en rectangles verts si les sprites ne sont pas chargés
 */
void draw_snake_fallback(
    Snake *snake,
    CellSize cell_size,
    GridOffset grid_offset
);

#endif

