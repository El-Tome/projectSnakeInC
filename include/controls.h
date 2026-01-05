#ifndef CONTROLS_H
#define CONTROLS_H

#include <MLV/MLV_all.h>

typedef enum {
    NONE,
    UP,
    DOWN,
    RIGHT,
    LEFT,
    ESCAPE,
    ENTER,
    MOUSE_LEFT_CLICK,
    MOUSE_RIGHT_CLICK,
    /* Touches Joueur 2 (ZQSD) */
    P2_UP,
    P2_DOWN,
    P2_RIGHT,
    P2_LEFT
} ToucheClavier;


MLV_Keyboard_button get_key_pressed();

ToucheClavier convert_key_to_enum(MLV_Keyboard_button key);

/* Convertit la touche en entrée joueur 2 (ZQSD) */
ToucheClavier convert_key_to_enum_p2(MLV_Keyboard_button key);

/* Récupère l'événement pour les 2 joueurs simultanément */
void get_event_two_players(ToucheClavier *p1_key, ToucheClavier *p2_key);

ToucheClavier get_event();

void clear_event();


#endif
