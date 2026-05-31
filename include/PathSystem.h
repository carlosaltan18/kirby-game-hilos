#ifndef PATH_SYSTEM_H
#define PATH_SYSTEM_H

#include "Enemy.h"
#include "Player.h"

// Rutinas de movimiento para enemigos normales. Mantiene memoria de patrulla
// por enemigo en el .cpp para que cada uno tenga su propio recorrido.
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
