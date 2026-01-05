#ifndef ASSETS_DISPLAY_H
#define ASSETS_DISPLAY_H

#include <MLV/MLV_all.h>
#include "grille.h"

/* Taille par défaut des sprites (en pixels) */
#define ASSET_DEFAULT_SIZE 32

/* Chemins vers les assets */
#define APPLE_PATH    "ressources/assets/apple.png"
#define BONUS_PATH    "ressources/assets/bonus.png"
#define WALL_PATH     "ressources/assets/mur.png"
#define OBSTACLE_PATH "ressources/assets/obstacle.png"

/* Structure pour un asset individuel */
typedef struct {
    MLV_Image *img;           /* Image originale */
    MLV_Image *img_resized;   /* Image redimensionnée pour la taille actuelle */
    int current_size;         /* Taille actuelle de l'image redimensionnée */
    int is_loaded;            /* 1 si l'image est chargée, 0 sinon */
    MLV_Color default_color;  /* Couleur de fallback si image non chargée */
} AssetDisplay;

/* Structure contenant tous les assets du jeu */
typedef struct {
    AssetDisplay apple;       /* Pomme (nourriture normale) */
    AssetDisplay bonus;       /* Bonus */
    AssetDisplay wall;        /* Mur */
    AssetDisplay obstacle;    /* Obstacle */
} AllAssetsDisplay;

/* Charge tous les assets du jeu */
void load_all_assets(AllAssetsDisplay *assets);

/* Libère tous les assets du jeu */
void free_all_assets(AllAssetsDisplay *assets);

/* Redimensionne tous les assets à la nouvelle taille de cellule */
void resize_all_assets(AllAssetsDisplay *assets, int cell_size);

/* ============== Fonctions pour un asset individuel ============== */

/* Charge un asset individuel */
int load_asset(AssetDisplay *asset, const char *path, MLV_Color default_color);

/* Libère un asset individuel */
void free_asset(AssetDisplay *asset);

/* Redimensionne un asset individuel */
void resize_asset(AssetDisplay *asset, int new_size);

/* ============== Fonctions de dessin ============== */

/* Dessine un asset à une position donnée sur la grille */
void draw_asset(AssetDisplay *asset, Position pos, int cell_size, int offset_x, int offset_y);

/* Fonctions de dessin spécifiques */
void apple_display_draw(AllAssetsDisplay *assets, Position pos, int cell_size, int offset_x, int offset_y);
void bonus_display_draw(AllAssetsDisplay *assets, Position pos, int cell_size, int offset_x, int offset_y);
void wall_display_draw(AllAssetsDisplay *assets, Position pos, int cell_size, int offset_x, int offset_y);
void obstacle_display_draw(AllAssetsDisplay *assets, Position pos, int cell_size, int offset_x, int offset_y);

#endif
