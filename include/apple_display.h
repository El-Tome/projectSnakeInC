#ifndef APPLE_DISPLAY_H
#define APPLE_DISPLAY_H

#include <MLV/MLV_all.h>
#include "grille.h"

/* Initialise l'affichage de la pomme */
int apple_display_init(void);

/* Libère les ressources */
void apple_display_free(void);

/* Redimensionne la pomme si la taille de cellule change */
void apple_display_resize(int cell_size);

/* Dessine une pomme à la position donnée */
void apple_display_draw(Position pos, int cell_size, int ox, int oy);

#endif
