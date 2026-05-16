#ifndef SPAWN_SYSTEM_H
#define SPAWN_SYSTEM_H

#include <vector>

#include "Enemy.h"
#include "Food.h"

class SpawnSystem {

public:

    void spawnEnemies(
        std::vector<Enemy*> &enemies,
        int amount
    );

    void spawnFood(
        std::vector<Food*> &foods,
        int amount
    );
};

#endif