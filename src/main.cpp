#include <ncurses.h>
#include "MainMenu.h"

int main() {
    initscr();              
    noecho();               
    cbreak();               
    keypad(stdscr, TRUE);   
    curs_set(0);            

    MainMenu menu;
    menu.show();

    endwin();               
    return 0;
}