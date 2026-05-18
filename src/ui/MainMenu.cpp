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

    // ======================================
    // SUBMENUS UTILIZADOS
    // ======================================
    InstructionScreen instructionScreen;
    ScoreScreen scoreScreen;
    while(true) {
        clear();

        mvprintw(2, 20, "=============================");
        mvprintw(3, 20, "      KIRBY CONSOLE ENGINE");
        mvprintw(4, 20, "=============================");

        for(int i = 0; i < 4; i++) {
            if(i == highlight)
                attron(A_REVERSE);

            mvprintw(8 + (i * 2), 25, "%s", options[i].c_str());

            if(i == highlight)
                attroff(A_REVERSE);
        }

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
                    // --- EJECUCIÓN DEL MOTOR CONCURRENTE REAL ---
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