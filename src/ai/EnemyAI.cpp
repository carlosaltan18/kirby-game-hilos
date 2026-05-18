#include "../../include/EnemyAI.h"

// ======================================
// CONSTRUCTOR
// ======================================

EnemyAI::EnemyAI() {

    currentState = PATROL;
}

// ======================================
// UPDATE ENEMY AI
// ======================================

void EnemyAI::updateEnemy(
    Enemy* enemy,
    Player* player
) {

    // ======================================
    // CALCULAR DISTANCIA
    // ======================================

    int distance =
        pathSystem.calculateDistance(
            enemy,
            player
        );

    // ======================================
    // DETECTAR JUGADOR
    // ======================================

    if(distance < 15) {

        currentState = CHASE;
    }

    else {

        currentState = PATROL;
    }

    // ======================================
    // EJECUTAR IA
    // ======================================

    switch(currentState) {

        // ==================================
        // PATRULLAR
        // ==================================

        case PATROL:

            pathSystem.patrolMovement(
                enemy,
                5,
                150
            );

            break;

        // ==================================
        // PERSEGUIR JUGADOR
        // ==================================

        case CHASE:

            pathSystem.moveTowardsTarget(
                enemy,
                player->getX()
            );

            break;
    }
}