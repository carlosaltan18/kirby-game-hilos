#include "../../include/EnemyAI.h"
#include <cmath>

EnemyAI::EnemyAI() {
    currentState = PATROL;
}

void EnemyAI::updateEnemy(Enemy* enemy, Player* player) {

    // ======================================
    // Calcular las distancias horizontal y vertical entre el enemigo y el jugador
    // ======================================
    int distanceX = pathSystem.calculateDistance(enemy, player);
    int distanceY = abs(enemy->getY() - player->getY()); // Diferencia de altura entre el enemigo y el jugador


    // Solo persigue si Kirby está cerca horizontalmente Y además están casi al mismo nivel del suelo
    if(distanceX < 15 && distanceY <= 3) {
        currentState = CHASE;
    } else {
        currentState = PATROL;
    }

    // ======================================
    // Llamar a la función de movimiento correspondiente según el estado actual
    // ======================================
    switch(currentState) {
        case PATROL:
            pathSystem.patrolMovement(enemy, 0, 0);
            break;

        case CHASE:
            pathSystem.moveTowardsTarget(enemy, player->getX());
            break;
    }
}