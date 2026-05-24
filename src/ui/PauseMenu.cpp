#include "../../include/PauseMenu.h"
#include <ncurses.h>
#include <string>

bool PauseMenu::show() {
    int choice = 0;
    int highlight = 0;
    std::string options[2] = {"Continuar Partida", "Salir al Menu Principal"};

    while(true) {
        clear();

        if (has_colors()) attron(COLOR_PAIR(4) | A_BOLD);
        else attron(A_BOLD);
        mvprintw(5, 25, "==================");
        mvprintw(6, 25, "      PAUSA       ");
        mvprintw(7, 25, "==================");
        if (has_colors()) attroff(COLOR_PAIR(4) | A_BOLD);
        else attroff(A_BOLD);

        for(int i = 0; i < 2; i++) {
            if(i == highlight) {
                if (has_colors()) attron(COLOR_PAIR(6) | A_BOLD);
                else attron(A_REVERSE);
            } else if (has_colors()) {
                attron(COLOR_PAIR(4));
            }
            mvprintw(10 + (i*2), 25, "%s", options[i].c_str());
            if(i == highlight) {
                if (has_colors()) attroff(COLOR_PAIR(6) | A_BOLD);
                else attroff(A_REVERSE);
            } else if (has_colors()) {
                attroff(COLOR_PAIR(4));
            }
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
            case 10: 
                if (highlight == 0) return true;  
                if (highlight == 1) return false; 
                break;
        }
    }
}
