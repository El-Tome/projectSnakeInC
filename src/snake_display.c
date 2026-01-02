#include "snake_display.h"
#include "grid_display.h"
#include <stdlib.h>

/* Extrait une portion de la sprite sheet et la retourne comme nouvelle image */
static MLV_Image* extract_sprite_part(MLV_Image *sheet, int y_offset) {
    MLV_Image *part;
    int x, y;
    int r, g, b, a;
    
    part = MLV_create_image(SPRITE_SIZE, SPRITE_SIZE);
    if (part == NULL) {
        return NULL;
    }
    
    /* Copier les pixels de la portion souhaitée */
    for (y = 0; y < SPRITE_SIZE; y++) {
        for (x = 0; x < SPRITE_SIZE; x++) {
            MLV_get_pixel_on_image(sheet, x, y_offset + y, &r, &g, &b, &a);
            MLV_set_pixel_on_image(x, y, MLV_rgba(r, g, b, a), part);
        }
    }
    
    return part;
}

/* Crée une copie pivotée d'une image */
static MLV_Image* create_rotated_copy(MLV_Image *original, double angle) {
    MLV_Image *copy;
    
    copy = MLV_copy_image(original);
    if (copy != NULL && angle != 0.0) {
        MLV_rotate_image(copy, angle);
    }
    return copy;
}

/* Initialise les 4 rotations d'une partie du serpent */
static int init_part_sprites(PartSprites *part, MLV_Image *sheet, int y_offset) {
    /* Extraire la partie de base (orientée vers le haut) */
    part->up = extract_sprite_part(sheet, y_offset);
    if (part->up == NULL) {
        return 0;
    }
    
    /* Créer les versions pivotées */
    /* L'image de base pointe vers le haut (DIR_UP) */
    part->right = create_rotated_copy(part->up, 270.0);   /* 90° pour droite */
    part->down = create_rotated_copy(part->up, 180.0);   /* 180° pour bas */
    part->left = create_rotated_copy(part->up, 90.0);   /* 270° pour gauche */
    
    if (part->right == NULL || part->down == NULL || part->left == NULL) {
        return 0;
    }
    
    return 1;
}

/* Libère les sprites d'une partie */
static void free_part_sprites(PartSprites *part) {
    if (part->up != NULL) {
        MLV_free_image(part->up);
        part->up = NULL;
    }
    if (part->down != NULL) {
        MLV_free_image(part->down);
        part->down = NULL;
    }
    if (part->left != NULL) {
        MLV_free_image(part->left);
        part->left = NULL;
    }
    if (part->right != NULL) {
        MLV_free_image(part->right);
        part->right = NULL;
    }
}

/* Initialise les sprites du serpent avec les 4 rotations pour chaque partie */
int init_snake_sprites(SnakeSprites *sprites, const char *sprite_path) {
    MLV_Image *sheet;
    
    /* Charger la sprite sheet complète */
    sheet = MLV_load_image(sprite_path);
    if (sheet == NULL) {
        sprites->is_loaded = 0;
        return 0;
    }
    
    /* Initialiser chaque partie avec ses 4 rotations */
    /* Tête à l'offset 32 */
    if (!init_part_sprites(&sprites->head, sheet, SPRITE_HEAD_Y)) {
        MLV_free_image(sheet);
        free_snake_sprites(sprites);
        return 0;
    }
    
    /* Corps à l'offset 64 (on prend le premier corps) */
    if (!init_part_sprites(&sprites->body, sheet, SPRITE_BODY_1_Y)) {
        MLV_free_image(sheet);
        free_snake_sprites(sprites);
        return 0;
    }
    
    /* Queue à l'offset 160 */
    if (!init_part_sprites(&sprites->tail, sheet, SPRITE_TAIL_Y)) {
        MLV_free_image(sheet);
        free_snake_sprites(sprites);
        return 0;
    }
    
    /* Libérer la sprite sheet originale, on n'en a plus besoin */
    MLV_free_image(sheet);
    
    sprites->is_loaded = 1;
    sprites->current_size = SPRITE_SIZE;
    return 1;
}

/* Redimensionne les sprites d'une partie */
static void resize_part_sprites(PartSprites *part, int new_size) {
    if (part->up != NULL) {
        MLV_resize_image(part->up, new_size, new_size);
    }
    if (part->down != NULL) {
        MLV_resize_image(part->down, new_size, new_size);
    }
    if (part->left != NULL) {
        MLV_resize_image(part->left, new_size, new_size);
    }
    if (part->right != NULL) {
        MLV_resize_image(part->right, new_size, new_size);
    }
}

