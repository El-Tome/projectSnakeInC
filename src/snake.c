#include "snake.h"
#include <stdlib.h> /* Pour abs si besoin */

/* 
 * Calcule le type de coin entre deux directions
 * prev_dir: direction du segment précédent (vers la queue)
 * next_dir: direction du segment suivant (vers la tête)
 */
static CornerType get_corner_type(Direction prev_dir, Direction next_dir) {
    /* Pas de coin si même direction */
    if (prev_dir == next_dir || prev_dir == DIR_NONE || next_dir == DIR_NONE) {
        return CORNER_NONE;
    }
    
    /* Coin à droite (sens horaire) */
    if ((prev_dir == DIR_UP && next_dir == DIR_RIGHT) ||
        (prev_dir == DIR_RIGHT && next_dir == DIR_DOWN) ||
        (prev_dir == DIR_DOWN && next_dir == DIR_LEFT) ||
        (prev_dir == DIR_LEFT && next_dir == DIR_UP)) {
        return CORNER_RIGHT;
    }
    
    /* Coin à gauche (sens anti-horaire) */
    if ((prev_dir == DIR_UP && next_dir == DIR_LEFT) ||
        (prev_dir == DIR_LEFT && next_dir == DIR_DOWN) ||
        (prev_dir == DIR_DOWN && next_dir == DIR_RIGHT) ||
        (prev_dir == DIR_RIGHT && next_dir == DIR_UP)) {
        return CORNER_LEFT;
    }
    
    return CORNER_NONE;
}

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
    snake->buffer = DIR_NONE;
    snake->has_next_direction = 0;


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
        snake->segments[i].corner = CORNER_NONE;  /* Pas de coin au départ */
        set_cell(grid, &snake->segments[i].position, CELL_SNAKE);
    }
}

/* Retourne la position de la tête */
Position get_head_position(Snake snake) {
    return snake.segments[snake.head_index].position;
}

/* Vérifie si la direction demandée n'est pas l'opposée de l'actuelle */
int is_valid_direction_change(Direction current, Direction new_dir) {
    int valid = 1;
    /* Si la direction demandée est l'opposée de l'actuelle, on retourne 0 */
    if (new_dir == DIR_NONE) {
        valid = 0;
    } else if (current == DIR_UP && new_dir == DIR_DOWN) {
        valid = 0;
    } else if (current == DIR_DOWN && new_dir == DIR_UP) {
        valid = 0;
    } else if (current == DIR_LEFT && new_dir == DIR_RIGHT) {
        valid = 0;
    } else if (current == DIR_RIGHT && new_dir == DIR_LEFT) {
        valid = 0;
    }

    return valid;
}

void set_snake_direction(Snake *snake, Direction new_dir) {
    /* Si la direction demandée est valide on la met à jour et on vide le buffer */
    if (
        is_valid_direction_change(snake->segments[snake->head_index].direction, new_dir) &&
        snake->has_next_direction == 0
    ) {
        snake->segments[snake->head_index].direction = new_dir;
        snake->has_next_direction = 1;
        snake->buffer             = DIR_NONE;

    /* Si le buffer n'est pas vide et la direction demandée est valide, on la met à jour */
    } else if (
        snake->has_next_direction == 0 &&
        snake->buffer != DIR_NONE &&
        is_valid_direction_change(snake->segments[snake->head_index].direction, snake->buffer)
    ) {
        snake->segments[snake->head_index].direction = snake->buffer;
        snake->has_next_direction = 1;
        snake->buffer = DIR_NONE;
    } else if (
        snake->has_next_direction == 1 &&
        snake->buffer == DIR_NONE &&
        is_valid_direction_change(snake->segments[snake->head_index].direction, new_dir)
    ) {
        snake->buffer = new_dir;
    } else if (
        snake->has_next_direction == 1 &&
        snake->buffer != DIR_NONE &&
        is_valid_direction_change(snake->segments[snake->head_index].direction, new_dir)
    ) {
        snake->buffer = new_dir;
    }
}

