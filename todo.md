# TODO pour le projet snake en C

## 1. Structure du projet

```
projetSnake/
├── Makefile
├── README.md
├── snake (exécutable généré)
├── src/           (fichiers .c)
├── include/       (fichiers .h)
├── lib/           (MLV si besoin)
├── build/         (Fichier .o)
└── ressources/
    ├── images/
    ├── saves/
    └── scores/
```

---

## 2. Fichiers et fonctions à créer

### 📁 **main.c** / **main.h**
**Rôle :** Point d'entrée, orchestration du jeu

**Fonctions :**
- `int main(int argc, char *argv[])` - Point d'entrée principal
- `void init_game(Game *game)`       - Initialisation par référence
- `void cleanup_game(Game *game)`    - Nettoyage par référence
- `void game_loop(Game *game)`       - Boucle principale du jeu (voir structure imposée)

---

### 📁 **game_state.c** / **game_state.h**
**Rôle :** Gestion de l'état global du jeu

A check si on prend tout les types
**Structures :**
```c
typedef enum {
    STATE_MENU,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAME_OVER,
    STATE_LOAD_GAME,
    STATE_SAVE_GAME,
    STATE_SCORES
} GameState;

typedef struct {
    GameState current_state;
    int score;
    int level;
    int game_speed;
    time_t start_time;
    int is_running;
} Game;
```

**Fonctions :**
- `void init_game_state(Game *game)`
- `void set_game_state(Game *game, GameState state)`
- `GameState get_game_state(Game game)` - Passage par valeur pour lecture

---
### 📁 **grid.c** / **grid.h**
**Rôle :** Gestion de la grille de jeu

**Structures :**
```c
#define GRID_WIDTH 40   /* Largeur en cases */
#define GRID_HEIGHT 30  /* Hauteur en cases */

typedef enum {
    CELL_EMPTY,
    CELL_SNAKE,
    CELL_FOOD,
    CELL_WALL,
    CELL_BONUS
} CellType;

/* Tableau 2D statique */
typedef struct {
    CellType cells[GRID_HEIGHT][GRID_WIDTH];  /* Tableau statique */
    int width;                                  /* = GRID_WIDTH */
    int height;                                 /* = GRID_HEIGHT */
    int has_borders;                            /* 0 ou 1 */
} Grid;


typedef struct {
    int x;
    int y;
} Position;
```

**Fonctions :**
- `void init_grid(Grid *grid, int has_borders)`            - Initialise tableau
- `void clear_grid(Grid *grid)`                            - Remplit de CELL_EMPTY
- `void update_grid_with_snake(Grid *grid, Snake snake)`   - Marque cellules serpent
- `void update_grid_with_food(Grid *grid, Position food)`  - Marque cellule nourriture
- `CellType get_cell(Grid grid, Position p)`               - Lecture cellule
- `void set_cell(Grid *grid, Position p, CellType type)`   - Écriture cellule

---

### 📁 **snake.c** / **snake.h**
**Rôle :** Gestion du serpent

**Structures :**
```c
#define MAX_SNAKE_LENGTH 500  /* Taille max du serpent */

typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

/* Serpent = tableau de positions (comme une file circulaire) */
typedef struct {
    Position segments[MAX_SNAKE_LENGTH];  /* Tableau statique contenant toutes les positions du serpant */
    int head_index;                       /* Index de la tête */
    int tail_index;                       /* Index de la queue */
    int length;                           /* Longueur actuelle */
    Direction direction;          /* Direction du déplacement en cours */
    int is_alive;
} Snake;
```

**Fonctions :**
- `void init_snake(Snake *snake, Position p, int initial_length)`
- `void move_snake(Snake *snake)`                                    - Déplace la tête, avance la queue
- `void grow_snake(Snake *snake)`                                    - N'avance pas la queue lors du prochain move
- `int check_self_collision(Snake snake)`                            - Vérifie si tête touche corps
- `void set_snake_direction(Snake *snake, Direction dir)`            - change la valeur de direction
- `int is_valid_direction_change(Direction current, Direction new)`  - check si la valeur de la prochaine direction est valide
- `Position get_head_position(Snake snake)`                          - Retourne position tête
- `Position get_segment_position(Snake snake, int index)`            - Position segment i

