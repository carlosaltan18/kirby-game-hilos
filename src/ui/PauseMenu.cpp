#include "../../include/PauseMenu.h"
#include <ncurses.h>
#include <string>

bool PauseMenu::show() {
    int choice = 0;
    int highlight = 0;
    std::string options[2] = {"Continuar Partida", "Salir al Menu Principal"};

    while(true) {
        clear();
        
        mvprintw(5, 25, "==================");
        mvprintw(6, 25, "      PAUSA       ");
        mvprintw(7, 25, "==================");

        for(int i = 0; i < 2; i++) {
            if(i == highlight) attron(A_REVERSE);
            mvprintw(10 + (i*2), 25, "%s", options[i].c_str());
            if(i == highlight) attroff(A_REVERSE);
        }

        refresh();
        choice = getch();

        switch(choice) {
            case KEY_UP:
                highlight = (highlight == 0) ? 1 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == 1) ? 0 : highlight + 1;
                break;
            case 10: // Enter
                if (highlight == 0) return true;  // Continuar
                if (highlight == 1) return false; // Salir
                break;
        }
    }
}