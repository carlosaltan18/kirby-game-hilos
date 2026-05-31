#include "../../include/GameOverScreen.h"
#include "../../include/ScoreManager.h"
#include <ncurses.h>
#include <string>

static std::string askPlayerName(int y, int maxX) {
    echo();
    curs_set(1);
    nodelay(stdscr, FALSE);

    char nameBuffer[24];
    std::string prompt = "Nombre: ";
    int promptX = (maxX - 32) / 2;

    if (has_colors()) attron(COLOR_PAIR(4) | A_BOLD);
    mvprintw(y, promptX, "%s", prompt.c_str());
    if (has_colors()) attroff(COLOR_PAIR(4) | A_BOLD);
    getnstr(nameBuffer, 23);

    noecho();
    curs_set(0);

    std::string playerName = nameBuffer;
    if (playerName.empty()) {
        playerName = "INVITADO";
    }

    return playerName;
}

static bool askRestartChoice(int y, int maxX) {
    std::string restartText = "[R] Reiniciar partida    [M] Volver al menu";
    int restartX = (maxX - restartText.length()) / 2;

    if (has_colors()) attron(COLOR_PAIR(4) | A_BOLD);
    mvprintw(y, restartX, "%s", restartText.c_str());
    if (has_colors()) attroff(COLOR_PAIR(4) | A_BOLD);
    refresh();
    flushinp();

    while (true) {
        int ch = getch();
        if (ch == 'r' || ch == 'R') {
            return true;
        }
        if (ch == 'm' || ch == 'M' || ch == 10) {
            return false;
        }
    }
}

bool GameOverScreen::show(int finalScore) {
    clear(); 

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX); 

    int boxWidth = 40;
    int startX = (maxX - boxWidth) / 2; 
    int startY = (maxY / 2) - 5;        

    if (has_colors()) attron(COLOR_PAIR(8) | A_BOLD);
    else attron(A_BOLD);
    mvprintw(startY,     startX, "========================================");
    mvprintw(startY + 1, startX, "|                                      |");
    mvprintw(startY + 2, startX, "|              GAME OVER               |");
    mvprintw(startY + 3, startX, "|                                      |");
    mvprintw(startY + 4, startX, "========================================");
    if (has_colors()) attroff(COLOR_PAIR(8) | A_BOLD);
    else attroff(A_BOLD);

    std::string scoreText = "Puntaje Final: " + std::to_string(finalScore);
    int scoreX = (maxX - scoreText.length()) / 2;
    if (has_colors()) attron(COLOR_PAIR(3) | A_BOLD);
    mvprintw(startY + 7, scoreX, "%s", scoreText.c_str());
    if (has_colors()) attroff(COLOR_PAIR(3) | A_BOLD);

    std::string playerName = askPlayerName(startY + 9, maxX);
    ScoreManager::saveScore(finalScore, playerName);

    return askRestartChoice(startY + 12, maxX);
}

bool GameOverScreen::showVictory(int finalScore) {
    clear();

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    int boxWidth = 44;
    int startX = (maxX - boxWidth) / 2;
    int startY = (maxY / 2) - 5;

    if (has_colors()) attron(COLOR_PAIR(5) | A_BOLD);
    else attron(A_BOLD);
    mvprintw(startY,     startX, "============================================");
    mvprintw(startY + 1, startX, "|                                          |");
    mvprintw(startY + 2, startX, "|              JEFE DERROTADO             |");
    mvprintw(startY + 3, startX, "|                                          |");
    mvprintw(startY + 4, startX, "============================================");
    if (has_colors()) attroff(COLOR_PAIR(5) | A_BOLD);
    else attroff(A_BOLD);

    std::string scoreText = "Puntaje Final: " + std::to_string(finalScore);
    int scoreX = (maxX - scoreText.length()) / 2;
    if (has_colors()) attron(COLOR_PAIR(3) | A_BOLD);
    mvprintw(startY + 7, scoreX, "%s", scoreText.c_str());
    if (has_colors()) attroff(COLOR_PAIR(3) | A_BOLD);

    std::string playerName = askPlayerName(startY + 9, maxX);
    ScoreManager::saveScore(finalScore, playerName);

    return askRestartChoice(startY + 12, maxX);
}
