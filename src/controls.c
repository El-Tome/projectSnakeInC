#include "controls.h"

MLV_Keyboard_button get_key_pressed() {
    MLV_Keyboard_button touche;
    MLV_Button_state state;

    MLV_get_event (
        &touche, 
        NULL, 
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        &state
    ); 
    
    if (state == MLV_PRESSED) {
        return touche;
    } else {
        return MLV_KEYBOARD_NONE;
    }
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
        case MLV_KEYBOARD_RETURN:
            touche = ENTER;
            break;
        default:
            break;
    }
    return touche;
}

ToucheClavier convert_key_to_enum_p2(MLV_Keyboard_button key) {
    ToucheClavier touche = NONE;

    switch (key) {
        case MLV_KEYBOARD_z:
            touche = P2_UP;
            break;
        case MLV_KEYBOARD_s:
            touche = P2_DOWN;
            break;
        case MLV_KEYBOARD_d:
            touche = P2_RIGHT;
            break;
        case MLV_KEYBOARD_q:
            touche = P2_LEFT;
            break;
        default:
            break;
    }
    return touche;
}

void get_event_two_players(ToucheClavier *p1_key, ToucheClavier *p2_key) {
    MLV_Keyboard_button key;
    MLV_Button_state state;
    MLV_Event event;

    *p1_key = NONE;
    *p2_key = NONE;

    event = MLV_get_event(
        &key,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        &state
    );

    if (event == MLV_KEY && state == MLV_PRESSED) {
        /* Vérifie si c'est une touche joueur 1 */
        *p1_key = convert_key_to_enum(key);
        /* Vérifie si c'est une touche joueur 2 */
        *p2_key = convert_key_to_enum_p2(key);
    }
}

ToucheClavier get_event() {
    MLV_Keyboard_button key;
    MLV_Mouse_button    mouse_button;
    MLV_Button_state    state;
    MLV_Event           event;

    event = MLV_get_event(
        &key,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    &mouse_button,
        &state
    );

    if (event == MLV_KEY && state == MLV_PRESSED) {
        return convert_key_to_enum(key);
    } else if (event == MLV_MOUSE_BUTTON && state == MLV_PRESSED && mouse_button == MLV_BUTTON_LEFT) {
        return MOUSE_LEFT_CLICK;
    }

    return NONE;
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


