#include "assets_display.h"



int load_asset(AssetDisplay *asset, const char *path, MLV_Color default_color) {
    asset->img = MLV_load_image(path);
    asset->img_resized = NULL;
    asset->current_size = 0;
    asset->default_color = default_color;
    
    if (asset->img == NULL) {
        asset->is_loaded = 0;
        return 0;
    }
    
    asset->is_loaded = 1;
    return 1;
}

void free_asset(AssetDisplay *asset) {
    if (asset->img != NULL) {
        MLV_free_image(asset->img);
        asset->img = NULL;
    }
    if (asset->img_resized != NULL) {
        MLV_free_image(asset->img_resized);
        asset->img_resized = NULL;
    }
    asset->is_loaded = 0;
    asset->current_size = 0;
}

void resize_asset(AssetDisplay *asset, int new_size) {
    /* Si l'asset n'est pas chargé ou si la taille est la même, on ne fait rien */
    if (asset->is_loaded && asset->current_size != new_size) {
        /* Libérer l'ancienne image redimensionnée si elle existe */
        if (asset->img_resized != NULL) {
            MLV_free_image(asset->img_resized);
            asset->img_resized = NULL;
        }
        
        /* Créer une copie de l'image originale et la redimensionner */
        asset->img_resized = MLV_copy_image(asset->img);
        if (asset->img_resized != NULL) {
            MLV_resize_image(asset->img_resized, new_size, new_size);
            asset->current_size = new_size;
        }
    }
}

/* ============== Fonctions de gestion globale ============== */

void load_all_assets(AllAssetsDisplay *assets) {
    load_asset(&assets->apple,    APPLE_PATH,    MLV_COLOR_RED);
    load_asset(&assets->bonus,    BONUS_PATH,    MLV_COLOR_YELLOW);
    load_asset(&assets->wall,     WALL_PATH,     MLV_COLOR_GRAY);
    load_asset(&assets->obstacle, OBSTACLE_PATH, MLV_COLOR_DARK_GRAY);
}

void free_all_assets(AllAssetsDisplay *assets) {
    free_asset(&assets->apple);
    free_asset(&assets->bonus);
    free_asset(&assets->wall);
    free_asset(&assets->obstacle);
}

void resize_all_assets(AllAssetsDisplay *assets, int cell_size) {
    resize_asset(&assets->apple, cell_size);
    resize_asset(&assets->bonus, cell_size);
    resize_asset(&assets->wall, cell_size);
    resize_asset(&assets->obstacle, cell_size);
}

/* ============== Fonctions de dessin ============== */

void draw_asset(AssetDisplay *asset, Position pos, int cell_size, int offset_x, int offset_y) {
    int x, y;
    
    /* Calculer la position en pixels */
    x = offset_x + pos.x * cell_size;
    y = offset_y + pos.y * cell_size;
    
    /* Si l'asset est chargé, dessiner l'image */
    if (asset->is_loaded) {
        /* Redimensionner si nécessaire */
        if (asset->current_size != cell_size) {
            resize_asset(asset, cell_size);
        }
        
        /* Dessiner l'image redimensionnée si disponible, sinon l'originale */
        if (asset->img_resized != NULL) {
            MLV_draw_image(asset->img_resized, x, y);
        } else {
            MLV_draw_image(asset->img, x, y);
        }
    } else {
        /* Fallback: dessiner un rectangle coloré */
        MLV_draw_filled_rectangle(
            x + 2, 
            y + 2, 
            cell_size - 4, 
            cell_size - 4, 
            asset->default_color
        );
    }
}

/* ============== Fonctions de dessin spécifiques ============== */

void apple_display_draw(AllAssetsDisplay *assets, Position pos, int cell_size, int offset_x, int offset_y) {
    draw_asset(&assets->apple, pos, cell_size, offset_x, offset_y);
}

void bonus_display_draw(AllAssetsDisplay *assets, Position pos, int cell_size, int offset_x, int offset_y) {
    draw_asset(&assets->bonus, pos, cell_size, offset_x, offset_y);
}

void wall_display_draw(AllAssetsDisplay *assets, Position pos, int cell_size, int offset_x, int offset_y) {
    draw_asset(&assets->wall, pos, cell_size, offset_x, offset_y);
}

void obstacle_display_draw(AllAssetsDisplay *assets, Position pos, int cell_size, int offset_x, int offset_y) {
    draw_asset(&assets->obstacle, pos, cell_size, offset_x, offset_y);
}
