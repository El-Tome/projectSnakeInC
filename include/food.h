#ifndef FOOD_H
#define FOOD_H

#define MAX_FOOD_COUNT 100

#include "grille.h"
#include "snake.h"

typedef struct {
    Position position;
    int value;
    int is_active;
} Food;

typedef struct {
    Food foods[MAX_FOOD_COUNT];
    int food_count;
} FoodList;

void spawn_food(Grid *grid, FoodList *food_list, int value, int nb_to_spawn);

/* Fait apparaître nb obstacles (pierres) aléatoirement sur la grille */
void spawn_obstacle(Grid *grid, int nb_to_spawn);

#endif
