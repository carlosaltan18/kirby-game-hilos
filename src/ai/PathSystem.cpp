#include "../../include/PathSystem.h"
#include <cmath>
#include <map>

// ======================================
// CEREBROS INDEPENDIENTES PARA ENEMIGOS
// ======================================
// Guarda el estado individual de cada enemigo
std::map<Enemy*, int> startPositions;
std::map<Enemy*, bool> movingLeftStates;

PathSystem::PathSystem() {}

// ======================================
// Perseguir el objetivo
// ======================================
void PathSystem::moveTowardsTarget(Enemy* enemy, int targetX) {
    if(enemy->getX() > targetX) {
        enemy->setX(enemy->getX() - 1);
    } else if(enemy->getX() < targetX) {
        enemy->setX(enemy->getX() + 1);
    }
}

// ======================================
// Limites dinamicos de patrulla
// ======================================
void PathSystem::patrolMovement(Enemy* enemy, int leftLimit, int rightLimit) {
    //registramos dónde nació
    if (startPositions.find(enemy) == startPositions.end()) {
        startPositions[enemy] = enemy->getX();
        movingLeftStates[enemy] = true; // Inicia moviéndose a la izquierda
    }

    int startX = startPositions[enemy];
    int patrolRadius = 10; // cuantos pasos camina dependiendo de su posición inicial

    // Mueve al enemigo basado en su propia variable de estado
    if(movingLeftStates[enemy]) {
        enemy->setX(enemy->getX() - 1);
        if(enemy->getX() <= startX - patrolRadius) {
            movingLeftStates[enemy] = false; // Da la vuelta hacia la derecha
        }
    } else {
        enemy->setX(enemy->getX() + 1);
        if(enemy->getX() >= startX + patrolRadius) {
            movingLeftStates[enemy] = true; // Da la vuelta hacia la izquierda
        }
    }
}

int PathSystem::calculateDistance(Enemy* enemy, Player* player) {
    return abs(enemy->getX() - player->getX());
}