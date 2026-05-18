#include "../../include/Renderer.h"
#include <ncurses.h>


Renderer::Renderer() {}

// ======================================
// MOTOR DE RENDERIZADO
// ======================================
void Renderer::render(
    TileMap &map,
    Camera &camera,
    Player &player,
    std::vector<Enemy*> &enemies,
    std::vector<Projectile*> &projectiles
) {
    clear();

    int offsetX = camera.getOffsetX();
    std::vector<std::string>& grid = map.getGrid();

    // ======================================
    // 1. RENDERIZAR MAPA SEGÚN LA CÁMARA
    // ======================================
    for (int y = 0; y < map.getHeight(); y++) {
        std::string visibleRow = "";
        if (offsetX < (int)grid[y].length()) {
            visibleRow = grid[y].substr(offsetX, 80); 
        }
        mvprintw(y, 0, "%s", visibleRow.c_str());
    }

    // ======================================
    // 2. RENDERIZAR AL JUGADOR 
    // ======================================
    int playerScreenX = player.getX() - offsetX;
    if (playerScreenX >= 0 && playerScreenX < 80) {
        mvprintw(player.getY(), playerScreenX, "%s", player.getSymbol().c_str());
    }

    // ======================================
    // 3. RENDERIZAR ENEMIGOS
    // ======================================
    for (auto enemy : enemies) {
        if (enemy->isActive()) {
            int enemyScreenX = enemy->getX() - offsetX;
            if (enemyScreenX >= 0 && enemyScreenX < 80) {
                mvprintw(enemy->getY(), enemyScreenX, "%s", enemy->getSymbol().c_str());
            }
        }
    }

    // ======================================
    // 4. RENDERIZAR PROYECTILES
    // ======================================
    for (auto projectile : projectiles) {
        if (projectile->isActive()) {
            int projScreenX = projectile->getX() - offsetX;
            if (projScreenX >= 0 && projScreenX < 80) {
                mvprintw(projectile->getY(), projScreenX, "%s", projectile->getSymbol().c_str());
            }
        }
    }

    refresh();
}