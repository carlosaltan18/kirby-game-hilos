#include "../../include/GameOverScreen.h"
#include <ncurses.h>
#include <string>

void GameOverScreen::show(int finalScore) {
    clear(); 

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX); 

    int boxWidth = 40;
    int startX = (maxX - boxWidth) / 2; 
    int startY = (maxY / 2) - 5;        

    attron(A_BOLD); 
    mvprintw(startY,     startX, "========================================");
    mvprintw(startY + 1, startX, "|                                      |");
    mvprintw(startY + 2, startX, "|              GAME OVER               |");
    mvprintw(startY + 3, startX, "|                                      |");
    mvprintw(startY + 4, startX, "========================================");
    attroff(A_BOLD); 

    std::string scoreText = "Puntaje Final: " + std::to_string(finalScore);
    int scoreX = (maxX - scoreText.length()) / 2;
    mvprintw(startY + 7, scoreX, "%s", scoreText.c_str());

    std::string continueText = "Presiona cualquier tecla para continuar...";
    int continueX = (maxX - continueText.length()) / 2;
    
    attron(A_DIM); 
    mvprintw(startY + 11, continueX, "%s", continueText.c_str());
    attroff(A_DIM);
    refresh();
    getch(); 
}