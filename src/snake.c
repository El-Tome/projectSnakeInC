#include "snake.h"
#include <stdlib.h> /* Pour abs si besoin */

/* Initialisation du serpent */
void init_snake(Grid *grid, Snake *snake, int initial_length) {
    Position start_pos;
    int i;

    start_pos.x = grid->width / 3;
    start_pos.y = grid->height / 2;

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
        snake->segments[i].position.x = start_pos.x - (initial_length - 1 - i);
        snake->segments[i].position.y = start_pos.y;
        snake->segments[i].direction = DIR_RIGHT; /* Direction par défaut */
        set_cell(grid, &snake->segments[i].position, CELL_SNAKE);
    }
}

/* Retourne la position de la tête */
Position get_head_position(Snake snake) {
    return snake.segments[snake.head_index].position;
}

/* Vérifie si la direction demandée n'est pas l'opposée de l'actuelle */
int is_valid_direction_change(Direction current, Direction new_dir) {
    /* Si la direction demandée est l'opposée de l'actuelle, on retourne 0 */
    if (current == DIR_UP && new_dir == DIR_DOWN) {
        return 0;
    } else if (current == DIR_DOWN && new_dir == DIR_UP) {
        return 0;
    } else if (current == DIR_LEFT && new_dir == DIR_RIGHT) {
        return 0;
    } else if (current == DIR_RIGHT && new_dir == DIR_LEFT) {
        return 0;
    } else {
        /* Si la direction demandée n'est pas l'opposée de l'actuelle, c'est valide */
        return 1;
    }
}

void set_snake_direction(Snake *snake, Direction new_dir) {
    if (is_valid_direction_change(snake->segments[snake->head_index].direction, new_dir)) {
        snake->segments[snake->head_index].direction = new_dir;
    }
}

/* Déplace le serpent */
void move_snake(Grid *grid, Snake *snake) {
    /* 1. Calculer la nouvelle position de la tête */
    Segment current_head = snake->segments[snake->head_index];
    Segment new_head     = current_head;

    switch (snake->segments[snake->head_index].direction) {
        case DIR_UP:
            new_head.position.y -= 1;
            if (new_head.position.y < 0) {
                new_head.position.y = grid->height - 1;
            }
            break;
        case DIR_DOWN:  
            new_head.position.y += 1; 
            if (new_head.position.y >= grid->height) {
                new_head.position.y = 0;
            }
            break;
        case DIR_LEFT:  
            new_head.position.x -= 1; 
            if (new_head.position.x < 0) {
                new_head.position.x = grid->width - 1;
            }
            break;
        case DIR_RIGHT: 
            new_head.position.x += 1; 
            if (new_head.position.x >= grid->width) {
                new_head.position.x = 0;
            }
            break;
        default:
            break;
    }

    /* 2. Avancer l'index de la tête (Circulaire) */
    snake->head_index = (snake->head_index + 1) % MAX_SNAKE_LENGTH;

    /* 3. Écrire la nouvelle position */
    snake->segments[snake->head_index] = new_head;
    set_cell(grid, &new_head.position, CELL_SNAKE);

    /* 4. Avancer l'index de la queue (on supprime l'ancienne queue) */
    set_cell(grid, &snake->segments[snake->tail_index].position, CELL_EMPTY);
    snake->tail_index = (snake->tail_index + 1) % MAX_SNAKE_LENGTH;
}


/* Vérifie collision avec soi-même */
int check_self_collision(Snake snake) {
    Position head = get_head_position(snake);

    /* On parcourt tout le corps (sauf la tête elle-même, donc on commence offset 1) */
    /*int i;
    for (i = 1; i < snake.length; i++) {
        Position body_part = get_segment_position(snake, i);
        if (head.x == body_part.x && head.y == body_part.y) {
            return 1;  *//* Collision */
        /*}
    }*/
    return 0;
}
