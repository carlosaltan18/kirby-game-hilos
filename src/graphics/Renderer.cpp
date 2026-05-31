#include "../../include/Renderer.h"
#include "../../include/Boss.h"
#include <ncurses.h>
#include <string>

Renderer::Renderer() {
    backgroundFrame = 0;
}

static void drawAsciiCloud(int startX, int startY, int mapHeight) {
    std::string cloud[3] = {
        "    .--.        ",
        " .-(    ).-.    ",
        "(___.__)____)   "
    };

    if (has_colors()) attron(COLOR_PAIR(1) | A_DIM);

    for (int row = 0; row < 3; row++) {
        int screenY = startY + row;
        if (screenY < 3 || screenY >= mapHeight - 2) {
            continue;
        }

        for (int col = 0; col < (int)cloud[row].length(); col++) {
            if (cloud[row][col] == ' ') {
                continue;
            }

            int screenX = startX + col;
            if (screenX >= 0 && screenX < 80) {
                mvaddch(screenY, screenX, cloud[row][col]);
            }
        }
    }

    if (has_colors()) attroff(COLOR_PAIR(1) | A_DIM);
}

void Renderer::render(
    TileMap &map,
    Camera &camera,
    Player &player,
    std::vector<Enemy*> &enemies,
    std::vector<Projectile*> &projectiles,
    std::vector<Food*> &foods
) {
    animationSystem.update();
    backgroundFrame++;

    // 1. Limpiar el buffer
    erase();
    // Desplazamiento de la cámara para el scroll horizontal
    int offsetX = camera.getOffsetX();
    std::vector<std::string>& grid = map.getGrid();

    int slowFrame = backgroundFrame / 8;
    int firstCloudX = 80 - (slowFrame % 120);
    int secondCloudX = 80 - ((slowFrame + 55) % 120);

    drawAsciiCloud(firstCloudX, 4, map.getHeight());
    drawAsciiCloud(secondCloudX, 10, map.getHeight());

    // Renderizar el mapa
    for (int y = 0; y < map.getHeight(); y++) {
        std::string visibleRow = "";
        if (offsetX < (int)grid[y].length()) {
            visibleRow = grid[y].substr(offsetX, 80);
        }

        for (int x = 0; x < (int)visibleRow.length(); x++) {
            char tile = visibleRow[x];
            if (has_colors() && tile == '#') {
                attron(COLOR_PAIR(5) | A_BOLD);
                mvaddch(y, x, tile);
                attroff(COLOR_PAIR(5) | A_BOLD);
            } else if (has_colors() && tile != ' ') {
                attron(COLOR_PAIR(3) | A_BOLD);
                mvaddch(y, x, tile);
                attroff(COLOR_PAIR(3) | A_BOLD);
            } else if (tile != ' ') {
                mvaddch(y, x, tile);
            }
        }
    }

    // Renderizar al Jugador
    // Calcular la posición del jugador en la pantalla
    int playerScreenX = player.getX() - offsetX;
    if (playerScreenX >= 0 && playerScreenX < 80) {
        if (has_colors()) attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(player.getY(), playerScreenX, "%s", player.getSymbol().c_str());
        if (has_colors()) attroff(COLOR_PAIR(1) | A_BOLD);
    }
    for (auto enemy : enemies) {
        if (enemy->isActive()) {
            int enemyScreenX = enemy->getX() - offsetX;
            // Comprobar frustum culling horizontal
            if (enemyScreenX >= 0 && enemyScreenX < 80) {
                Boss* boss = dynamic_cast<Boss*>(enemy);
                if (boss != nullptr) {
                    if (has_colors()) attron(COLOR_PAIR(8) | A_BOLD);
                    mvprintw(enemy->getY(), enemyScreenX, "%s", boss->getLine1().c_str());
                    mvprintw(enemy->getY() + 1, enemyScreenX, "%s", boss->getLine2().c_str());
                    mvprintw(enemy->getY() + 2, enemyScreenX, "%s", boss->getLine3().c_str());
                    if (has_colors()) attroff(COLOR_PAIR(8) | A_BOLD);
                } else {
                    std::string sprite = animationSystem.getEnemySprite(enemy->isActive());
                    if (has_colors()) attron(COLOR_PAIR(2) | A_BOLD);
                    mvprintw(enemy->getY(), enemyScreenX, "%s", sprite.c_str());
                    if (has_colors()) attroff(COLOR_PAIR(2) | A_BOLD);
                }
            }
        }
    }

    // Renderizar Comida
    for (auto food : foods) {
        if (food->isActive()) {
            int foodScreenX = food->getX() - offsetX;
            if (foodScreenX >= 0 && foodScreenX < 80) {
                std::string sprite = food->getSymbol();
                if (has_colors()) attron(COLOR_PAIR(3) | A_BOLD);
                mvprintw(food->getY(), foodScreenX, "%s", sprite.c_str());
                if (has_colors()) attroff(COLOR_PAIR(3) | A_BOLD);
            }
        }
    }

    // Renderizar Proyectiles
    for (auto projectile : projectiles) {
        if (projectile->isActive()) { // Verificar que el proyectil no haya colisionado
            int projScreenX = projectile->getX() - offsetX;
            if (projScreenX >= 0 && projScreenX < 80) {
                if (has_colors()) attron(COLOR_PAIR(3) | A_BOLD);
                mvprintw(projectile->getY(), projScreenX, "%s", projectile->getSymbol().c_str());
                if (has_colors()) attroff(COLOR_PAIR(3) | A_BOLD);
            }
        }
    }
}