/* Déplace le serpent */
void move_snake(Grid *grid, Snake *snake) {
    int old_head_idx, prev_idx;
    Direction old_dir, new_dir;
    Segment new_head;
    
    old_head_idx = snake->head_index;
    old_dir = snake->segments[old_head_idx].direction;
    new_dir = old_dir;
    
    /* Copier la position actuelle */
    new_head.position = snake->segments[old_head_idx].position;
    new_head.direction = new_dir;
    new_head.corner = CORNER_NONE;

    /* Calculer la nouvelle position */
    switch (new_dir) {
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

    /* Calculer le coin de l'ancien segment de tête (il devient corps) */
    prev_idx = (old_head_idx - 1 + MAX_SNAKE_LENGTH) % MAX_SNAKE_LENGTH;
    snake->segments[old_head_idx].corner = get_corner_type(
        snake->segments[prev_idx].direction,
        old_dir
    );

    /* Avancer l'index de la tête (Circulaire) */
    snake->head_index = (snake->head_index + 1) % MAX_SNAKE_LENGTH;

    /* D'abord on efface l'ancienne queue pour éviter d'effacer la tête si elle arrive au même endroit */
    set_cell(grid, &snake->segments[snake->tail_index].position, CELL_EMPTY);
    snake->tail_index = (snake->tail_index + 1) % MAX_SNAKE_LENGTH;

    /* Ensuite on affiche la nouvelle tête */
    snake->segments[snake->head_index] = new_head;
    set_cell(grid, &new_head.position, CELL_SNAKE);
}

void grow_snake(Grid *grid, Snake *snake) {
    int old_head_idx, prev_idx;
    Direction old_dir, new_dir;
    Segment new_head;
    
    old_head_idx = snake->head_index;
    old_dir = snake->segments[old_head_idx].direction;
    new_dir = old_dir;
    
    /* Copier la position actuelle */
    new_head.position = snake->segments[old_head_idx].position;
    new_head.direction = new_dir;
    new_head.corner = CORNER_NONE;

    /* Calculer la nouvelle position */
    switch (new_dir) {
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

    /* Calculer le coin de l'ancien segment de tête (il devient corps) */
    prev_idx = (old_head_idx - 1 + MAX_SNAKE_LENGTH) % MAX_SNAKE_LENGTH;
    snake->segments[old_head_idx].corner = get_corner_type(
        snake->segments[prev_idx].direction,
        old_dir
    );

    /* Avancer l'index de la tête (Circulaire) */
    snake->head_index = (snake->head_index + 1) % MAX_SNAKE_LENGTH;

    /* Écrire la nouvelle position */
    snake->segments[snake->head_index] = new_head;
    set_cell(grid, &new_head.position, CELL_SNAKE);

    snake->length++;
}


/* On récupére la valeur de la cellule suivante */
CellType get_next_cell_value(Grid *grid, Snake *snake) {
    Position next_position;
    next_position.x = snake->segments[snake->head_index].position.x;
    next_position.y = snake->segments[snake->head_index].position.y;

    switch (snake->segments[snake->head_index].direction) {
        case DIR_UP:
            next_position.y -= 1;
            break;
        case DIR_DOWN:
            next_position.y += 1;
            break;
        case DIR_LEFT:
            next_position.x -= 1;
            break;
        case DIR_RIGHT:
            next_position.x += 1;
            break;
        default:
            break;
    }

    if (next_position.x < 0 || next_position.x >= grid->width || next_position.y < 0 || next_position.y >= grid->height) {
        return CELL_WALL;
    }

    /* Si la prochaine case est la queue, ce n'est pas une collision car elle va bouger */
    if (next_position.x == snake->segments[snake->tail_index].position.x && 
        next_position.y == snake->segments[snake->tail_index].position.y) {
        return CELL_EMPTY;
    }

    return get_cell(grid, &next_position);
}
