#ifndef COMBAT_BOSS_H
#define COMBAT_BOSS_H

#include "grille.h"

#define LARGEUR_PATES   3
#define LONGUEUR_PATES  3

#define DUREE_OMBRE_MS  5000
#define DUREE_PATTE_MS  5000

#define BOSS_MAX_HP     5


/* États de la patte */
typedef enum {
    PATTE_INACTIVE,
    PATTE_OMBRE,
    PATTE_ACTIVE
} PatteState;

/* Structure d'une patte */
typedef struct {
    Position centre; 
    PatteState state;
    int timer_ms; /*temp que la patte reste*/
    int is_alive; /*0 = pas sur le terrain 1 = sur le terrain*/
} Patte;

/* Structure du boss */
typedef struct {
    int hp;
    Patte patte : 1; /* nb patte */
} Boss;


void boss_init(Boss *boss);
void boss_update(Boss *boss, Grid *grid, int delta_ms);
void boss_on_patte_hit(Boss *boss, Grid *grid);

#endif