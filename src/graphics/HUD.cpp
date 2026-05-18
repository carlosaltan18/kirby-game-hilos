#include "../../include/HUD.h"
#include <ncurses.h>

HUD::HUD() {}

void HUD::render(Player* player, int currentLevel) {
    if (player != nullptr) {
        mvprintw(1, 2, "Vida: %d    Score: %d    Nivel: %d", player->getHealth(), player->getScore(), currentLevel);
        mvprintw(2, 0, "--------------------------------------------------------------------------------");
    }
}