---


### 📁 **food.c** / **food.h**
**Rôle :** Gestion de la nourriture

**Structures :**
```c
typedef struct {
    Position pos;
    int value;
    int is_active;
} Food;
```

**Fonctions :**
- `void spawn_food(Grid *grid, Snake snake, Food *food)` - Génère position aléatoire
- `int check_food_collision(Snake snake, Food food)`     - Retourne 1 si collision 0 sinon
- `void disable_food(Food *food)`                        - Désactive la nourriture

---

### 📁 **controls.c** / **controls.h**
**Rôle :** Gestion des entrées clavier

Pas check si les fonctions sont cohérante
**Fonctions :**
- `void handle_input(Game *game, Snake *snake, MLV_Keyboard_button key)`               - Traite input jeu
- `void handle_menu_input(Game *game, MLV_Keyboard_button key, int *menu_selection)`   - Navigation menu
- `void handle_pause_input(Game *game, MLV_Keyboard_button key, int *pause_selection)` - Menu pause
- `Direction key_to_direction(MLV_Keyboard_button key)`                                - Convertit touche en direction
- `int is_direction_key(MLV_Keyboard_button key)`                                      - Vérifie si touche directionnelle

---

### 📁 **collision.c** / **collision.h**
**Rôle :** Détection des collisions

Probablement inutile
**Fonctions :**
- `int check_wall_collision(Grid grid, Position pos)` - Retourne 1 si collision mur
- `int check_snake_collision(Snake snake, Position pos)` - Retourne 1 si collision serpent
- `int check_food_collision_at(Position snake_pos, Position food_pos)` - Collision directe
- `void resolve_collisions(Game *game, Grid *grid, Snake *snake, Food *food)` - Gère toutes collisions

---

### 📁 **menu.c** / **menu.h**
**Rôle :** Menus du jeu

**Structures :**
```c
typedef enum {
    MENU_NEW_GAME,
    MENU_LOAD_GAME,
    MENU_SCORES,
    MENU_QUIT
} MenuOption;

typedef enum {
    PAUSE_RESUME,
    PAUSE_SAVE,
    PAUSE_MAIN_MENU,
    PAUSE_QUIT
} PauseOption;
```

**Fonctions :**
- `void display_main_menu(int selected_option)`
- `void display_pause_menu(int selected_option)`
- `MenuOption handle_main_menu(Game *game)`
- `PauseOption handle_pause_menu(Game *game)`
- `void display_game_over_screen(int score, int is_high_score)`

---

### 📁 **render.c** / **render.h**
**Rôle :** Affichage graphique (MLV)

**Constantes :**
```c
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define CELL_SIZE 20
#define FPS 60
#define FRAME_TIME_NS (1000000000 / FPS)
```

**Fonctions :**
- `void init_window()` - Crée fenêtre MLV
- `void close_window()` - Ferme fenêtre MLV
- `void render_game(Grid grid, Snake snake, Food food, int score)` - Affiche tout
- `void render_grid(Grid grid)` - Dessine grille
- `void render_snake(Snake snake)` - Dessine serpent
- `void render_food(Food food)` - Dessine nourriture
- `void clear_screen()` - Efface écran
- `void update_display()` - Wrapper pour MLV_actualize_window()

---

### 📁 **scores.c** / **scores.h**
**Rôle :** Gestion des scores

**Structures :**
```c
#define MAX_SCORES 10
#define MAX_NAME_LENGTH 50

typedef struct {
    char player_name[MAX_NAME_LENGTH];
    int score;
    int level;
    int length;
    time_t date;
    int duration_seconds;
} ScoreEntry;

typedef struct {
    ScoreEntry scores[MAX_SCORES];  /* Tableau statique */
    int count;                       /* Nombre de scores valides */
} HighScores;
```

