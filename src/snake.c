#include "snake.h"
#include <stdlib.h> /* Pour abs si besoin */

/* Initialisation du serpent */
void init_snake(Grid *grid, Snake *snake, int initial_length) {
    Position start_pos;
    int i;

    start_pos.x = grid->width / 3;
    start_pos.y = grid->height / 2;

    snake->direction = DIR_RIGHT; /* Direction par défaut */
    snake->is_alive = 1;

    
    if (initial_length > MAX_SNAKE_LENGTH) {
        initial_length = MAX_SNAKE_LENGTH;
    } else if (initial_length < 1) {
        initial_length = 1;
    } else if (start_pos.x - (initial_length - 1) < 0) {
        /* Si le serpent est trop long, on va reduire la longueur du serpent pour qu'il soit inférieur à la grille/3 */
        initial_length = grid->width / 3;
    }

    snake->length = initial_length;


    /*
     * Placement de l'index de la tête et de la queue
     */
    snake->head_index = initial_length - 1;
    snake->tail_index = 0;

    /* On place les segments initiaux */
    for (i = 0; i < initial_length; i++) {
        snake->segments[i].x = start_pos.x - (initial_length - 1 - i);
        snake->segments[i].y = start_pos.y;
        set_cell(grid, &snake->segments[i], CELL_SNAKE);
    }
}

/* Retourne la position de la tête */
Position get_head_position(Snake snake) {
    return snake.segments[snake.head_index];
}

/* 
 * Retourne la position d'un segment par rapport à la tête.
 * offset 0 = tête
 * offset 1 = le segment juste après la tête
 * offset length-1 = queue
 */
Position get_segment_position(Snake snake, int offset) {
    /* 
     * On recule depuis la tête.
     * Index = (head_index - offset)
     * Comme c'est circulaire, on gère les nombres négatifs.
     */
    int index = (snake.head_index - offset);
    if (index < 0) {
        index += MAX_SNAKE_LENGTH;
    }
    return snake.segments[index];
}

/* Vérifie si la direction demandée n'est pas l'opposée de l'actuelle */
int is_valid_direction_change(Direction current, Direction new_dir) {
    if (current == DIR_UP && new_dir == DIR_DOWN) return 0;
    if (current == DIR_DOWN && new_dir == DIR_UP) return 0;
    if (current == DIR_LEFT && new_dir == DIR_RIGHT) return 0;
    if (current == DIR_RIGHT && new_dir == DIR_LEFT) return 0;
    return 1;
}

void set_snake_direction(Snake *snake, Direction new_dir) {
    if (is_valid_direction_change(snake->direction, new_dir)) {
        snake->direction = new_dir;
    }
}

/* Déplace le serpent */
void move_snake(Snake *snake) {
    /* 1. Calculer la nouvelle position de la tête */
    Position current_head = get_head_position(*snake);
    Position new_head = current_head;

    switch (snake->direction) {
        case DIR_UP:    new_head.y -= 1; break;
        case DIR_DOWN:  new_head.y += 1; break;
        case DIR_LEFT:  new_head.x -= 1; break;
        case DIR_RIGHT: new_head.x += 1; break;
    }

    /* 2. Avancer l'index de la tête (Circulaire) */
    snake->head_index = (snake->head_index + 1) % MAX_SNAKE_LENGTH;

    /* 3. Écrire la nouvelle position */
    snake->segments[snake->head_index] = new_head;

    /* 4. Avancer l'index de la queue (on supprime l'ancienne queue) */
    /* Note: Si on voulait grandir, on n'avancerait pas la queue ici */
    snake->tail_index = (snake->tail_index + 1) % MAX_SNAKE_LENGTH;
}

/* 
 * Pour faire grandir le serpent :
 * L'approche la plus simple avec move_snake déjà implémenté comme ci-dessus
 * est de reculer l'index de la queue de 1 (annuler le "suppression" de queue du dernier move).
 * Cela duplique temporairement la queue, qui sera "corrigée" au prochain move.
 */
void grow_snake(Snake *snake) {
    if (snake->length < MAX_SNAKE_LENGTH) {
        /* On recule l'index de la queue pour inclure l'ancien segment "perdu" */
        snake->tail_index = (snake->tail_index - 1);
        if (snake->tail_index < 0) {
            snake->tail_index += MAX_SNAKE_LENGTH;
        }
        snake->length++;
    }
}

/* Vérifie collision avec soi-même */
int check_self_collision(Snake snake) {
    Position head = get_head_position(snake);

    /* On parcourt tout le corps (sauf la tête elle-même, donc on commence offset 1) */
    int i;
    for (i = 1; i < snake.length; i++) {
        Position body_part = get_segment_position(snake, i);
        if (head.x == body_part.x && head.y == body_part.y) {
            return 1; /* Collision */
        }
    }
    return 0;
}
