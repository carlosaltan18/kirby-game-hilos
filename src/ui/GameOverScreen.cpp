#include "../../include/GameOverScreen.h"
#include <ncurses.h>

void GameOverScreen::show(int finalScore) {
    clear();
    
    mvprintw(5, 20, "#################################");
    mvprintw(6, 20, "#                               #");
    mvprintw(7, 20, "#          GAME OVER            #");
    mvprintw(8, 20, "#                               #");
    mvprintw(9, 20, "#################################");

    mvprintw(12, 25, "Puntaje Final: %d", finalScore);

    mvprintw(16, 20, "Presiona cualquier tecla para continuar...");
    
    refresh();
    getch(); 
}