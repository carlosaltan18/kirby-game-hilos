#include "GameInternals.h"
#include <cstdlib>
#include <ncurses.h>

void Game::logEvent(const std::string& message) {
    eventLog.push_back(message);
    // Mantiene el HUD corto; demasiadas lineas tapan el area jugable.
    if (eventLog.size() > 8) {
        eventLog.erase(eventLog.begin());
    }
}
// Cuenta enemigos activos para decidir si se puede avanzar de nivel o no.
int Game::countActiveEnemies() {
    int activeEnemies = 0;

    for (auto enemy : enemies) {
        if (enemy->isActive()) {
            activeEnemies++;
        }
    }

    return activeEnemies;
}
// Cuenta proyectiles activos para limitar la cantidad que el jugador puede tener en pantalla.
int Game::countActiveProjectiles() {
    int activeProjectiles = 0;

    for (auto projectile : projectiles) {
        if (projectile->isActive()) {
            activeProjectiles++;
        }
    }

    return activeProjectiles;
}
// Dispara un proyectil si el jugador tiene una habilidad copiada. El proyectil hereda
// la habilidad y direccion de Kirby en el momento del disparo, 
// lo que permite usarlo para atacar mientras se esta moviendo o incluso cambiar de direccion justo despues de disparar.
void Game::fireProjectile() {
    if (!player->hasSpecialAbility()) {
        logEvent("Kirby intento atacar, pero no tiene habilidad copiada.");
        return;
    }

    // El proyectil hereda habilidad y direccion en el momento exacto del disparo.
    KirbyAbility ability = player->getAbility();
    int direction = player->getProjectileDirection();
    int spawnX = direction > 0 ? player->getX() + player->getWidth() : player->getX() - 4;

    if (spawnX < 0) {
        spawnX = 0;
    }

    Projectile* projectile = new Projectile(spawnX, player->getY(), direction, ability);
    projectiles.push_back(projectile);
    createProjectileThread(projectile, &threadManager.gameMutex);

    logEvent("Kirby uso la habilidad " + player->getAbilityName() + ".");
}
// Intenta absorber un enemigo cercano. El rango de absorcion es pequeño para que el jugador
void Game::tryAbsorbEnemy() {
    player->inhale();

    for (auto enemy : enemies) {
        if (enemy->isActive() && enemy->canBeAbsorbed()) {
            int distance = abs(player->getX() - enemy->getX());
            // Absorber respeta la direccion visual de Kirby para no capturar por atras.
            bool enemyInFront = player->isFacingRight()
                ? enemy->getX() >= player->getX()
                : enemy->getX() <= player->getX();

            if (enemyInFront && distance <= 8 && abs(player->getY() - enemy->getY()) <= 2) {
                std::string abilityName = enemy->getCopiedAbilityName();
                enemy->takeDamage(enemy->getHealth());
                player->grantAbility(enemy->getCopiedAbility());
                player->addScore(150);
                logEvent("Kirby absorbio un enemigo y copio " + abilityName + ".");
                return;
            }
        }
    }

    logEvent("Kirby intento absorber, pero no habia enemigo cerca.");
}
// El modo computadora tiene una logica simple pero con varios comportamientos posibles para no ser tan predecible
// Elige entre avanzar, buscar comida, acercarse a enemigo o retirarse, y tiene timers para no repetir exactamente la misma accion cada frame.
void Game::processComputerInput() {
    // El modo computadora usa timers cortos para no repetir exactamente
    if (actionCooldown > 0) {
        actionCooldown--;
    }
    if (aiDecisionTimer > 0) {
        aiDecisionTimer--;
    }

    Enemy* nearestEnemy = nullptr;
    int nearestDistance = 9999;
    Food* nearestFood = nullptr;
    int nearestFoodDistance = 9999;
    int roll = rand() % 100;

    for (auto enemy : enemies) {
        if (!enemy->isActive()) {
            continue;
        }

        int distance = abs(enemy->getX() - player->getX());
        if (distance < nearestDistance) {
            nearestEnemy = enemy;
            nearestDistance = distance;
        }
    }

    for (auto food : foods) {
        if (!food->isActive()) {
            continue;
        }

        int distance = abs(food->getX() - player->getX());
        if (distance < nearestFoodDistance) {
            nearestFood = food;
            nearestFoodDistance = distance;
        }
    }

    if (aiDecisionTimer == 0) {
        // Cambia de intencion cada cierto tiempo: avanzar, buscar comida,
        // acercarse a enemigo o retirarse.
        aiBehavior = rand() % 5;
        aiDecisionTimer = 16 + (rand() % 30);

        if (aiBehavior == 2 && nearestFood != nullptr) {
            aiTargetX = nearestFood->getX();
        } else if (aiBehavior == 1 && nearestEnemy != nullptr) {
            aiTargetX = nearestEnemy->getX();
        } else {
            int offset = 20 + (rand() % 55);
            aiTargetX = player->getX() + offset;
            if (roll < 20 && player->getX() > 25) {
                aiTargetX = player->getX() - (10 + (rand() % 25));
            }
        }
    }

    if (nearestEnemy != nullptr && nearestDistance <= 9) {
        bool shouldRetreat = aiBehavior == 3 || roll < 22;

        // De cerca decide entre retirarse, absorber o usar la habilidad copiada.
        if (shouldRetreat) {
            if (nearestEnemy->getX() < player->getX()) {
                player->moveRight();
            } else {
                player->moveLeft();
            }
            if (roll > 45) {
                player->jump();
            }
        } else if (nearestEnemy->getX() < player->getX()) {
            player->moveLeft();
        } else {
            player->moveRight();
        }

        if (actionCooldown == 0) {
            if (!player->hasSpecialAbility() || roll < 35) {
                tryAbsorbEnemy();
            } else {
                fireProjectile();
            }
            actionCooldown = 8 + (rand() % 12);
        }

        if (roll > 75) {
            player->jump();
        }
        return;
    }

    if (nearestEnemy != nullptr && nearestDistance <= 18 && roll < 40) {
        player->jump();
    }

    if (aiBehavior == 2 && nearestFood != nullptr) {
        aiTargetX = nearestFood->getX();
    }

    if (player->getX() > aiTargetX + 2 && player->getX() > 8) {
        player->moveLeft();
    } else if (player->getX() < aiTargetX - 2) {
        player->moveRight();
    } else {
        aiDecisionTimer = 0;
    }

    if (roll < 14 || (aiBehavior == 4 && roll < 35)) {
        player->jump();
    }

    if (nearestEnemy != nullptr && player->hasSpecialAbility() &&
        nearestDistance <= 28 && actionCooldown == 0 && roll > 45) {
        if (nearestEnemy->getX() < player->getX()) {
            player->moveLeft();
        } else {
            player->moveRight();
        }

        fireProjectile();
        actionCooldown = 10 + (rand() % 10);
    }
}
// metodo que procesa la entrada del jugador
void Game::processInput() {
    int ch = inputManager.getInput();

    if (computerMode) {
        // En modo automatico solo Q queda como escape manual del jugador.
        if (ch == 'q' || ch == 'Q') {
            running = false;
            return;
        }

        processComputerInput();
        return;
    }

    if (ch == ERR) return;

    switch(ch) {
        case 'a': case 'A': player->moveLeft(); break;
        case 'd': case 'D': player->moveRight(); break;
        case 'w': case 'W': player->jump(); break;

        case 'j': case 'J': tryAbsorbEnemy(); break;

        case 'h': case 'H': player->stopInhaling(); break;

        case 'k': case 'K': fireProjectile(); break;

        case 'q': case 'Q': running = false; break;
    }
}
