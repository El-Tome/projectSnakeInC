#include "snake_display.h"
#include "grid_display.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Chemins des dossiers */
#define STRAIGHT_DIR "ressources/snake/straight"
#define RIGHT_DIR "ressources/snake/right"
#define LEFT_DIR "ressources/snake/left"

/* Initialise une partie vide des sprites */
void init_directional(DirectionalSprites *part) {
    int i;
    part->up.frame_count    = 0;
    part->right.frame_count = 0;
    part->down.frame_count  = 0;
    part->left.frame_count  = 0;
    for (i = 0; i < MAX_FRAMES; i++) {
        part->up.frames[i]    = NULL;
        part->right.frames[i] = NULL;
        part->down.frames[i]  = NULL;
        part->left.frames[i]  = NULL;
    }
}
/* Initialise une partie vide des coins */
void init_corner(CornerSprites *corner) {
    init_directional(&corner->turn_right);
    init_directional(&corner->turn_left);
}
void init_all_empty_sprites(SnakeSprites *sprites) {
    init_directional(&sprites->head);
    init_directional(&sprites->body_head);
    init_directional(&sprites->body);
    init_directional(&sprites->tail);
    init_corner(&sprites->head_corner);
    init_corner(&sprites->body_corner);
    init_corner(&sprites->tail_corner);
}

/* Charge une image et crée les 4 rotations (base orientée vers le haut) */
int load_rotations_up(DirectionalSprites *part, const char *path, int frame_idx) {
    MLV_Image *base = NULL;

    base = MLV_load_image(path);
    if (base == NULL) {
        return 0; /* Si l'image n'est pas chargée, on retourne 0 */
    }
    
    /* Rotation de 0° */
    part->up.frames[frame_idx]    = MLV_copy_image(base);
    /* Rotation de 90° */
    MLV_rotate_image(MLV_copy_image(base), 90.0);
    part->left.frames[frame_idx]  = base;
    /* Rotation de 180° */
    MLV_rotate_image(MLV_copy_image(base), 180.0);
    part->down.frames[frame_idx]  = base;
    /* Rotation de 270° */
    MLV_rotate_image(MLV_copy_image(base), 270.0);
    part->right.frames[frame_idx] = base;
    
    /* Libération de l'image de base */
    MLV_free_image(base);
    return 1;
}
void set_frame_count(DirectionalSprites *part, int count) {
    part->up.frame_count    = count;
    part->right.frame_count = count;
    part->down.frame_count  = count;
    part->left.frame_count  = count;
}
void load_sprites(SnakeSprites *sprites, char *dir) {
    int i, loaded;
    char path[256];
    
    /* Têtes */
    loaded = 0;
    for (i = 0; i < 4; i++) {
        sprintf(path, "%s/%d_Head.png", dir, i + 1);
        if (load_rotations_up(&sprites->head, path, i)) {
            loaded++;
        }
    }
    set_frame_count(&sprites->head, loaded);
    
    /* Body_Head */
    if (strcmp(dir, STRAIGHT_DIR) == 0) {
        loaded = 0;
        for (i = 0; i < 4; i++) {
            sprintf(path, "%s/%d_Body_Head.png", dir, i + 5);
            if (load_rotations_up(&sprites->body_head, path, i)) {
                loaded++;
            }
        }
        set_frame_count(&sprites->body_head, loaded);
    }
    /* Corps */
    loaded = 0;
    for (i = 0; i < 3; i++) {
        sprintf(path, "%s/%d_Body.png", dir, i + 9);
        if (load_rotations_up(&sprites->body, path, i)) {
            loaded++;
        }
    }
    set_frame_count(&sprites->body, loaded);
    
    /* Queue */
    loaded = 0;
    for (i = 0; i < 4; i++) {
        sprintf(path, "%s/%d_Tail.png", dir, i + 13);
        if (load_rotations_up(&sprites->tail, path, i)) {
            loaded++;
        }
    }
    set_frame_count(&sprites->tail, loaded);
}

