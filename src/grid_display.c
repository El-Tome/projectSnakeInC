#include "grid_display.h"
#include "snake_display.h"
#include "MLV/MLV_color.h"

CellSize get_cell_size(Grid *grid, WindowSize window_size) {
    CellSize cell_size;
    int cell_width, cell_height, min_size;
    
    /* Calcul des tailles possibles */
    cell_width  = (window_size.width - 4)  / grid->width;
    cell_height = (window_size.height - 4) / grid->height;
    
    /* On prend le minimum pour avoir des cases carrées */
    min_size = (cell_width < cell_height) ? cell_width : cell_height;
    
    /* On s'assure que la taille est un multiple de 32 pour les sprites */
    /* ou au minimum 32 pixels */
    if (min_size < SPRITE_SIZE) {
        min_size = SPRITE_SIZE;
    }
    
    cell_size.width  = min_size;
    cell_size.height = min_size;
    
    return cell_size;
}

/* Calcule l'offset pour centrer la grille dans la fenêtre */
GridOffset get_grid_offset(Grid *grid, WindowSize window_size, CellSize cell_size) {
    GridOffset offset;
    int grid_total_width, grid_total_height;
    
    grid_total_width  = grid->width  * cell_size.width;
    grid_total_height = grid->height * cell_size.height;
    
    /* Centrage horizontal et vertical */
    offset.x = (window_size.width  - grid_total_width)  / 2;
    offset.y = (window_size.height - grid_total_height) / 2;
    
    return offset;
}

void draw_cell(CellSize cell_size, GridOffset offset, Position *p, CellType cell) {
    int x, y;

    x = offset.x + p->x * cell_size.width;
    y = offset.y + p->y * cell_size.height;

    switch (cell) {
        case CELL_EMPTY:
            MLV_draw_filled_rectangle(x + 2, y + 2, cell_size.width - 4, cell_size.height - 4, MLV_rgba(0,0,0,255));
            break;
        
        case CELL_SNAKE:
            /* Le serpent sera dessiné par snake_display, on dessine juste le fond */
            MLV_draw_filled_rectangle(x + 2, y + 2, cell_size.width - 4, cell_size.height - 4, MLV_rgba(0, 0, 0, 255));
            break;
        
        case CELL_FOOD:
            MLV_draw_filled_rectangle(x + 2, y + 2, cell_size.width - 4, cell_size.height - 4, MLV_rgba(255, 0, 0, 255));
            break;
        
        case CELL_WALL:
            MLV_draw_filled_rectangle(x + 2, y + 2, cell_size.width - 4, cell_size.height - 4, MLV_rgba(255, 255, 255, 255));
            break;
        
        default:
            break;
    }
}

void draw_grid(Grid *grid, WindowSize *window_size) {
    int i, j;
    Position p;
    CellSize cell_size;
    GridOffset offset;

    MLV_draw_filled_rectangle(0, 0, window_size->width, window_size->height, MLV_rgba(50, 50, 50, 255));
    cell_size = get_cell_size(grid, *window_size);
    offset = get_grid_offset(grid, *window_size, cell_size);

    for (i = 0; i < grid->width; i++) {
        for (j = 0; j < grid->height; j++) {
            p.x = i;
            p.y = j;
            draw_cell(cell_size, offset, &p, get_cell(grid, &p));
        }
    }
}
