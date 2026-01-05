#include "food.h"
#include "grille.h"

int get_list_of_empty_cells(Grid *grid, Position empty_cells[]) {
    Position p;
    int i, j, empty_cells_count = 0;
    
    for (i = 0; i < grid->width; i++) {
        for (j = 0; j < grid->height; j++) {
            p.x = i;
            p.y = j;
            if (get_cell(grid, &p) == CELL_EMPTY) {
                empty_cells[empty_cells_count] = p;
                empty_cells_count++;
            }
        }
    }
    return empty_cells_count;
}


/*Fonctions :*/
void spawn_food(Grid *grid, FoodList *food_list, int value, int nb_to_spawn){
    Position p, empty_cells[NB_CELLS];
    int i, j, empty_cells_count, random_index, index_used[NB_CELLS], index_used_count = 0, found = 0;

    /* On récupère la liste des cellules vides */
    empty_cells_count = get_list_of_empty_cells(grid, empty_cells);

    /* On spawn la/les nourriture/s */
    for (i = 0; i < nb_to_spawn && empty_cells_count > 0; i++) {
        /* On choisit une cellule vide aléatoire */
        /* On boucle tant que la cellule est déjà utilisée */
        found = 0;
        while (!found && empty_cells_count > 0) {
            random_index = rand() % empty_cells_count;
            found = 1;

            /* On vérifie si la cellule est déjà utilisée */
            for (j = 0; j < index_used_count; j++) {
                if (index_used[j] == random_index) {
                    found = 0;
                }
            }
        }

        /* On marque la cellule comme utilisée */
        index_used[index_used_count] = random_index;
        index_used_count++;

        /* On récupère la cellule vide */
        p = empty_cells[random_index];

        /* On ajoute la nourriture à la liste */
        food_list->foods[food_list->food_count+i].position  = p;
        food_list->foods[food_list->food_count+i].value     = value;
        food_list->foods[food_list->food_count+i].is_active = 1;

        /* On augmente le compteur de nourriture */
        food_list->food_count++;

        /* On marque la cellule comme nourriture */
        set_cell(grid, &p, CELL_FOOD);

        /* On retire la cellule de la liste des cellules vides */
        empty_cells_count--;
    }
}

void spawn_obstacle(Grid *grid, int nb_to_spawn) {
    Position p, empty_cells[NB_CELLS];
    int i, j, empty_cells_count, random_index, index_used[NB_CELLS], index_used_count = 0, found = 0;

    /* On récupère la liste des cellules vides */
    empty_cells_count = get_list_of_empty_cells(grid, empty_cells);

    /* On spawn le/les obstacle/s */
    for (i = 0; i < nb_to_spawn && empty_cells_count > 0; i++) {
        /* On choisit une cellule vide aléatoire */
        found = 0;
        while (!found && empty_cells_count > 0) {
            random_index = rand() % empty_cells_count;
            found = 1;

            /* On vérifie si la cellule est déjà utilisée */
            for (j = 0; j < index_used_count; j++) {
                if (index_used[j] == random_index) {
                    found = 0;
                }
            }
        }

        /* On marque la cellule comme utilisée */
        index_used[index_used_count] = random_index;
        index_used_count++;

        /* On récupère la cellule vide */
        p = empty_cells[random_index];

        /* On marque la cellule comme obstacle (pierre/mur) */
        set_cell(grid, &p, CELL_WALL);

        /* On retire la cellule de la liste des cellules vides */
        empty_cells_count--;
    }
}
