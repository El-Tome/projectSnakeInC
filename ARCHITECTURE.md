# Architecture du projet Snake (SANS POINTEURS)

## 🎯 Contrainte principale
**PAS DE POINTEURS** sauf pour le passage par référence (`&` en C)
- ❌ Pas de `malloc`, `free`, `new`, `delete`
- ❌ Pas de liste chaînée avec `struct->next`
- ❌ Pas de tableaux dynamiques
- ✅ Tableaux statiques avec taille maximale définie
- ✅ Passage par référence avec `&` pour modifier structures

---

## 📊 Structure des données principales

### Serpent (sans liste chaînée)

**Problématique :** Comment stocker un serpent sans liste chaînée ?

**Solution :** Utiliser un **tableau circulaire** (circular buffer)

```c
#define MAX_SNAKE_LENGTH 500

typedef struct {
    Position segments[MAX_SNAKE_LENGTH];  /* Tableau fixe de 500 positions */
    int head_index;                       /* Index de la tête dans le tableau */
    int tail_index;                       /* Index de la queue dans le tableau */
    int length;                           /* Longueur actuelle du serpent */
    Direction current_direction;
    Direction next_direction;
    int is_alive;
} Snake;
```

**Principe de fonctionnement :**
```
Exemple avec MAX_SNAKE_LENGTH = 10 et un serpent de longueur 4 :

Index:     [0] [1] [2] [3] [4] [5] [6] [7] [8] [9]
           ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
Segments:  │   │   │ Q │ B │ B │ T │   │   │   │   │
           └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
                   ↑           ↑
              tail_index    head_index
              (queue)         (tête)

Q = Queue, B = Body, T = Tête

Pour déplacer le serpent :
1. Calculer nouvelle position tête selon direction
2. head_index = (head_index + 1) % MAX_SNAKE_LENGTH
3. Placer nouvelle tête à segments[head_index]
4. Si pas de croissance : tail_index = (tail_index + 1) % MAX_SNAKE_LENGTH
```

**Avantages :**
- Pas d'allocation dynamique
- Taille fixe connue à la compilation
- Accès rapide à tous les segments
- Facile à sauvegarder (copie directe de la structure)

---

### Grille (tableau 2D statique)

```c
#define GRID_WIDTH 40
#define GRID_HEIGHT 30

typedef struct {
    CellType cells[GRID_HEIGHT][GRID_WIDTH];  /* Tableau 2D fixe : 30 lignes × 40 colonnes */
    int width;                                  /* = 40 */
    int height;                                 /* = 30 */
    int has_borders;                            /* 0 ou 1 */
} Grid;
```

**Accès :** `grid.cells[y][x]` où `y` ∈ [0, 29] et `x` ∈ [0, 39]

**Mémoire utilisée :** 40 × 30 × sizeof(CellType) = 1200 bytes (si CellType = 1 byte)

---

### Scores (top 10)

```c
#define MAX_SCORES 10
#define MAX_NAME_LENGTH 50

typedef struct {
    ScoreEntry scores[MAX_SCORES];  /* Tableau fixe de 10 scores */
    int count;                       /* Nombre de scores valides (0-10) */
} HighScores;
```

**Principe :**
- Tableau toujours trié (décroissant)
- Lorsqu'on ajoute un score, on insère au bon endroit
- Si count = 10, on remplace le 10ème si nouveau score est meilleur

---

### Sauvegardes (4 slots)

```c
#define MAX_SAVE_SLOTS 4

typedef struct {
    Game game_state;    /* État du jeu */
    Snake snake;        /* Serpent complet (tableau copié) */
    Food food;          /* Position nourriture */
    Grid grid;          /* Grille complète (tableau copié) */
    time_t save_time;   /* Horodatage */
} GameSave;
```

**Sauvegarde binaire simple :**
```c
void save_game(GameSave save, int slot_number) {
    char filepath[100];
    snprintf(filepath, 100, "./ressources/saves/slot_%d.bin", slot_number);
    
    FILE *file = fopen(filepath, "wb");
    if (file) {
        fwrite(&save, sizeof(GameSave), 1, file);
        fclose(file);
    }
}
```

**Avantage :** Toute la structure est copiée d'un bloc (pas de pointeurs à suivre)

