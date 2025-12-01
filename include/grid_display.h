#ifndef GRID_DISPLAY_H
#define GRID_DISPLAY_H

#include "grille.h"
#include "main.h"

typedef struct {
    int width;
    int height;
} CellSize;

void draw_cell(CellSize cell_size, Position *p, CellType cell);

void draw_grid(Grid *grid, WindowSize window_size);

#endif
