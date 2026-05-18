#include "../../include/HUD.h"
#include <ncurses.h>

void HUD::draw(int lives, int score, int level) {
    // Dibuja la barra de información en la parte superior de la pantalla
    mvprintw(1, 2, "Vida: %d    Score: %d    Nivel: %d", lives, score, level);
    mvprintw(2, 0, "------------------------------------------------------------");
}