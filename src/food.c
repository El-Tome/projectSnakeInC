#include "food.h"

/*Fonctions :*/
int compte_vide(Grid *grid, Position tab[]){
    int x, y, compteur=0;
    Position posi;
    for (x = 0; (grid->width) > x; x++) {
        for (y = 0; (grid->height) > y; y++) {
            if (grid->cells[x][y] == CELL_EMPTY) {       
                posi.x = x;
                posi.y = y;

                tab[compteur] = posi;
                compteur++;
            }
        }
    }
    return compteur;
}

void generateFood(Grid *grid) {
    int nbCaseLibre, numCaseNextFood;
    Position caseLibre[MAX_CASE];
    
    nbCaseLibre = compte_vide(grid, caseLibre);

    if (nbCaseLibre > 0) {
        numCaseNextFood = rand() % nbCaseLibre;

        set_cell(grid, caseLibre[numCaseNextFood], CELL_FOOD);
    }


}
    
/* int check_food_collision(Snake snake, Food food) */  /*Retourne 1 si collision 0 sinon*/
/*void disable_food(Food *food)                     */    /* Désactive la nourriture */

    