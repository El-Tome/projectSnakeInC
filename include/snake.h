#ifndef SNAKE_H
#define SNAKE_H

#include "grille.h"
#include "controls.h"

#define MAX_SNAKE_LENGTH 2000

typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_NONE
} Direction;

/* Type de coin pour les segments du corps */
typedef enum {
    CORNER_NONE,        /* Pas de coin, segment droit */
    CORNER_RIGHT,       /* Tourne à droite (sens horaire) */
    CORNER_LEFT         /* Tourne à gauche (sens anti-horaire) */
} CornerType;

typedef struct {
    Position position;
    Direction direction;
    CornerType corner;  /* Type de coin si c'est un segment de corps */
} Segment;

/* Serpent = tableau de positions (buffer circulaire) */
typedef struct {
    Segment segments[MAX_SNAKE_LENGTH];  /* Tableau statique contenant toutes les positions */
    int head_index;                       /* Index de la tête dans le tableau */
    int tail_index;                       /* Index de la queue dans le tableau */
    int length;                           /* Longueur actuelle */
    int is_alive;                         /* 1 = vivant, 0 = mort */
    int has_next_direction;
    Direction buffer;
} Snake;

/* 
 * Initialise le serpent au centre (ou position donnée) avec une taille initiale.
 * Le serpent grandit vers le bas par défaut lors de l'init si besoin, 
 * ou est initialisé "replié" sur lui même si length=1.
 */
void init_snake(Grid *grid, Snake *snake, int initial_length);

/* Initialise le serpent du joueur 2 à droite de la grille */
void init_snake_player2(Grid *grid, Snake *snake, int initial_length);

/* 
 * Déplace le serpent d'une case dans sa direction actuelle.
 * Met à jour head_index et tail_index (circular buffer).
 * Ne gère PAS les collisions (vérifiées séparément).
 */
void move_snake(Grid *grid, Snake *snake);

/* On récupére la valeur de la cellule suivante */
CellType get_next_cell_value(Grid *grid, Snake *snake);

/* 
 * Fait grandir le serpent au prochain mouvement.
 * En pratique: on n'incrémentera pas tail_index au prochain move,
 * ou on peut l'appeler juste après un move pour "reculer" la queue.
 *
 * NOTE: Dans une implémentation buffer circulaire strict, "grandir" se fait souvent 
 * au moment du mouvement. Si on appelle grow_snake(), on peut simplement 
 * décrémenter tail_index (modulo) pour "récupérer" l'ancienne queue, 
 * ou utiliser un flag "should_grow".
 * 
 * Simplification ici: on va dire que grow_snake ajoute un segment immédiatement
 * à la position de la queue actuelle (duplication temporaire de la queue).
 */
void grow_snake(Grid *grid, Snake *snake);

/* Change la direction si le changement est valide (pas de demi-tour immédiat) */
void set_snake_direction(Snake *snake, Direction new_dir);

/* Vérifie si on peut passer de current à new_dir (pas d'opposés) */
int is_valid_direction_change(Direction current, Direction new_dir);

/* Helpers pour récupérer les positions sans se soucier du buffer circulaire */
Position get_head_position(Snake snake);
Position get_segment_position(Snake snake, int index_offset); /* index_offset: 0=tête, 1=cou, ..., length-1=queue */

/* Vérifie si deux serpents sont en collision (l'un touche l'autre) */
int check_snake_collision(Snake *attacker, Snake *target);

#endif