---

## 🔄 Passage de paramètres

### Règle générale

1. **Modification nécessaire** → Passage par référence
   ```c
   void init_snake(Snake *snake, int x, int y, int length);
   void move_snake(Snake *snake);
   ```

2. **Lecture seule** → Passage par valeur (si petite structure) ou référence (si grosse)
   ```c
   int check_self_collision(Snake snake);           /* Petit : par valeur */
   void render_grid(Grid grid);                     /* Gros : préférer Grid *grid */
   Position get_head_position(Snake snake);
   ```

3. **Tableaux** → Toujours par référence (comportement par défaut en C)
   ```c
   void get_save_filepath(char filepath[], int slot_number);
   ```

---

## 📂 Organisation des fichiers

```
projetSnake/
├── Makefile
├── README.md
├── ARCHITECTURE.md
├── snake (exécutable)
│
├── src/              Fichiers .c
│   ├── main.c
│   ├── game_state.c
│   ├── snake.c
│   ├── grid.c
│   ├── food.c
│   ├── controls.c
│   ├── collision.c
│   ├── menu.c
│   ├── render.c
│   ├── scores.c
│   ├── save.c
│   ├── timer.c
│   ├── utils.c
│   ├── bonus.c       (optionnel)
│   ├── level.c       (optionnel)
│   └── multiplayer.c (optionnel)
│
├── include/          Fichiers .h (mêmes noms)
│   ├── game_state.h
│   ├── snake.h
│   ├── ...
│
├── lib/              (si MLV pas installé système)
│
└── ressources/
    ├── images/
    ├── saves/
    │   ├── slot_0.bin
    │   ├── slot_1.bin
    │   ├── slot_2.bin
    │   └── slot_3.bin
    └── scores/
        └── highscores.txt
```

---

## 🔧 Exemple de Makefile

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
LDFLAGS = -lMLV -lm
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# Liste des fichiers sources
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
EXECUTABLE = snake

# Règle par défaut (silencieuse)
all: $(EXECUTABLE)
	@echo "Compilation terminée : ./$(EXECUTABLE)"

# Création de l'exécutable
$(EXECUTABLE): $(OBJECTS)
	@$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compilation des .o à partir des .c
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

# Création du dossier obj si nécessaire
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Nettoyage
clean:
	@rm -rf $(OBJ_DIR) $(EXECUTABLE)
	@echo "Nettoyage effectué"

.PHONY: all clean
```

---

## ⚡ Boucle de jeu (structure imposée)

```c
void game_loop(Game *game) {
    struct timespec debut, fin;
    long elapsed_ns;
    const long FRAME_TIME_NS = 1000000000 / FPS;  /* 60 FPS = 16.67ms */
    int quitter = 0;
    
    Snake snake;
    Grid grid;
    Food food;
    
    init_snake(&snake, GRID_WIDTH/2, GRID_HEIGHT/2, 3);
    init_grid(&grid, 0);  /* 0 = pas de bordures */
    spawn_food(&grid, snake, &food);
    
    while (!quitter) {
        /* 1. Récupération du temps au début de l'image */
        clock_gettime(CLOCK_REALTIME, &debut);
        
        /* 2. Affichage de l'image courante */
        clear_screen();
        render_game(grid, snake, food, game->score);
        
        /* 3. Récupération d'UN SEUL événement clavier par image */
        MLV_Keyboard_button key;
        if (MLV_get_event(&key, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL) == MLV_KEY) {
            /* 4. Résolution des événements */
            handle_input(game, &snake, key);
        }
        
        /* 5. Déplacement des objets (logique du jeu) */
        if (game->current_state == STATE_PLAYING) {
            move_snake(&snake);
            update_grid_with_snake(&grid, snake);
        }
        
        /* 6. Résolution des collisions */
        resolve_collisions(game, &grid, &snake, &food);
        
        /* 7. Actualisation de l'affichage */
        update_display();
        
        /* 8. Récupération du temps à la fin de l'image */
        clock_gettime(CLOCK_REALTIME, &fin);
        
        /* 9. Calcul du temps passé sur l'image courante */
        elapsed_ns = (fin.tv_sec - debut.tv_sec) * 1000000000L + 
                     (fin.tv_nsec - debut.tv_nsec);
        
        /* 10. Attente si l'image a été trop rapide */
        if (elapsed_ns < FRAME_TIME_NS) {
            struct timespec sleep_time;
            long remaining_ns = FRAME_TIME_NS - elapsed_ns;
            sleep_time.tv_sec = 0;
            sleep_time.tv_nsec = remaining_ns;
            nanosleep(&sleep_time, NULL);
        }
        
        quitter = (game->current_state == STATE_MENU && key == MLV_KEYBOARD_ESCAPE);
    }
}
```

---

## 💡 Astuces pour éviter les pointeurs

### ❌ AVANT (avec pointeurs)
```c
Snake* snake = malloc(sizeof(Snake));
init_snake(snake);
move_snake(snake);
free(snake);
```

### ✅ APRÈS (sans pointeurs)
```c
Snake snake;  /* Allocation sur la pile (stack) */
init_snake(&snake, 10, 10, 3);  /* Passage par référence */
move_snake(&snake);
/* Pas de free nécessaire, libération automatique */
```

---

### ❌ AVANT (liste chaînée)
```c
typedef struct Segment {
    Position pos;
    struct Segment *next;  /* Pointeur vers segment suivant */
} Segment;

