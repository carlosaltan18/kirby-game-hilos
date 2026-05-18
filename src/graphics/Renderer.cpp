#include "../../include/Renderer.h"

#include <ncurses.h>

// ======================================
// CONSTRUCTOR
// ======================================

Renderer::Renderer() {}

// ======================================
// RENDER ENGINE
// ======================================

void Renderer::render(
    TileMap &map,
    Camera &camera,
    Player &player,
    std::vector<Enemy*> &enemies,
    std::vector<Projectile*> &projectiles
) {

    clear();

    // ======================================
    // MAPA
    // ======================================

    for(int y = 0; y < 24; y++) {

        for(int x = 0; x < 80; x++) {

            char tile = map.getTile(
                x + camera.getOffsetX(),
                y
            );

            mvaddch(
                y,
                x,
                tile
            );
        }
    }

    // ======================================
    // PLAYER
    // ======================================

    mvaddch(
        player.getY(),
        player.getX()
        -
        camera.getOffsetX(),
        player.getSymbol()
    );

    // ======================================
    // ENEMIES
    // ======================================

    for(auto enemy : enemies) {

        if(enemy->isActive()) {

            mvaddch(
                enemy->getY(),
                enemy->getX()
                -
                camera.getOffsetX(),
                enemy->getSymbol()
            );
        }
    }

    // ======================================
    // PROJECTILES
    // ======================================

    for(auto projectile : projectiles) {

        if(projectile->isActive()) {

            mvaddch(
                projectile->getY(),
                projectile->getX()
                -
                camera.getOffsetX(),
                projectile->getSymbol()
            );
        }
    }

    refresh();
}