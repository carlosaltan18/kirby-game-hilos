#include "ScoreScreen.h"
#include "../../include/ScoreManager.h"
#include <ncurses.h>
#include <string>

void ScoreScreen::show() {
    clear();
    auto scores = ScoreManager::loadTopScores();

    if (has_colors()) attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(2, 20, "=============================");
    mvprintw(3, 20, "    PUNTAJES DESTACADOS      ");
    mvprintw(4, 20, "=============================");
    if (has_colors()) attroff(COLOR_PAIR(1) | A_BOLD);

    if (has_colors()) attron(COLOR_PAIR(4) | A_BOLD);
    mvprintw(7, 20, "POS   JUGADOR      PUNTAJE");
    mvprintw(8, 20, "-----------------------------");
    if (has_colors()) attroff(COLOR_PAIR(4) | A_BOLD);

    if (scores.empty()) {
        if (has_colors()) attron(COLOR_PAIR(3));
        mvprintw(10, 20, "Aun no hay puntajes guardados");
        if (has_colors()) attroff(COLOR_PAIR(3));
    } else {
        int maxRows = scores.size() > 10 ? 10 : (int)scores.size();
        for (int i = 0; i < maxRows; i++) {
            if (has_colors()) attron(i == 0 ? COLOR_PAIR(3) | A_BOLD : COLOR_PAIR(4));
            mvprintw(10 + i, 20, "%2d    %-10s   %6d", i + 1, scores[i].first.c_str(), scores[i].second);
            if (has_colors()) attroff(i == 0 ? COLOR_PAIR(3) | A_BOLD : COLOR_PAIR(4));
        }
    }

    if (has_colors()) attron(COLOR_PAIR(3));
    mvprintw(22, 20, "Presiona cualquier tecla para volver al menu...");
    if (has_colors()) attroff(COLOR_PAIR(3));

    refresh();
    getch();
}
