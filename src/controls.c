#include "controls.h"

MLV_Keyboard_button get_key_pressed() {
    MLV_Keyboard_button touche;
    MLV_Button_state state;

    MLV_get_event (/*Si une touche est presser les infos que l'on prend sont :*/
        &touche,   /*La touche en question*/
        NULL,      /*les nulls servent a ne pas déclarer le reste de la fonction*/
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        &state     /*etat de la touche*/
    ); 
    
    if (state == MLV_PRESSED) {   /*si une touche est presser, on retourne la touche en question */
        return touche;
    } else {
        return MLV_KEYBOARD_NONE; /*rien n'est renvoyer car rien n'est pressé */
    }
}

ToucheClavier convert_key_to_enum(MLV_Keyboard_button key) { /*Fonction qui permert d'attibuer une touche MLV a une touche du programme*/
    ToucheClavier touche = NONE;

    switch (key) {             /*switch est un if/else plus obtimiser quand on a plus de 2 élément*/
        case MLV_KEYBOARD_UP:  /*case = if, la touche pressé est UP */
            touche = UP;       /*assigne à la varible touche: UP*/
            break;             /*fin du programme pour ne pas à avoir à tester les choses*/
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


void clear_event() {
    MLV_Keyboard_button key; /*variable "key" de type MLV_Keyboard_button*/

    while (
        MLV_get_event (      /*les infos prises a chaque fois sont :*/
        &key,                /*l'adresse par références de la varible key*/
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


