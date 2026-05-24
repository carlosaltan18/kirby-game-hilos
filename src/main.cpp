#include <ncurses.h>
#include "MainMenu.h"

int main() {
    initscr();              
    noecho();               
    cbreak();               
    keypad(stdscr, TRUE);   
    curs_set(0);

    if (has_colors()) {
        start_color();
        use_default_colors();

        short kirbyPink = COLOR_MAGENTA;
        if (can_change_color() && COLORS > 16) {
            init_color(16, 1000, 350, 750);
            kirbyPink = 16;
        }

        init_pair(1, kirbyPink, -1);
        init_pair(2, COLOR_RED, -1);
        init_pair(3, COLOR_YELLOW, -1);
        init_pair(4, COLOR_CYAN, -1);
        init_pair(5, COLOR_GREEN, -1);
        init_pair(6, COLOR_BLACK, COLOR_YELLOW);
        init_pair(7, COLOR_WHITE, COLOR_BLUE);
        init_pair(8, COLOR_WHITE, COLOR_RED);
    }

    MainMenu menu;
    menu.show();

    endwin();               
    return 0;
}
