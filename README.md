# Projet Snake - L2 Informatique

Ce projet est une implémentation du jeu Snake en langage C, utilisant la bibliothèque graphique MLV.

## 1. Compilation

Le projet utilise un **Makefile** pour automatiser la compilation.

### Prérequis
*   **GCC** (Compilateur C)
*   **Bibliothèque MLV** 

### Commandes
À la racine du projet, ouvrez un terminal et exécutez :

```bash
make
```

Cela générera l'exécutable `snake`.

Pour nettoyer les fichiers de compilation (objets et exécutable) :
```bash
make clean
```

## 2. Exécution

Une fois la compilation terminée, lancez le jeu avec la commande :

```bash
./snake
```

> **Note :** Le programme doit être exécuté depuis la racine du projet pour charger correctement les ressources (images, sauvegardes).

## 3. Système de Sauvegardes (Slots)

Le jeu dispose de **4 emplacements de sauvegarde (slots)** numérotés de 0 à 3.
Les sauvegardes ne sont disponibles qu'en **mode Classique** (1 joueur).

### Fonctionnement
1.  **En jeu**, appuyez sur **Echap** (Pause) puis sélectionnez "Sauvegarder".
2.  **Gestion automatique :** Le jeu cherche automatiquement le premier slot vide (par exemple, le slot 0 s'il est libre) et y enregistre la partie.
3.  **Gestion manuelle :** Si les 4 slots sont pleins, un menu s'ouvre pour vous permettre de choisir quel slot écraser.
4.  **Chargement :** Depuis le menu principal, choisissez "Charger Partie" pour voir les slots utilisés. Vous pouvez charger une partie ou supprimer un slot existant.

## 4. Structure des fichiers de sauvegarde

Les sauvegardes sont stockées dans le dossier `ressources/saves/` sous la forme de fichiers binaires `.dat` (ex: `save_0.dat`).

### Format Technique
Les fichiers sont écrits en binaire direct (via `fwrite`) pour correspondre à la structure mémoire du programme. Ils ne sont pas éditables avec un éditeur de texte.

Structure C (`SaveData`) contenue dans le fichier :

| Champ | Type | Description |
|-------|------|-------------|
| `magic` | `unsigned int` | Nombre magique (`0x534E414B`) pour valider le fichier. |
| `version` | `unsigned int` | Version du format de sauvegarde. |
| `timestamp` | `time_t` | Date et heure de la sauvegarde. |
| `settings` | `struct` | Paramètres de la partie (vitesse, taille grille, murs...). |
| `grid` | `struct` | État complet de la grille de jeu. |
| `snake` | `struct` | Position et état du serpent. |
| `food_list` | `struct` | Liste des fruits présents sur la grille. |
| `score` | `int` | Score actuel du joueur. |

Le jeu vérifie l'intégrité du fichier (numéro magique et cohérence des données) avant de le charger pour éviter les plantages.

## 5. Architecture Rapide

Le code sépare strictement la **Logique (Modèle)** de l'**Affichage (Vue)**.

*   **src/** (.c) et **include/** (.h) :
    *   `main.c` / `game.c` : Point d'entrée, boucle principale et gestion des états (Menu, Jeu, Pause).
    *   `snake.c` / `food.c` / `grille.c` : Logique pure des entités (déplacements, collisions, plateau).
    *   `*_display.c` : Gestion de l'affichage MLV uniquement (ne modifie pas les données du jeu).
    *   `menu_handler.c` : Gestion des interactions (clavier/souris) dans les menus.
    *   `save.c` : Gestion bas niveau des fichiers binaires et sérialisation.
