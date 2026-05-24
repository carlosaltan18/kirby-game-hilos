#include "../../include/HUD.h"
#include <ncurses.h>

HUD::HUD() {}

void HUD::render(Player* player, int currentLevel, Boss* boss) {
    if (player != nullptr) {
        if (has_colors()) attron(COLOR_PAIR(4) | A_BOLD);
        mvprintw(1, 2, "Vida: %d    Score: %d    Nivel: %d", player->getHealth(), player->getScore(), currentLevel);
        if (has_colors()) attroff(COLOR_PAIR(4) | A_BOLD);
        if (boss != nullptr && boss->isActive()) {
            if (has_colors()) attron(COLOR_PAIR(8) | A_BOLD);
            mvprintw(1, 45, "Boss HP: %d", boss->getHealth());
            if (has_colors()) attroff(COLOR_PAIR(8) | A_BOLD);
        }
        if (has_colors()) attron(COLOR_PAIR(4));
        mvprintw(2, 0, "--------------------------------------------------------------------------------");
        if (has_colors()) attroff(COLOR_PAIR(4));
    }
}
