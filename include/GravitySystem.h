#ifndef GRAVITY_SYSTEM_H
#define GRAVITY_SYSTEM_H

#include "Character.h"
#include "TileMap.h"

class GravitySystem {
public:
    void applyGravity(Character* character, TileMap* map);
};

#endif