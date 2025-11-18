#ifndef FOOD_H
#define FOOD_H

#include "grille.h"

typedef struct {
    Position pos;
    int value;
    int is_active;
} Food;

#endif
