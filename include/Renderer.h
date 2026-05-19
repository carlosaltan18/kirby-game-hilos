#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "TileMap.h"
#include "Camera.h"

#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Food.h"
#include "AnimationSystem.h"

class Renderer {

private:

    AnimationSystem animationSystem;

public:

    Renderer();

    void render(
        TileMap &map,
        Camera &camera,
        Player &player,
        std::vector<Enemy*> &enemies,
        std::vector<Projectile*> &projectiles,
        std::vector<Food*> &foods
    );
};

#endif
