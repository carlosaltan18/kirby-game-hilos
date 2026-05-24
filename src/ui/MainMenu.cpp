#include "../../include/MainMenu.h"
#include "../../include/InstructionScreen.h"
#include "../../include/ScoreScreen.h"
#include "../../include/Game.h" 
#include <ncurses.h>
#include <string>

void MainMenu::show() {
    int choice = 0;
    int highlight = 0;

    std::string options[4] = {
        "Iniciar Partida",
        "Instrucciones",
        "Puntajes Destacados",
        "Salir"
    };

    InstructionScreen instructionScreen;
    ScoreScreen scoreScreen;
    
    while(true) {
        clear();
        if (has_colors()) attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(2, 22, " _  _______ _____  ______ __     __ ");
        mvprintw(3, 22, "| |/ /_   _|  __ \\|  _ \\ \\ \\   / / ");
        mvprintw(4, 22, "| ' /  | | | |__) | |_) | \\ \\_/ /  ");
        mvprintw(5, 22, "|  <   | | |  _  /|  _ <   \\   /   ");
        mvprintw(6, 22, "| . \\ _| |_| | \\ \\| |_) |   | |    ");
        mvprintw(7, 22, "|_|\\_\\_____|_|  \\_\\____/    |_|    ");
        if (has_colors()) attroff(COLOR_PAIR(1) | A_BOLD);

        if (has_colors()) attron(COLOR_PAIR(4) | A_BOLD);
        mvprintw(9, 28, "--- CONSOLE ENGINE ---");
        if (has_colors()) attroff(COLOR_PAIR(4) | A_BOLD);

        if (has_colors()) attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(10, 36, "(>'-')>");
        if (has_colors()) attroff(COLOR_PAIR(1) | A_BOLD);

        for(int i = 0; i < 4; i++) {
            if(i == highlight) {
                if (has_colors()) attron(COLOR_PAIR(6) | A_BOLD);
                else attron(A_REVERSE | A_BOLD);
                mvprintw(13 + (i * 2), 26, "  -> %-19s <-  ", options[i].c_str());
                if (has_colors()) attroff(COLOR_PAIR(6) | A_BOLD);
                else attroff(A_REVERSE | A_BOLD);
            } else {
                if (has_colors()) attron(COLOR_PAIR(4));
                mvprintw(13 + (i * 2), 31, "%s", options[i].c_str());
                if (has_colors()) attroff(COLOR_PAIR(4));
            }
        }

        if (has_colors()) attron(COLOR_PAIR(3));
        mvprintw(22, 12, "[ARRIBA / ABAJO] Navegar   ---   [ENTER] Seleccionar");
        if (has_colors()) attroff(COLOR_PAIR(3));

        refresh();
        choice = getch();

        switch(choice) {
            case KEY_UP:
                highlight = (highlight == 0) ? 3 : highlight - 1;
                break;

            case KEY_DOWN:
                highlight = (highlight == 3) ? 0 : highlight + 1;
                break;

            case 10: 
                if (highlight == 0) {
                    Game game;
                    game.init();
                    game.run(); 
                } else if (highlight == 1) {
                    instructionScreen.show();
                } else if (highlight == 2) {
                    scoreScreen.show();
                } else if (highlight == 3) {
                    return; 
                }
                break;

            default:
                break;
        }
    }
}
