#include "../../include/Renderer.h"
#include <ncurses.h>

Renderer::Renderer() {}

void Renderer::render(
    TileMap &map,
    Camera &camera,
    Player &player,
    std::vector<Enemy*> &enemies,
    std::vector<Projectile*> &projectiles
) {
    // 1. Limpiar el buffer
    erase(); 
    // Desplazamiento de la cámara para el scroll horizontal
    int offsetX = camera.getOffsetX();
    std::vector<std::string>& grid = map.getGrid();
    // Renderizar el mapa (
    for (int y = 0; y < map.getHeight(); y++) {
        std::string visibleRow = "";
        if (offsetX < (int)grid[y].length()) {
            visibleRow = grid[y].substr(offsetX, 80); 
        }
        mvprintw(y, 0, "%s", visibleRow.c_str());
    }

    // Renderizar al Jugador
    // Calcular la posición del jugador en la pantalla
    int playerScreenX = player.getX() - offsetX;
        if (playerScreenX >= 0 && playerScreenX < 80) {
        mvprintw(player.getY(), playerScreenX, "%s", player.getSymbol().c_str());
    }
    for (auto enemy : enemies) {
        if (enemy->isActive()) { 
            int enemyScreenX = enemy->getX() - offsetX;
            // Comprobar frustum culling horizontal 
            if (enemyScreenX >= 0 && enemyScreenX < 80) {
                mvprintw(enemy->getY(), enemyScreenX, "%s", enemy->getSymbol().c_str());
            }
        }
    }

    // Renderizar Proyectiles
    for (auto projectile : projectiles) {
        if (projectile->isActive()) { // Verificar que el proyectil no haya colisionado
            int projScreenX = projectile->getX() - offsetX;            
            if (projScreenX >= 0 && projScreenX < 80) {
                mvprintw(projectile->getY(), projScreenX, "%s", projectile->getSymbol().c_str());
            }
        }
    }
}