#ifndef GRILLE_H
#define GRILLE_H

#define MAX_GRID_WIDTH 40   /* Largeur en cases */
#define MAX_GRID_HEIGHT 30  /* Hauteur en cases */
#define MIN_GRID_WIDTH 10   /* Largeur minimale en cases */
#define MIN_GRID_HEIGHT 10  /* Hauteur minimale en cases */
#define NB_CELLS 1200

#include <stdio.h>


typedef enum {
    CELL_EMPTY,
    CELL_SNAKE,
    CELL_FOOD,
    CELL_WALL,
    CELL_BONUS,
    CELLE_SHADOW,
    CELL_PATES
} CellType;

/* Tableau  2D statique */
typedef struct {
    CellType cells[MAX_GRID_HEIGHT][MAX_GRID_WIDTH];  /* Tableau statique */
    int width;                                  /* = MAX_GRID_WIDTH */
    int height;                                 /* = MAX_GRID_HEIGHT */
    int has_borders;                            /* 0 ou 1 */
} Grid;


typedef struct {
    int x;
    int y;
} Position;




/*Fonctions :*/
Grid init_grid(int width, int height, int has_borders);

void clear_grid(Grid *grid);

CellType get_cell(Grid *grid, Position *p);

void set_cell(Grid *grid, Position *p, CellType type);

#endif

