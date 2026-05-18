#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "TileMap.h"
#include "Camera.h"

#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"

class Renderer {

public:

    Renderer();

    void render(
        TileMap &map,
        Camera &camera,
        Player &player,
        std::vector<Enemy*> &enemies,
        std::vector<Projectile*> &projectiles
    );
};

#endif