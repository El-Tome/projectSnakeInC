
/*Fonctions :*/
/*
void init_grid(Grid *grid, int has_borders)            - Initialise tableau
void clear_grid(Grid *grid)                            - Remplit de CELL_EMPTY
void update_grid_with_snake(Grid *grid, Snake snake)   - Marque cellules serpent
void update_grid_with_food(Grid *grid, Position food)  - Marque cellule nourriture
CellType get_cell(Grid grid, Position p)               - Lecture cellule
void set_cell(Grid *grid, Position p, CellType type)   - Écriture cellule
*/
int test(Grid *grid){ /*Si la hauteur/longueur sont impossible*/
    if (width<0 || height<0)
    {
        printf("Le nombre de case est trop petit ! Erreur dans grille.c");
        return 0;
    }
    else if (width>GRID_WIDTH || height>GRID_HEIGHT){
        printf("Le nombre de case est trop grand ! Erreur dans grille.c");
        return 0
            }
    return 1
        }


    
    void clear_grid(Grid *grid){
        int i, j;
        for (i=0; i<width; i++)
        {
            for (j=0; j<height; j++)
            {
                grid.cells[i][j]=CELL_EMPTY;
            }
        }
    }

void init_grid(Grid *grid) {
    clear_grid(Grid *grid);
    grid->has_borders = has_borders ;
    
}

CellType get_cell(Grid grid, Position p){
    return grid.cells[p.x][p.y];
            
}

void set_cell(Grid *grid, Position p, CellType type){
    grid.cells[p.x][p.y] = type;
}