/* Redimensionne tous les sprites à la taille des cases */
void resize_snake_sprites(SnakeSprites *sprites, int cell_size) {
    if (!sprites->is_loaded) {
        return;
    }
    
    /* Ne redimensionner que si la taille a changé */
    if (sprites->current_size == cell_size) {
        return;
    }
    
    resize_part_sprites(&sprites->head, cell_size);
    resize_part_sprites(&sprites->body, cell_size);
    resize_part_sprites(&sprites->tail, cell_size);
    
    sprites->current_size = cell_size;
}

/* Libère la mémoire des sprites */
void free_snake_sprites(SnakeSprites *sprites) {
    free_part_sprites(&sprites->head);
    free_part_sprites(&sprites->body);
    free_part_sprites(&sprites->tail);
    sprites->is_loaded = 0;
    sprites->current_size = 0;
}

/* Initialise l'animation du serpent */
void init_snake_animation(SnakeAnimation *anim, int fps, int game_speed) {
    anim->animation_progress = 0.0f;
    /* 
     * frames_per_cell = nombre de frames d'affichage pour traverser une cellule
     * La formule utilisée dans le main est (31 - speed) frames entre chaque mouvement
     */
    (void)fps; /* Non utilisé avec la formule actuelle */
    anim->frames_per_cell = 31 - game_speed;
    if (anim->frames_per_cell < 1) {
        anim->frames_per_cell = 1;
    }
    anim->current_frame = 0;
}

/* Met à jour l'animation (appelé à chaque frame d'affichage) */
void update_snake_animation(SnakeAnimation *anim) {
    anim->current_frame++;
    
    /* Calcule la progression de 0.0 à 1.0 */
    anim->animation_progress = (float)anim->current_frame / (float)anim->frames_per_cell;
    
    if (anim->animation_progress > 1.0f) {
        anim->animation_progress = 1.0f;
    }
}

/* Réinitialise l'animation après un mouvement logique */
void reset_snake_animation(SnakeAnimation *anim) {
    anim->current_frame = 0;
    anim->animation_progress = 0.0f;
}

/* 
 * Calcule le sprite Y offset en fonction de la partie et de l'animation
 * Pour le corps, utilise animation_offset pour "glisser" sur les 3 corps identiques
 */
int get_sprite_y_offset(SnakePartType part_type, int animation_offset) {
    switch (part_type) {
        case SNAKE_PART_HEAD:
            return SPRITE_HEAD_Y;
            
        case SNAKE_PART_BODY:
            /* 
             * Le corps occupe 3 sprites identiques (64-160)
             * On utilise animation_offset pour "glisser" sur ces sprites
             * animation_offset va de 0 à SPRITE_SIZE-1
             * On commence à SPRITE_BODY_1_Y + animation_offset pour avoir l'effet de défilement
             */
            return SPRITE_BODY_1_Y + animation_offset;
            
        case SNAKE_PART_TAIL:
            return SPRITE_TAIL_Y;
            
        default:
            return SPRITE_HEAD_Y;
    }
}

/* Retourne l'image correspondant à la partie et la direction */
static MLV_Image* get_sprite_for_direction(PartSprites *part, Direction direction) {
    switch (direction) {
        case DIR_UP:
            return part->up;
        case DIR_DOWN:
            return part->down;
        case DIR_LEFT:
            return part->left;
        case DIR_RIGHT:
            return part->right;
        default:
            return part->up;
    }
}

/* 
 * Calcule le décalage visuel en pixels selon la direction
 * Le décalage va de -cell_size/2 (début) à 0 (fin) selon la progression
 * Cela crée l'effet où le serpent est visuellement "à moitié" dans sa case
 */
static void get_visual_offset(
    Direction direction, 
    float animation_progress, 
    int cell_size,
    int *offset_x, 
    int *offset_y
) {
    int half_cell;
    float visual_progress;
    
    half_cell = cell_size / 2;
    
    /* 
     * visual_progress va de -1 (début) à 0 (fin)
     * Cela décale le serpent en arrière de moitié au début du mouvement
     */
    visual_progress = animation_progress - 1.0f;
    
    *offset_x = 0;
    *offset_y = 0;
    
    switch (direction) {
        case DIR_UP:
            /* Vers le haut : décalage Y positif (en bas = en arrière) */
            *offset_y = (int)(visual_progress * -half_cell);
            break;
        case DIR_DOWN:
            /* Vers le bas : décalage Y négatif (en haut = en arrière) */
            *offset_y = (int)(visual_progress * half_cell);
            break;
        case DIR_LEFT:
            /* Vers la gauche : décalage X positif (à droite = en arrière) */
            *offset_x = (int)(visual_progress * -half_cell);
            break;
        case DIR_RIGHT:
            /* Vers la droite : décalage X négatif (à gauche = en arrière) */
            *offset_x = (int)(visual_progress * half_cell);
            break;
        default:
            break;
    }
}

