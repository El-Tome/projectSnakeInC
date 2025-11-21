#ifndef FOOD_H
#define FOOD_H
#define MAX_CASE (GRID_WIDTH * GRID_HEIGHT)
#include "grille.h"


typedef struct {
    Position pos;
    int value;
    int is_active;
} Food;

#endif
