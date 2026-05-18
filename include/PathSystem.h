#ifndef PATH_SYSTEM_H
#define PATH_SYSTEM_H

#include "Enemy.h"
#include "Player.h"

class PathSystem {

public:

    PathSystem();

    void moveTowardsTarget(
        Enemy* enemy,
        int targetX
    );

    void patrolMovement(
        Enemy* enemy,
        int leftLimit,
        int rightLimit
    );

    int calculateDistance(
        Enemy* enemy,
        Player* player
    );
};

#endif