/* Charge les sprites du serpent */
int init_snake_sprites(SnakeSprites *sprites) {
    
    /* Initialiser toutes les structures des sprites à vide*/
    init_all_empty_sprites(sprites);

    /* Initialiser le statut des sprites à non chargés + la taille des sprites à la taille de base*/
    sprites->is_loaded = 0;
    sprites->current_size = SPRITE_SIZE;
    

    /* LIGNES DROITES */
    load_sprites(sprites, STRAIGHT_DIR);
    
    /* COINS DROITE */
    load_sprites(sprites, RIGHT_DIR);

    /* COINS GAUCHE */
    load_sprites(sprites, LEFT_DIR);
    
    /* Vérifie si au moins un sprite est chargé */
    if (
        /* Straight */
        sprites->head.up.frame_count > 0      &&
        sprites->body_head.up.frame_count > 0 &&
        sprites->body.up.frame_count > 0      &&
        sprites->tail.up.frame_count > 0      &&
        /* Right */
        sprites->head_corner.turn_right.up.frame_count > 0 &&
        sprites->body_corner.turn_right.up.frame_count > 0 &&
        sprites->tail_corner.turn_right.up.frame_count > 0 && 
        /* Left */
        sprites->head_corner.turn_left.up.frame_count > 0 &&
        sprites->body_corner.turn_left.up.frame_count > 0 &&
        sprites->tail_corner.turn_left.up.frame_count > 0
    ) {
        sprites->is_loaded = 1;
        return 1;
    } else {
        return 0;
    }
}






/* Crée une copie pivotée d'une image */
MLV_Image* create_rotated(MLV_Image *original, double angle) {
    MLV_Image *copy;
    
    copy = MLV_copy_image(original);
    if (copy != NULL && angle != 0.0) {
        MLV_rotate_image(copy, angle);
    }
    return copy;
}

/* Charge une image sans rotation (pour les coins qui seront orientés manuellement) */
static int load_single_frame(AnimatedSprite *sprite, const char *path, int frame_idx) {
    MLV_Image *img = MLV_load_image(path);
    if (img == NULL) {
        return 0;
    }
    sprite->frames[frame_idx] = img;
    return 1;
}



/* Libère une AnimatedSprite */
static void free_animated(AnimatedSprite *sprite) {
    int i;
    for (i = 0; i < MAX_FRAMES; i++) {
        if (sprite->frames[i] != NULL) {
            MLV_free_image(sprite->frames[i]);
            sprite->frames[i] = NULL;
        }
    }
    sprite->frame_count = 0;
}

static void free_directional(DirectionalSprites *part) {
    free_animated(&part->up);
    free_animated(&part->right);
    free_animated(&part->down);
    free_animated(&part->left);
}

static void free_corner(CornerSprites *corner) {
    free_directional(&corner->turn_right);
    free_directional(&corner->turn_left);
}

/* Redimensionne */
static void resize_animated(AnimatedSprite *sprite, int size) {
    int i;
    for (i = 0; i < sprite->frame_count; i++) {
        if (sprite->frames[i] != NULL) {
            MLV_resize_image(sprite->frames[i], size, size);
        }
    }
}

static void resize_directional(DirectionalSprites *part, int size) {
    resize_animated(&part->up, size);
    resize_animated(&part->right, size);
    resize_animated(&part->down, size);
    resize_animated(&part->left, size);
}

static void resize_corner(CornerSprites *corner, int size) {
    resize_directional(&corner->turn_right, size);
    resize_directional(&corner->turn_left, size);
}

/* 
 * Crée les 4 rotations pour les coins à partir de l'image de base
 * L'image de base montre un coin où on entre par le BAS (prev_dir = DIR_UP)
 */
static void create_corner_rotations(DirectionalSprites *part, int frame_idx) {
    MLV_Image *base;
    
    /* L'image dans up.frames est la base (entrée par le bas) */
    base = part->up.frames[frame_idx];
    if (base == NULL) {
        return;
    }
    
    /* prev_dir = DIR_RIGHT : entrée par la gauche → rotation 270° (90° horaire) */
    part->right.frames[frame_idx] = create_rotated(base, 270.0);
    /* prev_dir = DIR_DOWN : entrée par le haut → rotation 180° */
    part->down.frames[frame_idx] = create_rotated(base, 180.0);
    /* prev_dir = DIR_LEFT : entrée par la droite → rotation 90° (90° anti-horaire) */
    part->left.frames[frame_idx] = create_rotated(base, 90.0);
}



void free_snake_sprites(SnakeSprites *sprites) {
    free_directional(&sprites->head);
    free_directional(&sprites->body_head);
    free_directional(&sprites->body);
    free_directional(&sprites->tail);
    free_corner(&sprites->head_corner);
    free_corner(&sprites->body_corner);
    free_corner(&sprites->tail_corner);
    sprites->is_loaded = 0;
}