/* 
 * Dessine un segment du serpent à une position donnée
 * La position grid_pos est la position logique sur la grille
 * Le sprite prend toute la case et est orienté selon la direction
 * Le serpent est visuellement décalé en arrière de moitié d'une case
 */
void draw_snake_part(
    SnakeSprites *sprites,
    SnakePartType part_type,
    Position *grid_pos,
    Direction direction,
    float animation_progress,
    int cell_size,
    int grid_offset_x,
    int grid_offset_y
) {
    int screen_x, screen_y;
    int offset_x, offset_y;
    PartSprites *part;
    MLV_Image *sprite_to_draw;
    
    if (!sprites->is_loaded) {
        return;
    }
    
    /* Redimensionner les sprites si nécessaire */
    resize_snake_sprites(sprites, cell_size);
    
    /* Sélectionner la partie (tête, corps ou queue) */
    switch (part_type) {
        case SNAKE_PART_HEAD:
            part = &sprites->head;
            break;
        case SNAKE_PART_BODY:
            part = &sprites->body;
            break;
        case SNAKE_PART_TAIL:
            part = &sprites->tail;
            break;
        default:
            part = &sprites->body;
            break;
    }
    
    /* Obtenir le sprite avec la bonne rotation */
    sprite_to_draw = get_sprite_for_direction(part, direction);
    if (sprite_to_draw == NULL) {
        return;
    }
    
    /* Position de base sur l'écran avec l'offset de la grille */
    screen_x = grid_offset_x + grid_pos->x * cell_size;
    screen_y = grid_offset_y + grid_pos->y * cell_size;
    
    /* Calculer le décalage visuel pour l'animation fluide */
    get_visual_offset(direction, animation_progress, cell_size, &offset_x, &offset_y);
    screen_x += offset_x;
    screen_y += offset_y;
    
    /* Dessiner le sprite (prend toute la case) */
    MLV_draw_image(sprite_to_draw, screen_x, screen_y);
}

/* 
 * Dessine le serpent complet avec animation fluide
 * Chaque segment est décalé visuellement selon sa direction et la progression de l'animation
 */
void draw_snake_animated(
    SnakeSprites *sprites,
    Snake *snake,
    SnakeAnimation *anim,
    int cell_size,
    int grid_offset_x,
    int grid_offset_y
) {
    int i;
    int segment_index;
    SnakePartType part_type;
    Position pos;
    Direction dir;
    
    if (!sprites->is_loaded) {
        return;
    }
    
    /* Parcours du serpent de la queue à la tête */
    for (i = 0; i < snake->length; i++) {
        /* Calcul de l'index réel dans le buffer circulaire */
        segment_index = (snake->tail_index + i) % MAX_SNAKE_LENGTH;
        
        pos = snake->segments[segment_index].position;
        dir = snake->segments[segment_index].direction;
        
        /* Détermination du type de partie */
        if (i == 0) {
            part_type = SNAKE_PART_TAIL;
        } else if (i == snake->length - 1) {
            part_type = SNAKE_PART_HEAD;
        } else {
            part_type = SNAKE_PART_BODY;
        }
        
        draw_snake_part(
            sprites,
            part_type,
            &pos,
            dir,
            anim->animation_progress,
            cell_size,
            grid_offset_x,
            grid_offset_y
        );
    }
}

/* 
 * Fallback: dessine le serpent en rectangles verts si les sprites ne sont pas chargés
 */
void draw_snake_fallback(
    Snake *snake,
    CellSize cell_size,
    GridOffset grid_offset
) {
    int i;
    int segment_index;
    int screen_x, screen_y;
    Position pos;
    MLV_Color color;
    
    /* Parcours du serpent de la queue à la tête */
    for (i = 0; i < snake->length; i++) {
        segment_index = (snake->tail_index + i) % MAX_SNAKE_LENGTH;
        pos = snake->segments[segment_index].position;
        
        screen_x = grid_offset.x + pos.x * cell_size.width;
        screen_y = grid_offset.y + pos.y * cell_size.height;
        
        /* Couleur différente pour la tête */
        if (i == snake->length - 1) {
            color = MLV_rgba(0, 200, 0, 255); /* Tête: vert foncé */
        } else {
            color = MLV_rgba(0, 255, 0, 255); /* Corps: vert clair */
        }
        
        MLV_draw_filled_rectangle(
            screen_x + 2, 
            screen_y + 2, 
            cell_size.width - 4, 
            cell_size.height - 4, 
            color
        );
    }
}

