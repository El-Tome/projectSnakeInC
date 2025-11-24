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
    ENTER
} ToucheClavier;


MLV_Keyboard_button get_key_pressed();

ToucheClavier convert_key_to_enum(MLV_Keyboard_button key);

void clear_event();


#endif
