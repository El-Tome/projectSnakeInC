#include "main.h"
#include "menu.h"

int main() {
    WindowSize window_size;
    MenuState menu_state = MAIN_MENU;
    int quitter          = 0;



    /* Création de la fenetre MLV */
    MLV_get_desktop_size(&window_size.width, &window_size.height);
    MLV_create_full_screen_window("Snake", "Snake", window_size.width, window_size.height);    

    while (!quitter) {
        /* 1. Mis à jour de l'image */
        MLV_update_window();
        /* 2. Effacement de l'écran */
        MLV_clear_window(MLV_COLOR_BLACK);

        /* 3. Défini dans quel endroit on est dans le jeu et affiche le menu correspondant */
        switch (menu_state) {
            case MAIN_MENU:
                display_menu(window_size);
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

    MLV_free_window ();



    // detecte les evenements clavier / souris pour savoir sur quelle bouton on a cliqué

    // si on a cliqué sur le bouton "nouveau jeu", on crée une nouvelle partie

    // si on a cliqué sur le bouton "charger une partie", on charge la partie enregistrée

    // si on a cliqué sur le bouton "quitter", on quitte le jeu

    // si on a cliqué sur le bouton "scores", on affiche les scores

    exit(EXIT_SUCCESS);
}
