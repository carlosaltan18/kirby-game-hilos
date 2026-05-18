#ifndef ENEMY_AI_H
#define ENEMY_AI_H

#include "Enemy.h"
#include "Player.h"
#include "PathSystem.h"

enum EnemyState {

    PATROL,
    CHASE
};

class EnemyAI {

private:

    EnemyState currentState;

    PathSystem pathSystem;

public:

    EnemyAI();

    void updateEnemy(
        Enemy* enemy,
        Player* player
    );
};

#endif