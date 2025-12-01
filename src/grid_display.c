#include "grid_display.h"
#include "MLV/MLV_color.h"

CellSize get_cell_size(Grid *grid, WindowSize window_size) {
    CellSize cell_size;
    
    cell_size.width  = (window_size.width - 4)  / grid->width;
    cell_size.height = (window_size.height - 4) / grid->height;
    
    return cell_size;
}

void draw_cell(CellSize cell_size, Position *p, CellType cell) {
    int x, y;

    x = p->x * cell_size.width;
    y = p->y * cell_size.height;

    switch (cell) {
        case CELL_EMPTY:
            MLV_draw_filled_rectangle(x + 2, y + 2, cell_size.width - 4, cell_size.height - 4, MLV_rgba(0,0,0,255));
            break;
        
        case CELL_SNAKE:
            MLV_draw_filled_rectangle(x + 2, y + 2, cell_size.width - 4, cell_size.height - 4, MLV_rgba(0, 255, 0, 255));
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

void draw_grid(Grid *grid, WindowSize window_size) {
    int i, j;
    Position p;
    CellSize cell_size;


    MLV_draw_filled_rectangle(0, 0, window_size.width, window_size.height, MLV_rgba(255, 255, 255, 255));
    cell_size = get_cell_size(grid, window_size);

    for (i = 0; i < grid->width; i++) {
        for (j = 0; j < grid->height; j++) {
            p.x = i;
            p.y = j;
            draw_cell(cell_size, &p, get_cell(grid, &p));
        }
    }
}

