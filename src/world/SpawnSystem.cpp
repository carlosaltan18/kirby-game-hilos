#include "../../include/SpawnSystem.h"

#include <cstdlib>

void SpawnSystem::spawnEnemies(
    std::vector<Enemy*> &enemies,
    int amount
) {

    for(int i=0; i<amount; i++) {

        int x = rand() % 150 + 20;

        int y = 10;

        enemies.push_back(
            new Enemy(x,y)
        );
    }
}

void SpawnSystem::spawnFood(
    std::vector<Food*> &foods,
    int amount
) {

    for(int i=0; i<amount; i++) {

        int x = rand() % 150 + 20;

        int y = 8;

        foods.push_back(
            new Food(x,y)
        );
    }
}