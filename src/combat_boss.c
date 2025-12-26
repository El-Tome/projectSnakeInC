#include <stdlib.h>
#include "combat_boss.h"
#include "food.h" /*pour get_list_of_empty_cells*/


/*      vérification        */
int can_spawn_patte(Grid *grid, Position centre) {
    int i, j;
    Position p;

    for (i = -LARGEUR_PATES / 2; i <= LARGEUR_PATES / 2; i++) {         /*ex -3/2 3/2 donne dx=> {-1;0;1} */
        for (j = -LONGUEUR_PATES / 2; j <= LONGUEUR_PATES / 2; j++) {
            p.x = centre.x + i;
            p.y = centre.y + j;

            /* Sort de la grille */
            if (p.x < 0 || p.x >= grid->width ||
                p.y < 0 || p.y >= grid->height) {
                return 0;
            }

            /* Case occupée */
            if (get_cell(grid, &p) != CELL_EMPTY) {
                return 0;
            }
        }
    }
    return 1;
}



void fill_zone(Grid *grid, Position centre, CellType type) {
    int dx, dy;
    Position p;

    for (dx = -LARGEUR_PATES / 2; dx <= LARGEUR_PATES / 2; dx++) {
        for (dy = -LONGUEUR_PATES / 2; dy <= LONGUEUR_PATES / 2; dy++) {
            p.x = centre.x + dx;
            p.y = centre.y + dy;
            set_cell(grid, &p, type);
        }
    }
}




Position random_valid_position(Grid *grid) {
    Position empty_cells[NB_CELLS];
    int count = get_list_of_empty_cells(grid, empty_cells);
    Position p = {0, 0};

    if (count == 0)
        return p;

    for (int i = 0; i < 100; i++) {
        p = empty_cells[rand() % count];
        if (can_spawn_zone(grid, p))
            return p;
    }
    return p;
}


/*      BOSS        */
void boss_init(Boss *boss) {
    boss->hp = BOSS_MAX_HP;
    boss->patte.state = PATTE_INACTIVE;
    boss->patte.timer_ms = 0;
    boss->patte.is_alive = 0;
}


/*      Les différent états possibles       */





void boss_update(Boss *boss, Grid *grid, int delta_ms) {
    Patte *p = &boss->patte;

    if (boss->hp <= 0)
        return;/*écran de victoire*/

    switch (p->state) {

    /*        INACTIF        */
    case PATTE_INACTIVE:
        p->centre = random_valid_position(grid);
        p->state = PATTE_OMBRE;
        p->timer_ms = DUREE_OMBRE_MS;
        p->is_alive = 1;
        fill_zone(grid, p->centre, CELL_SHADOW); /*La zone d'attaque est prête avec son ombre*/
        break;

    /*        OMBRE         */
    case PATTE_OMBRE:
        p->timer_ms -= delta_ms;
        if (p->timer_ms <= 0) {
            fill_zone(grid, p->centre, CELL_PATTES); /* la patte tombe */
            p->state = PATTE_ACTIVE;
            p->timer_ms = DUREE_PATTE_MS;
        }
        break;

    /*        PATTE ACTIVE       */
    case PATTE_ACTIVE:
        p->timer_ms -= delta_ms;
        if (p->timer_ms <= 0) {
            fill_zone(grid, p->centre, CELL_EMPTY); /* la patte repart */
            p->state = PATTE_INACTIVE;
        }
        break;
    }
}

/* Appelé quand le serpent touche une patte */
void boss_on_patte_hit(Boss *boss, Grid *grid) {
    Patte *p = &boss->patte;

    if (p->state == PATTE_ACTIVE && p->is_alive) {
        boss->hp--;
        p->is_alive = 0;
        fill_zone(grid, p->centre, CELL_EMPTY);
        p->state = PATTE_INACTIVE;
    }
}