void resize_snake_sprites(SnakeSprites *sprites, int cell_size) {
    if (!sprites->is_loaded || sprites->current_size == cell_size) {
        return;
    }
    
    resize_directional(&sprites->head, cell_size);
    resize_directional(&sprites->body_head, cell_size);
    resize_directional(&sprites->body, cell_size);
    resize_directional(&sprites->tail, cell_size);
    resize_corner(&sprites->head_corner, cell_size);
    resize_corner(&sprites->body_corner, cell_size);
    resize_corner(&sprites->tail_corner, cell_size);
    sprites->current_size = cell_size;
}

void init_snake_animation(SnakeAnimation *anim, int frame_delay) {
    anim->current_frame = 0;
    anim->frame_delay = frame_delay;
    anim->tick_counter = 0;
}

void update_snake_animation(SnakeAnimation *anim) {
    anim->tick_counter++;
    if (anim->tick_counter >= anim->frame_delay) {
        anim->tick_counter = 0;
        anim->current_frame++;
    }
}

void reset_snake_animation(SnakeAnimation *anim) {
    anim->current_frame = 0;
    anim->tick_counter = 0;
}

/* Récupère le sprite pour une direction (lignes droites) */
static AnimatedSprite get_sprite_dir(DirectionalSprites part, Direction dir) {
    switch (dir) {
        case DIR_UP:    return part.up;
        case DIR_RIGHT: return part.right;
        case DIR_DOWN:  return part.down;
        case DIR_LEFT:  return part.left;
        default:        return part.up;
    }
}

/* 
 * Pour les coins, la rotation dépend de la direction d'où VIENT le serpent
 * Image de base : entrée par le BAS (prev_dir = DIR_UP = serpent va vers le haut)
 */
static AnimatedSprite get_corner_sprite(DirectionalSprites part, Direction prev_dir) {
    switch (prev_dir) {
        case DIR_UP:    return part.up;     /* Entrée par le bas = image de base */
        case DIR_RIGHT: return part.right;  /* Entrée par la gauche = rotation 270° */
        case DIR_DOWN:  return part.down;   /* Entrée par le haut = rotation 180° */
        case DIR_LEFT:  return part.left;   /* Entrée par la droite = rotation 90° */
        default:        return part.up;
    }
}

/* 
 * Détermine le type de coin entre deux directions
 * Retourne CORNER_RIGHT si on tourne à droite, CORNER_LEFT si à gauche, CORNER_NONE sinon
 */
static CornerType calc_corner(Direction from_dir, Direction to_dir) {
    if (from_dir == to_dir || from_dir == DIR_NONE || to_dir == DIR_NONE) {
        return CORNER_NONE;
    }
    
    /* Coin à droite (sens horaire) */
    if ((from_dir == DIR_UP && to_dir == DIR_RIGHT) ||
        (from_dir == DIR_RIGHT && to_dir == DIR_DOWN) ||
        (from_dir == DIR_DOWN && to_dir == DIR_LEFT) ||
        (from_dir == DIR_LEFT && to_dir == DIR_UP)) {
        return CORNER_RIGHT;
    }
    
    /* Coin à gauche (sens anti-horaire) */
    return CORNER_LEFT;
}

