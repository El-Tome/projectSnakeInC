#include "apple_display.h"
#include <stdio.h>

#define APPLE_PATH "ressources/assets/pomme.png"

static MLV_Image *apple_img = NULL;
static int current_size = 0;

int apple_display_init(void) {
    apple_img = MLV_load_image(APPLE_PATH);
    if (apple_img == NULL) {
        fprintf(stderr, "Erreur chargement image pomme : %s\n", APPLE_PATH);
        return 0;
    }
    current_size = 0;
    return 1;
}

void apple_display_resize(int cell_size) {
    if (apple_img == NULL)
        return;

    if (current_size != cell_size) {
        MLV_resize_image(apple_img, cell_size, cell_size);
        current_size = cell_size;
    }
}

void apple_display_draw(Position pos, int cell_size, int ox, int oy) {
    int x, y;

    if (apple_img == NULL)
        return;

    apple_display_resize(cell_size);

    x = ox + pos.x * cell_size;
    y = oy + pos.y * cell_size;

    MLV_draw_image(apple_img, x, y);
}

void apple_display_free(void) {
    if (apple_img != NULL) {
        MLV_free_image(apple_img);
        apple_img = NULL;
    }
}