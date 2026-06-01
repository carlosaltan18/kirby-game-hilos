#ifndef GAME_INTERNALS_H
#define GAME_INTERNALS_H

#include "../../include/Game.h"
#include "../../include/Boss.h"
#include "../../include/Food.h"
#include <pthread.h>
#include <vector>

extern int enemigosCreadosEnNivel;
extern int bossMinionTimer;
extern int contactDamageCooldown;

// Helpers internos del modulo Game. .
void createEnemyThread(Enemy* enemy, pthread_mutex_t* mutex);
void createProjectileThread(Projectile* projectile, pthread_mutex_t* mutex);

// Creacion y limpieza de entidades del mundo.
Enemy* createRandomEnemy(int x, int y);
void deactivateLevelEntities(
    std::vector<Enemy*> &enemies,
    std::vector<Projectile*> &projectiles,
    std::vector<Food*> &foods
);

// Utilidades de nivel, jefe e items compartidas por Game*.cpp.
void showBossScreen();
void placeFoodsOnGround(std::vector<Food*> &foods, TileMap &map);
int countActiveFoods(std::vector<Food*> &foods);
void spawnFoodsOnGround(std::vector<Food*> &foods, TileMap &map, int amount);
Boss* getActiveBoss(std::vector<Enemy*> &enemies);
void resetPlayerAfterFall(Player* player, int currentLevel);

#endif
