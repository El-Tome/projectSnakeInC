#include "controls.h"

MLV_Keyboard_button get_key_pressed() {
    MLV_Keyboard_button touche;

    MLV_get_event (
        &touche, 
        NULL, 
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    ); 
    
    return touche;
}

ToucheClavier convert_key_to_enum(MLV_Keyboard_button key) {
    ToucheClavier touche = NONE;

    switch (key) {
        case MLV_KEYBOARD_UP:
            touche = UP;
            break;
        case MLV_KEYBOARD_DOWN:
            touche = DOWN;
            break;
        case MLV_KEYBOARD_RIGHT:
            touche = RIGHT;
            break;
        case MLV_KEYBOARD_LEFT:
            touche = LEFT;
            break;
        case MLV_KEYBOARD_ESCAPE:
            touche = ESCAPE;
            break;
        default:
            break;
    }
    return touche;
}


void clear_event() {
    MLV_Keyboard_button key;

    while (
        MLV_get_event (
        &key, 
        NULL, 
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    ) != MLV_NONE) {
        ;
    }
}



