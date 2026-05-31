#include "../../include/SpawnSystem.h"
#include "../../include/FireEnemy.h"
#include <cstdlib>

void SpawnSystem::spawnEnemies(std::vector<Enemy*> &enemies, int amount) {
    for(int i = 0; i < amount; i++) {
        int x = rand() % 100 + 30; 
        int y = 5; 
        if (rand() % 3 == 0) {
            enemies.push_back(new FireEnemy(x, y));
        } else {
            enemies.push_back(new Enemy(x, y));
        }
    }
}

void SpawnSystem::spawnFood(std::vector<Food*> &foods, int amount) {
    for(int i = 0; i < amount; i++) {
        int x = rand() % 100 + 30;
        int y = 5; 
        foods.push_back(new Food(x, y));
    }
}
