#include "score.h"

void init_score_board(ScoreBoard *board) {
    int i;
    board->count = 0;
    for (i = 0; i < MAX_SCORES; i++) {
        board->entries[i].score     = 0;
        board->entries[i].width     = 0;
        board->entries[i].height    = 0;
        board->entries[i].speed     = 0;
        board->entries[i].has_walls = 0;
        board->entries[i].is_two_players = 0;
    }
}

/* Valide et corrige les donnees lues depuis le fichier */
void validate_score_board(ScoreBoard *board) {
    int i, valid_count;
    
    /* Borne le count entre 0 et MAX_SCORES */
    if (board->count < 0) {
        board->count = 0;
    }
    if (board->count > MAX_SCORES) {
        board->count = MAX_SCORES;
    }
    
    /* Verifie chaque score et ignore les invalides (negatifs) */
    valid_count = 0;
    for (i = 0; i < board->count; i++) {
        if (board->entries[i].score > 0) {
            /* Garde ce score, le deplace si besoin */
            if (valid_count != i) {
                board->entries[valid_count] = board->entries[i];
            }
            valid_count++;
        }
    }
    board->count = valid_count;
}

int load_scores(ScoreBoard *board) {
    int result;
    
    /* Initialise un board vide par defaut */
    init_score_board(board);
    
    /* Verifie si le fichier existe */
    if (!file_exists(SCORES_FILE_PATH)) {
        return FILE_UTILS_ERR_OPEN;
    }
    
    /* Tente de lire le fichier */
    result = read_binary_file(SCORES_FILE_PATH, board, sizeof(ScoreBoard));
    
    if (result != FILE_UTILS_SUCCESS) {
        init_score_board(board);
        return result;
    }
    
    /* Valide et corrige les donnees */
    validate_score_board(board);
    
    return FILE_UTILS_SUCCESS;
}

int save_scores(const ScoreBoard *board) {
    return write_binary_file(SCORES_FILE_PATH, board, sizeof(ScoreBoard));
}

int add_score(ScoreBoard *board, const ScoreEntry *entry) {
    int i;
    int insert_pos;
    
    /* Ignore les scores invalides */
    if (entry->score <= 0) {
        return 0;
    }
    
    /* Trouve la position d'insertion (tri decroissant par score) */
    insert_pos = board->count;
    for (i = 0; i < board->count; i++) {
        if (entry->score > board->entries[i].score) {
            insert_pos = i;
            break;
        }
    }
    
    /* Verifie si le score peut etre insere */
    if (insert_pos >= MAX_SCORES) {
        return 0;
    }
    
    /* Decale les scores inferieurs vers le bas */
    for (
        i = (board->count < MAX_SCORES ? board->count : MAX_SCORES - 1); 
        i > insert_pos; 
        i--
    ) {
        board->entries[i] = board->entries[i - 1];
    }
    
    /* Insere le nouveau score */
    board->entries[insert_pos] = *entry;
    
    /* Met a jour le compteur (max MAX_SCORES) */
    if (board->count < MAX_SCORES) {
        board->count++;
    }
    
    return 1;
}

ScoreEntry create_score_entry(int score, const GameSettings *settings) {
    ScoreEntry entry;
    
    entry.score  = score;
    entry.width  = settings->width;
    entry.height = settings->height;
    entry.speed  = settings->speed;
    entry.is_two_players = settings->is_two_players;
    entry.has_walls      = settings->has_walls;
    
    return entry;
}

int is_high_score(const ScoreBoard *board, int score) {
    int is_high_score;
    if (score <= 0) {
        is_high_score = 0;
    } else if (board->count < MAX_SCORES) {
        is_high_score = 1;
    } else {
        is_high_score = score > board->entries[MAX_SCORES - 1].score;
    }
    
    return is_high_score;
}

int get_best_score(const ScoreBoard *board) {
    int best_score;

    if (board->count == 0) {
        best_score = 0;
    } else {
        best_score = board->entries[0].score;
    }
    
    return best_score;
}

const ScoreEntry* get_score_entry(const ScoreBoard *board, int index) {
    const ScoreEntry* score_entry;

    if (index < 0 || index >= board->count) {
        score_entry = NULL;
    } else {
        score_entry = &board->entries[index];
    }
    
    return score_entry;
}

int remove_score(ScoreBoard *board, int index) {
    int i;
    
    if (index < 0 || index >= board->count) {
        return 0;
    }
    
    /* Decale tous les scores suivants vers le haut */
    for (i = index; i < board->count - 1; i++) {
        board->entries[i] = board->entries[i + 1];
    }
    
    board->count--;
    return 1;
}
