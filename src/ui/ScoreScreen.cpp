#include "ScoreScreen.h"
#include <ncurses.h>

void ScoreScreen::show() {
    clear();
    
    mvprintw(2, 20, "=============================");
    mvprintw(3, 20, "    PUNTAJES DESTACADOS      ");
    mvprintw(4, 20, "=============================");

    mvprintw(7, 20, "POS   JUGADOR      PUNTAJE");
    mvprintw(8, 20, "-----------------------------");
    
    mvprintw(10, 20, " 1    CRISTIAN      5000");
    mvprintw(11, 20, " 2    CARLOS        4200");
    mvprintw(12, 20, " 3    SERGIO        3800");
    mvprintw(13, 20, " 4    INVITADO      1500");

    mvprintw(17, 20, "Presiona cualquier tecla para volver al menu...");
    
    refresh();
    getch();
}