**Fonctions :**
- `void load_scores(HighScores *scores, char filepath[])` - Charge depuis fichier
- `void save_scores(HighScores scores, char filepath[])` - Sauvegarde dans fichier
- `int add_score(HighScores *scores, ScoreEntry new_score)` - Ajoute et retourne rang
- `void sort_scores(HighScores *scores)` - Tri décroissant
- `void display_scores(HighScores scores)` - Affichage MLV
- `int calculate_score(int food_eaten, int length, time_t duration, int level)` - Formule
- `int is_high_score(HighScores scores, int score)` - Vérifie si top 10

---

### 📁 **save.c** / **save.h**
**Rôle :** Sauvegarde/chargement de parties

**Structures :**
```c
#define MAX_SAVE_SLOTS 4
#define SAVE_PATH_LENGTH 100

typedef struct {
    int slot_number;
    int is_used;
    time_t save_date;
    int score;
    int level;
} SaveSlot;

/* Structure complète d'une sauvegarde */
typedef struct {
    Game game_state;
    Snake snake;        /* Copie complète du serpent */
    Food food;
    Grid grid;          /* Copie complète de la grille */
    time_t save_time;
} GameSave;
```

**Fonctions :**
- `int save_game(GameSave save, int slot_number)` - Retourne 0 si succès
- `int load_game(GameSave *save, int slot_number)` - Retourne 0 si succès
- `void list_save_slots(SaveSlot slots[MAX_SAVE_SLOTS])` - Liste les 4 slots
- `void delete_save(int slot_number)` - Supprime sauvegarde
- `int is_slot_used(int slot_number)` - Vérifie si slot occupé
- `void display_load_menu(SaveSlot slots[MAX_SAVE_SLOTS], int selected)` - Menu chargement

---

### 📁 **timer.c** / **timer.h**
**Rôle :** Gestion du temps et FPS

**Structures :**
```c
typedef struct {
    struct timespec start;
    struct timespec end;
    long elapsed_ns;
} Timer;
```

**Fonctions :**
- `void timer_start(Timer *timer)`
- `void timer_end(Timer *timer)`
- `long timer_get_elapsed_ns(Timer *timer)`
- `void timer_wait_frame(Timer *timer, long target_frame_time_ns)`
- `double get_elapsed_seconds(time_t start_time)`

---

Je ne check pas cette parti
### 📁 **bonus.c** / **bonus.h** (FONCTIONNALITÉS BONUS)
**Rôle :** Objets spéciaux et bonus

**Structures :**
```c
#define MAX_ACTIVE_BONUSES 5

typedef enum {
    BONUS_SPEED_UP,
    BONUS_SPEED_DOWN,
    BONUS_INVINCIBILITY,
    BONUS_DOUBLE_POINTS,
    BONUS_SHRINK
} BonusType;

typedef struct {
    Position pos;
    BonusType type;
    int duration_frames;
    int is_active;
} Bonus;

typedef struct {
    Bonus bonuses[MAX_ACTIVE_BONUSES];  /* Tableau statique */
    int count;
} BonusManager;
```

**Fonctions :**
- `void spawn_bonus(Grid *grid, Snake snake, Bonus *bonus)` - Génère bonus aléatoire
- `void apply_bonus(Game *game, Snake *snake, BonusType type)` - Applique effet
- `void update_active_bonuses(BonusManager *manager)` - Décrémente durées
- `void render_bonus(Bonus bonus)` - Affiche bonus

---

### 📁 **level.c** / **level.h** (FONCTIONNALITÉS BONUS)
**Rôle :** Gestion des niveaux avec obstacles

**Structures :**
```c
#define MAX_WALLS 100
#define MAX_LEVEL_NAME 50

typedef struct {
    int level_number;
    int speed;
    Position walls[MAX_WALLS];  /* Tableau statique */
    int wall_count;
    char level_name[MAX_LEVEL_NAME];
} Level;
```

