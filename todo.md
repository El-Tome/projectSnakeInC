# TODO pour le projet snake en C

## 1. Créer un Makefile
Faire un sorte quand dans le fichier on est juste besoin de faire make pour tout compiler.

La compilation devra compiler le code dans le dossier 
src/       | Contiendra le code source  (fichier .c)
include/   | Contiendra les déclaration (fichier .h)
lib/       | Contiendra la lib MLV si besoin
ressource/ | Contiendra les assets (images, save, score)

et créer le fichier exécutable à la racine ./snake 


## 2. dégrossissement des choses à faire :
main.c <- contiendra uniquement la fonction main et sera l'administreur de l'appelle des fonctions

control.c <- contiendra les fonctions pour transcrire les déplacement entré par l'utilisateur

grille.c <- contiendra la position du serpent et les les updates de position (check si déplacement possible)

grilleBonus.c <- sera un petit override pour permet la modification et la possibilité de règle de jeu en plus

menu.c <- sera les fonctions pour le menu

scores.c <- pour la gestions du score

updateView.c <- fera la maj visuel 


