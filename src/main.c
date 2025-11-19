#include "main.h"

int main() {
    unsigned int height = 600;
    unsigned int width = 800;
    int quitter = 0;
    MenuState menu_state = MAIN_MENU;


    /* Créaation de la fenetre MLV */
    MLV_get_desktop_size(&width, &height);
    MLV_create_full_screen_window("Snake", "Snake", width, height);    

    while (!quitter) {
        // mis à jour de l'image

        // défini dans quel endroit on est dans le jeu
        switch (menu_state) {
            case MAIN_MENU:
                display_menu();
                break;
            case NEW_GAME_MENU:
                create_new_game();
                break;
            case LOAD_GAME_MENU:
                load_game();
                break;
            case SCORES_MENU:
                display_scores_menu();
                break;
            case IN_GAME:
                break;
            default:
                quitter = 1;
                break;
        }
    //     clock_gettime(CLOCK_REALTIME, &debut);
        
    //     // Affichage
    //     // Récupération événement clavier (1 seul par image)
    //     // Résolution événements
    //     // Déplacement objets
    //     // Résolution collisions
        
    //     clock_gettime(CLOCK_REALTIME, &fin);
    //     // Calcul temps écoulé
    //     // Attente si nécessaire
    }


    // detecte les evenements clavier / souris pour savoir sur quelle bouton on a cliqué

    // si on a cliqué sur le bouton "nouveau jeu", on crée une nouvelle partie

    // si on a cliqué sur le bouton "charger une partie", on charge la partie enregistrée

    // si on a cliqué sur le bouton "quitter", on quitte le jeu

    // si on a cliqué sur le bouton "scores", on affiche les scores

    exit(EXIT_SUCCESS);
}