**Fonctions :**
- `void load_level(Level *level, int level_number)` - Charge niveau depuis fichier
- `void apply_level_to_grid(Grid *grid, Level level)` - Place obstacles sur grille
- `int check_level_completion(Game game)` - Vérifie conditions victoire
- `void init_default_level(Level *level)` - Niveau par défaut

---

### 📁 **multiplayer.c** / **multiplayer.h** (FONCTIONNALITÉS BONUS)
**Rôle :** Mode 2 joueurs

**Structures :**
```c
typedef struct {
    Snake snake1;       /* Joueur 1 (flèches) */
    Snake snake2;       /* Joueur 2 (ZQSD) */
    int score1;
    int score2;
    Food food;          /* Nourriture commune */
    int game_over;      /* 0 = en cours, 1 = J1 gagne, 2 = J2 gagne */
} MultiplayerGame;
```

**Fonctions :**
- `void init_multiplayer(MultiplayerGame *mp_game)` - Init 2 serpents
- `void handle_multiplayer_input(MultiplayerGame *mp_game, MLV_Keyboard_button key)` - 2 contrôles
- `void update_multiplayer(MultiplayerGame *mp_game, Grid *grid)` - Update 2 serpents
- `void render_multiplayer(MultiplayerGame mp_game, Grid grid)` - Affiche 2 serpents

---

### 📁 **utils.c** / **utils.h**
**Rôle :** Fonctions utilitaires

**Fonctions :**
- `int random_int(int min, int max)` - Nombre aléatoire
- `int clamp(int value, int min, int max)` - Limite valeur
- `void error_exit(char message[])` - Affiche erreur et quitte
- `void get_save_filepath(char filepath[], int slot_number)` - Construit chemin
- `void get_scores_filepath(char filepath[])` - Chemin fichier scores
- `int file_exists(char filepath[])` - Vérifie existence fichier
- `void copy_string(char dest[], char src[], int max_len)` - Copie sécurisée

---

## 3. Ordre d'implémentation suggéré

### Phase 1 - Base du jeu (Semaine 1-2)
1. ✅ Makefile et structure de dossiers
2. ⬜ `utils.c/h` - Fonctions de base
3. ⬜ `timer.c/h` - Gestion du temps
4. ⬜ `render.c/h` - Fenêtre MLV de base
5. ⬜ `snake.c/h` - Structure du serpent
6. ⬜ `grid.c/h` - Grille de jeu
7. ⬜ `food.c/h` - Nourriture
8. ⬜ `game_state.c/h` - État du jeu
9. ⬜ `main.c` - Boucle de jeu basique

### Phase 2 - Gameplay (Semaine 3)
10. ⬜ `controls.c/h` - Contrôles clavier
11. ⬜ `collision.c/h` - Détection collisions
12. ⬜ `render.c` - Affichage complet
13. ⬜ Test du jeu de base fonctionnel

### Phase 3 - Menus et système (Semaine 4)
14. ⬜ `menu.c/h` - Menus principaux
15. ⬜ `scores.c/h` - Gestion scores
16. ⬜ `save.c/h` - Sauvegarde/chargement
17. ⬜ Test de toutes les fonctionnalités obligatoires

### Phase 4 - Bonus (Semaine 5)
18. ⬜ `bonus.c/h` - Objets spéciaux
19. ⬜ `level.c/h` - Niveaux avec obstacles
20. ⬜ `multiplayer.c/h` - Mode 2 joueurs
21. ⬜ README.md
22. ⬜ Tests finaux et debug

---

## 4. Points d'attention obligatoires

### ⚠️ Contraintes techniques
- [ ] **PAS DE POINTEURS** (sauf passage par référence avec &)
- [ ] **Tableaux statiques uniquement** (pas de malloc/free)
- [ ] Compilation silencieuse avec `make`
- [ ] Utilisation de MLV obligatoire
- [ ] Chemins relatifs uniquement (`./ressources/`)
- [ ] Gestion d'erreurs robuste (pas de plantage)
- [ ] FPS limités avec `clock_gettime()` et `CLOCK_REALTIME`
- [ ] Boucle de jeu suivant la structure imposée

