#include "ScoreScreen.h"
#include "../../include/ScoreManager.h"
#include <ncurses.h>
#include <string>

void ScoreScreen::show() {
    clear();
    auto scores = ScoreManager::loadTopScores();
    
    mvprintw(2, 20, "=============================");
    mvprintw(3, 20, "    PUNTAJES DESTACADOS      ");
    mvprintw(4, 20, "=============================");

    mvprintw(7, 20, "POS   JUGADOR      PUNTAJE");
    mvprintw(8, 20, "-----------------------------");

    if (scores.empty()) {
        mvprintw(10, 20, "Aun no hay puntajes guardados");
    } else {
        int maxRows = scores.size() > 10 ? 10 : scores.size();
        for (int i = 0; i < maxRows; i++) {
            mvprintw(10 + i, 20, "%2d    %-10s   %6d", i + 1, scores[i].first.c_str(), scores[i].second);
        }
    }

    mvprintw(22, 20, "Presiona cualquier tecla para volver al menu...");
    
    refresh();
    getch();
}