/* Dessine un segment */
static void draw_segment(SnakeSprites sprites, Snake snake, int seg_idx, 
                         SnakePartType type, int anim_frame, int cell_size, int ox, int oy) {
    Segment seg;
    AnimatedSprite anim_sprite;
    DirectionalSprites part;
    MLV_Image *img;
    int frame_idx, x, y;
    int prev_seg_idx, next_seg_idx;
    Direction prev_dir, next_dir;
    CornerType corner;
    
    seg = snake.segments[seg_idx];
    
    /* Trouver les directions des segments adjacents */
    prev_seg_idx = (seg_idx - 1 + MAX_SNAKE_LENGTH) % MAX_SNAKE_LENGTH;
    next_seg_idx = (seg_idx + 1) % MAX_SNAKE_LENGTH;
    prev_dir = snake.segments[prev_seg_idx].direction;
    next_dir = snake.segments[next_seg_idx].direction;
    
    /* Sélectionner les sprites selon le type */
    switch (type) {
        case SNAKE_PART_HEAD:
            /* La tête tourne si sa direction diffère du segment body_head */
            corner = calc_corner(prev_dir, seg.direction);
            if (corner == CORNER_RIGHT && sprites.head_corner.turn_right.up.frame_count > 0) {
                anim_sprite = get_corner_sprite(sprites.head_corner.turn_right, prev_dir);
            } else if (corner == CORNER_LEFT && sprites.head_corner.turn_left.up.frame_count > 0) {
                anim_sprite = get_corner_sprite(sprites.head_corner.turn_left, prev_dir);
            } else {
                anim_sprite = get_sprite_dir(sprites.head, seg.direction);
            }
            break;
            
        case SNAKE_PART_TAIL:
            /* La queue tourne si sa direction diffère du segment suivant */
            corner = calc_corner(seg.direction, next_dir);
            if (corner == CORNER_RIGHT && sprites.tail_corner.turn_right.up.frame_count > 0) {
                anim_sprite = get_corner_sprite(sprites.tail_corner.turn_right, seg.direction);
            } else if (corner == CORNER_LEFT && sprites.tail_corner.turn_left.up.frame_count > 0) {
                anim_sprite = get_corner_sprite(sprites.tail_corner.turn_left, seg.direction);
            } else {
                anim_sprite = get_sprite_dir(sprites.tail, seg.direction);
            }
            break;
            
        case SNAKE_PART_BODY:
        default:
            if (seg.corner == CORNER_RIGHT) {
                anim_sprite = get_corner_sprite(sprites.body_corner.turn_right, prev_dir);
            } else if (seg.corner == CORNER_LEFT) {
                anim_sprite = get_corner_sprite(sprites.body_corner.turn_left, prev_dir);
            } else {
                /* Segment juste après la tête = body_head */
                int pos_from_tail = (seg_idx - snake.tail_index + MAX_SNAKE_LENGTH) % MAX_SNAKE_LENGTH;
                if (pos_from_tail == snake.length - 2 && sprites.body_head.up.frame_count > 0) {
                    part = sprites.body_head;
                } else {
                    part = sprites.body;
                }
                anim_sprite = get_sprite_dir(part, seg.direction);
            }
            break;
    }
    
    if (anim_sprite.frame_count == 0) {
        return;
    }
    
    /* Calculer l'index de frame */
    frame_idx = anim_frame % anim_sprite.frame_count;
    img = anim_sprite.frames[frame_idx];
    
    if (img == NULL) {
        return;
    }
    
    /* Position sur l'écran */
    x = ox + seg.position.x * cell_size;
    y = oy + seg.position.y * cell_size;
    
    MLV_draw_image(img, x, y);
}

void draw_snake(SnakeSprites sprites, Snake snake, SnakeAnimation anim,
                int cell_size, int grid_ox, int grid_oy) {
    int i, idx;
    SnakePartType type;
    
    if (!sprites.is_loaded) {
        return;
    }
    
    /* Redimensionner si nécessaire */
    resize_snake_sprites(&sprites, cell_size);
    
    /* Dessiner de la queue à la tête */
    for (i = 0; i < snake.length; i++) {
        idx = (snake.tail_index + i) % MAX_SNAKE_LENGTH;
        
        if (i == 0) {
            type = SNAKE_PART_TAIL;
        } else if (i == snake.length - 1) {
            type = SNAKE_PART_HEAD;
        } else {
            type = SNAKE_PART_BODY;
        }
        
        draw_segment(sprites, snake, idx, type, anim.current_frame, cell_size, grid_ox, grid_oy);
    }
}

void draw_snake_fallback(Snake *snake, CellSize cell_size, GridOffset offset) {
    int i, idx, x, y;
    Position pos;
    
    for (i = 0; i < snake->length; i++) {
        idx = (snake->tail_index + i) % MAX_SNAKE_LENGTH;
        pos = snake->segments[idx].position;
        
        x = offset.x + pos.x * cell_size.width;
        y = offset.y + pos.y * cell_size.height;
        
        MLV_draw_filled_rectangle(
            x + 2, y + 2,
            cell_size.width - 4, cell_size.height - 4,
            (i == snake->length - 1) ? MLV_rgba(0, 200, 0, 255) : MLV_rgba(0, 255, 0, 255)
        );
    }
}
