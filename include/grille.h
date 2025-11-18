#ifndef GRILLE_H
#define GRILLE_H

#define GRID_WIDTH 40   /* Largeur en cases */
#define GRID_HEIGHT 30  /* Hauteur en cases */

#include <stdio.h>
#include <stdlib.h>


typedef enum {
    CELL_EMPTY,
    CELL_SNAKE,
    CELL_FOOD,
    CELL_WALL,
    CELL_BONUS
} CellType;

/* Tableau  2D statique */
typedef struct {
    CellType cells[GRID_HEIGHT][GRID_WIDTH];  /* Tableau statique */
    int width;                                  /* = GRID_WIDTH */
    int height;                                 /* = GRID_HEIGHT */
    int has_borders;                            /* 0 ou 1 */
} Grid;


typedef struct {
    int x;
    int y;
} Position;




/*Fonctions :*/
/*
void init_grid(Grid *grid, int has_borders)            - Initialise tableau
*/
#endif

