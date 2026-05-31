#include "../../include/HUD.h"
#include <ncurses.h>

HUD::HUD() {}

void HUD::render(
    Player* player,
    int currentLevel,
    Boss* boss,
    int activeEnemies,
    int activeProjectiles,
    const std::vector<std::string>& eventLog
) {
    if (player != nullptr) {
        if (has_colors()) attron(COLOR_PAIR(4) | A_BOLD);
        if (eventLog.empty()) {
            mvprintw(
                1,
                2,
                "Vida:%d  Score:%d  Nivel:%d  Habilidad:%s",
                player->getHealth(),
                player->getScore(),
                currentLevel,
                player->getAbilityName().c_str()
            );
        } else {
            mvprintw(
                1,
                2,
                "Kirby(%d,%d)  Vida:%d  Score:%d  Nivel:%d  Hab:%s",
                player->getX(),
                player->getY(),
                player->getHealth(),
                player->getScore(),
                currentLevel,
                player->getAbilityName().c_str()
            );
        }
        if (has_colors()) attroff(COLOR_PAIR(4) | A_BOLD);

        if (boss != nullptr && boss->isActive()) {
            if (has_colors()) attron(COLOR_PAIR(8) | A_BOLD);
            mvprintw(2, 2, "Boss HP: %d", boss->getHealth());
            if (has_colors()) attroff(COLOR_PAIR(8) | A_BOLD);
        }

        if (has_colors()) attron(COLOR_PAIR(3));
        mvprintw(2, 20, "Enemigos:%d  Ataques:%d", activeEnemies, activeProjectiles);
        if (has_colors()) attroff(COLOR_PAIR(3));

        if (has_colors()) attron(COLOR_PAIR(4));
        mvprintw(3, 0, "--------------------------------------------------------------------------------");
        if (has_colors()) attroff(COLOR_PAIR(4));

        int maxY, maxX;
        getmaxyx(stdscr, maxY, maxX);
        int startY = maxY - 4;

        if (startY > 4 && !eventLog.empty()) {
            if (has_colors()) attron(COLOR_PAIR(4) | A_BOLD);
            mvprintw(startY, 2, "Registro:");
            if (has_colors()) attroff(COLOR_PAIR(4) | A_BOLD);

            int logStart = eventLog.size() > 3 ? eventLog.size() - 3 : 0;
            for (int i = logStart; i < (int)eventLog.size(); i++) {
                std::string line = eventLog[i].substr(0, maxX - 6);
                mvprintw(startY + 1 + (i - logStart), 4, "%s", line.c_str());
            }
        }
    }
}
