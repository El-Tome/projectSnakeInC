#ifndef GRID_DISPLAY_H
#define GRID_DISPLAY_H

#include "grille.h"
#include "main.h"

typedef struct {
    int width;
    int height;
} CellSize;

/* Offset pour centrer la grille dans la fenêtre */
typedef struct {
    int x;
    int y;
} GridOffset;

/* Calcule la taille des cellules (carrées) */
CellSize get_cell_size(Grid *grid, WindowSize window_size);

/* Calcule l'offset pour centrer la grille */
GridOffset get_grid_offset(Grid *grid, WindowSize window_size, CellSize cell_size);

void draw_cell(CellSize cell_size, GridOffset offset, Position *p, CellType cell);

void draw_grid(Grid *grid, WindowSize window_size);

#endif
