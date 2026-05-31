#ifndef ENEMY_AI_H
#define ENEMY_AI_H

#include "Enemy.h"
#include "Player.h"
#include "PathSystem.h"

enum EnemyState {

    PATROL,
    CHASE
};

// Decide si un enemigo patrulla o persigue. El movimiento exacto lo delega en
// PathSystem para mantener separada la decision del desplazamiento.
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
