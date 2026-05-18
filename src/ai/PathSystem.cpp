#include "../../include/PathSystem.h"

#include <cmath>

// ======================================
// CONSTRUCTOR
// ======================================

PathSystem::PathSystem() {}

// ======================================
// MOVER HACIA OBJETIVO
// ======================================

void PathSystem::moveTowardsTarget(
    Enemy* enemy,
    int targetX
) {

    if(enemy->getX() > targetX) {

        enemy->setX(
            enemy->getX() - 1
        );
    }

    else if(enemy->getX() < targetX) {

        enemy->setX(
            enemy->getX() + 1
        );
    }
}

// ======================================
// PATRULLA
// ======================================

void PathSystem::patrolMovement(
    Enemy* enemy,
    int leftLimit,
    int rightLimit
) {

    static bool movingLeft = true;

    if(movingLeft) {

        enemy->setX(
            enemy->getX() - 1
        );

        if(enemy->getX() <= leftLimit) {

            movingLeft = false;
        }
    }

    else {

        enemy->setX(
            enemy->getX() + 1
        );

        if(enemy->getX() >= rightLimit) {

            movingLeft = true;
        }
    }
}

// ======================================
// DISTANCIA
// ======================================

int PathSystem::calculateDistance(
    Enemy* enemy,
    Player* player
) {

    return abs(
        enemy->getX()
        -
        player->getX()
    );
}