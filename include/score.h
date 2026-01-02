#ifndef SNAKE_SCORE_H
#define SNAKE_SCORE_H

#include "game.h"
#include "file_utils.h"

#define SCORES_FILE_PATH "ressources/scores.dat"
#define MAX_SCORES 10

/* Structure d'une entree de score */
typedef struct {
    int score;              /* Score obtenu (longueur du serpent - longueur initiale) * vitesse */
    int width;              /* Largeur de la grille */
    int height;             /* Hauteur de la grille */
    int speed;              /* Vitesse du jeu */
    int is_two_players;     /* Mode 2 joueurs */
    int has_walls;          /* Presence de murs */
} ScoreEntry;

/* Structure contenant tous les scores sauvegardes */
typedef struct {
    ScoreEntry entries[MAX_SCORES];
    int count;              /* Nombre de scores enregistres (0 a MAX_SCORES) */
} ScoreBoard;

/*
 * Initialise un tableau de scores vide.
 */
void init_score_board(ScoreBoard *board);

/*
 * Charge les scores depuis le fichier.
 * Retourne FILE_UTILS_SUCCESS si OK,
 * FILE_UTILS_ERR_OPEN si fichier inexistant (initialise un board vide),
 */
int load_scores(ScoreBoard *board);

/*
 * Sauvegarde les scores dans le fichier.
 * Retourne FILE_UTILS_SUCCESS en cas de succes, un code d'erreur sinon.
 */
int save_scores(const ScoreBoard *board);

/*
 * Ajoute un nouveau score au tableau (trie par ordre decroissant).
 * Si le tableau est plein, le score ne sera ajoute que s'il est
 * superieur au plus petit score existant.
 * Retourne 1 si le score a ete ajoute, 0 sinon.
 */
int add_score(ScoreBoard *board, const ScoreEntry *entry);

/*
 * Cree une entree de score a partir des parametres de jeu actuels.
 */
ScoreEntry create_score_entry(int score, const GameSettings *settings);

/*
 * Verifie si un score merite d'etre enregistre (top 10).
 * Retourne 1 si le score peut etre ajoute, 0 sinon.
 */
int is_high_score(const ScoreBoard *board, int score);

/*
 * Recupere le meilleur score. Retourne 0 si aucun score enregistre.
 */
int get_best_score(const ScoreBoard *board);

/*
 * Recupere une entree de score par index (0 = meilleur).
 * Retourne NULL si index invalide.
 */
const ScoreEntry* get_score_entry(const ScoreBoard *board, int index);

/*
 * Supprime un score par index.
 * Retourne 1 si suppression reussie, 0 sinon.
 */
int remove_score(ScoreBoard *board, int index);

#endif

