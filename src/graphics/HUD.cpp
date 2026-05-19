#include "../../include/HUD.h"
#include <ncurses.h>

HUD::HUD() {}

void HUD::render(Player* player, int currentLevel, Boss* boss) {
    if (player != nullptr) {
        mvprintw(1, 2, "Vida: %d    Score: %d    Nivel: %d", player->getHealth(), player->getScore(), currentLevel);
        if (boss != nullptr && boss->isActive()) {
            mvprintw(1, 45, "Boss HP: %d", boss->getHealth());
        }
        mvprintw(2, 0, "--------------------------------------------------------------------------------");
    }
}
