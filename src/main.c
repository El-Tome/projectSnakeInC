#include "main.h"
#include "menu.h"

int main() {
    /* Déclaration des variables */
    WindowSize window_size;
    ButtonsList buttons_list;
    MenuState menu_state;
    int quitter;
    struct timespec debut, fin;
    long elapsed_ns;

    /* Initialisation des variables */
    menu_state = MAIN_MENU;
    quitter    = 0;
    buttons_list.nb_buttons = 0;
    buttons_list.selected_button = 0;

    srand(time(NULL));

    /* Création de la fenetre MLV */
    MLV_get_desktop_size(&window_size.width, &window_size.height);
    MLV_create_full_screen_window("Snake", "Snake", window_size.width, window_size.height);    

    while (!quitter) {
        clock_gettime(CLOCK_REALTIME, &debut );
        /* 1. Mis à jour de l'image */
        MLV_update_window();
        /* 2. Effacement de l'écran */
        MLV_clear_window(MLV_COLOR_BLACK);

        /* 3. Défini dans quel endroit on est dans le jeu et affiche le menu correspondant */
        switch (menu_state) {
            case MAIN_MENU:
                display_menu(window_size, &buttons_list);

                switch (handle_main_menu_navigation(&buttons_list)) {
                    case 0:
                        menu_state = NEW_GAME_MENU;
                        buttons_list.selected_button = 1;
                        break;
                    case 1:
                        menu_state = LOAD_GAME_MENU;
                        break;
                    case 2:
                        menu_state = SCORES_MENU;
                        break;
                    case 3:
                        quitter = 1;
                        break;
                    default:
                        break;
                }

                break;


            case NEW_GAME_MENU:
                display_new_game_menu(window_size, &buttons_list);

                /* Gestion des actions des boutons */
                switch(handle_new_game_menu_navigation(&buttons_list)) {
                    case 0:
                        /* TODO: Basculer mode 2 joueurs */
                        break;
                    case 1:
                        /* TODO: Basculer Murs */
                        break;
                    case 2:
                        /* TODO: Diminuer Largeur */
                        break;
                    case 3:
                        /* TODO: Augmenter Largeur */
                        break;
                    case 4:
                        /* TODO: Diminuer Hauteur */
                        break;
                    case 5:
                        /* TODO: Augmenter Hauteur */
                        break;
                    case 6:
                        /* TODO: Diminuer Vitesse */
                        break;
                    case 7:
                        /* TODO: Augmenter Vitesse */
                        break;
                    case 8:
                        /* Retour */
                        menu_state = MAIN_MENU;
                        break;
                    case 9:
                        /* Lancer */
                        ; /* Code pour dire "Lancer la partie" */
                        break;
                }
                /*create_new_game(); */
                break;
            case LOAD_GAME_MENU:
                /*load_game();*/
                break;
            case SCORES_MENU:
                /*display_scores_menu();*/
                break;
            case IN_GAME:


                break;
            default:
                quitter = 1;
                break;
        }

        clock_gettime(CLOCK_REALTIME, &fin);

        elapsed_ns = (fin.tv_nsec - debut.tv_nsec);
        if (elapsed_ns < FRAME_TIME_NS) {
            MLV_wait_milliseconds((FRAME_TIME_NS - elapsed_ns) / NANOS_TO_MILLIS);
        }
        /*
            
        //     // Affichage
        //     // Récupération événement clavier (1 seul par image)
        //     // Résolution événements
        //     // Déplacement objets
        //     // Résolution collisions
            

        */
    }

    MLV_free_window ();


/*
    // detecte les evenements clavier / souris pour savoir sur quelle bouton on a cliqué

    // si on a cliqué sur le bouton "nouveau jeu", on crée une nouvelle partie

    // si on a cliqué sur le bouton "charger une partie", on charge la partie enregistrée

    // si on a cliqué sur le bouton "quitter", on quitte le jeu

    // si on a cliqué sur le bouton "scores", on affiche les scores
*/
    exit(EXIT_SUCCESS);
}