### ⚠️ Fonctionnalités obligatoires
- [ ] Contrôles avec flèches directionnelles
- [ ] Gestion des demi-tours impossibles
- [ ] Pause avec menu
- [ ] 4 slots de sauvegarde
- [ ] Top 10 scores persistants
- [ ] Menu principal complet
- [ ] Mort au contact du corps
- [ ] Téléportation sur les bords (sans murs)
- [ ] Code modulaire (plusieurs fichiers)
- [ ] Au moins 2 fonctionnalités bonus

### ⚠️ Boucle de jeu imposée
```c
while (!quitter) {
    clock_gettime(CLOCK_REALTIME, &debut);
    
    // Affichage
    // Récupération événement clavier (1 seul par image)
    // Résolution événements
    // Déplacement objets
    // Résolution collisions
    
    clock_gettime(CLOCK_REALTIME, &fin);
    // Calcul temps écoulé
    // Attente si nécessaire
}
```

---

## 5. Adaptation SANS POINTEURS : explications clés

### 🐍 Serpent sans liste chaînée
**Problème :** Normalement on utilise une liste chaînée (avec `next` pointeur)
**Solution :** Tableau circulaire avec indices head et tail

```c
Snake snake;
// Déplacement : 
// 1. Ajouter nouvelle tête à head_index + 1
// 2. Supprimer queue à tail_index (si pas de croissance)
// 3. Avancer les indices (modulo MAX_SNAKE_LENGTH)

// Exemple simplifié :
void move_snake(Snake *snake) {
    // Nouvelle position tête
    snake->head_index = (snake->head_index + 1) % MAX_SNAKE_LENGTH;
    // Calculer nouvelle position selon direction
    // ...
    // Si pas de croissance, avancer la queue
    if (!snake->is_growing) {
        snake->tail_index = (snake->tail_index + 1) % MAX_SNAKE_LENGTH;
    }
}
```

### 📦 Passage de structures
**À faire :**
- Structures **modifiées** → passage par référence `void func(Snake *snake)`
- Structures **lues uniquement** → passage par valeur `void func(Snake snake)` (ou par référence si grosse structure)

### 📄 Gestion de chaînes
**À éviter :** `char *str` et `malloc`
**À faire :** `char str[MAX_LENGTH]` et `strncpy`, `snprintf`

### 💾 Fichiers de sauvegarde
**Recommandé :** Utiliser `fwrite` et `fread` pour écrire structures entières
```c
FILE *file = fopen("save.bin", "wb");
fwrite(&game_save, sizeof(GameSave), 1, file);
fclose(file);
```

### 🎯 Tailles maximales recommandées
```c
#define MAX_SNAKE_LENGTH 500     /* Grille 40x30 = 1200 max, 500 est safe */
#define GRID_WIDTH 40
#define GRID_HEIGHT 30
#define MAX_SCORES 10
#define MAX_SAVE_SLOTS 4
#define MAX_WALLS 100            /* Pour niveaux avec obstacles */
#define MAX_NAME_LENGTH 50
```

---

## 6. Fonctionnalités bonus à implémenter (minimum 2)

**Suggestions :**
- [ ] Mode 2 joueurs (même écran) - **FACILE sans pointeurs**
- [ ] Niveaux avec murs/labyrinthes - **FACILE** (tableau de positions)
- [ ] Objets spéciaux (speed up/down, invincibilité) - **MOYEN**
- [ ] Sauvegardes binaires - **FACILE** (fwrite/fread de structures)
- [ ] Chronomètre + score basé sur le temps - **FACILE**
- [ ] Différents types de nourriture (valeurs différentes) - **FACILE**
- [ ] Système de niveaux progressifs - **MOYEN**
- [ ] Sons et musique (MLV_Audio) - **FACILE**
- [ ] Thèmes graphiques - **FACILE**
- [ ] Mini-map - **MOYEN**
- [ ] Replay du meilleur score - **DIFFICILE** (enregistrer inputs) 