Segment *head = malloc(sizeof(Segment));
head->next = malloc(sizeof(Segment));
/* ... */
```

### ✅ APRÈS (tableau circulaire)
```c
typedef struct {
    Position segments[MAX_SNAKE_LENGTH];
    int head_index;
    int tail_index;
    int length;
} Snake;

Snake snake;
snake.segments[0] = (Position){10, 10};
snake.head_index = 0;
snake.tail_index = 0;
snake.length = 1;
```

---

### ❌ AVANT (chaîne dynamique)
```c
char *filepath = malloc(100);
sprintf(filepath, "./saves/slot_%d.bin", slot);
free(filepath);
```

### ✅ APRÈS (chaîne statique)
```c
char filepath[100];
snprintf(filepath, 100, "./saves/slot_%d.bin", slot);
/* Pas de free, tableau local */
```

---

## 🎮 Gestion des collisions

```c
void resolve_collisions(Game *game, Grid *grid, Snake *snake, Food *food) {
    Position head = get_head_position(*snake);
    
    /* 1. Collision avec nourriture */
    if (check_food_collision(*snake, *food)) {
        game->score += food->value;
        snake->length++;
        spawn_food(grid, *snake, food);
    }
    
    /* 2. Collision avec soi-même */
    if (check_self_collision(*snake)) {
        snake->is_alive = 0;
        game->current_state = STATE_GAME_OVER;
    }
    
    /* 3. Téléportation sur les bords (si pas de murs) */
    if (!grid->has_borders) {
        Position wrapped = wrap_position(*grid, head);
        if (wrapped.x != head.x || wrapped.y != head.y) {
            /* Téléportation effectuée */
        }
    }
    
    /* 4. Collision avec murs (si bordures activées) */
    if (grid->has_borders && check_wall_collision(*grid, head)) {
        snake->is_alive = 0;
        game->current_state = STATE_GAME_OVER;
    }
}
```

---

## 📝 Checklist avant compilation

- [ ] Aucun `malloc`, `calloc`, `realloc` dans le code
- [ ] Aucun `free` dans le code
- [ ] Tous les tableaux ont une taille fixe définie par `#define`
- [ ] Tous les pointeurs sont utilisés uniquement pour passage par référence
- [ ] Les chaînes utilisent `char str[MAX]` et pas `char *str`
- [ ] Compilation avec `-Wall -Wextra` sans warnings
- [ ] `make` compile silencieusement (pas d'output sauf erreurs)
- [ ] Exécutable `./snake` créé à la racine

---

## 🚀 Bon courage !

Cette architecture permet de respecter **toutes les contraintes** du projet :
- ✅ Pas de pointeurs (sauf `&` pour passage par référence)
- ✅ Code modulaire et structuré
- ✅ Facile à sauvegarder (structures en un bloc)
- ✅ Mémoire prévisible (pas d'allocation dynamique)
- ✅ Compatible avec la bibliothèque MLV

