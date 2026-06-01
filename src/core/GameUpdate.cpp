#include "GameInternals.h"
#include "../../include/CollisionSystem.h"
#include "../../include/GameOverScreen.h"
#include <cstdlib>
#include <ncurses.h>

void Game::update() {
    player->update();

    // Cada cierto numero de frames despierta el hilo de eventos para items.
    eventSignalCounter++;
    //si el contador llega a 36, se despierta el hilo de eventos y se reinicia el contador. 
    // Esto hace que el hilo de eventos se despierte cada 36 frames, 
    // lo que permite que los items se actualicen y aparezcan en el juego de manera regular.
    if (eventSignalCounter >= 36) {
        sem_post(&threadManager.eventSemaphore);
        eventSignalCounter = 0;
    }
    // El cooldown de daño por contacto evita perder muchas vidas de golpe.4
    if (contactDamageCooldown > 0) {
        contactDamageCooldown--;
    }
    // Validacion de caida: si el jugador esta por debajo del mapa, se resetea a una posicion segura.
    if (player->getY() >= map.getHeight() - player->getHeight()) {
        // Caer por un hueco castiga, pero reposiciona para no quedar atrapado.
        resetPlayerAfterFall(player, currentLevel);
        logEvent("Kirby cayo por un hueco y perdio una vida.");
    }

    gravitySystem.applyGravity(player, &map);

    int activeEnemiesCount = 0;
    // Se actualizan enemigos activos y se valida contacto con el jugador.
    for(auto enemy : enemies) {
        if (enemy->isActive()) {
            activeEnemiesCount++;

            // El jefe tiene patron propio; los demas usan IA y gravedad normal.
            Boss* boss = dynamic_cast<Boss*>(enemy);
            if (boss != nullptr) {
                boss->update();
            } else {
                enemyAI.updateEnemy(enemy, player);
                gravitySystem.applyGravity(enemy, &map);
            }

            if (CollisionSystem::checkAABB(
                    player->getX(), player->getY(), player->getWidth(), player->getHeight(),
                    enemy->getX(), enemy->getY(), enemy->getWidth(), enemy->getHeight()
                )) {
                if (contactDamageCooldown == 0) {
                    player->takeDamage(1);
                    contactDamageCooldown = 20;
                    logEvent("Kirby recibio dano por tocar un enemigo.");
                }
            }
        }
    }

    Boss* activeBoss = getActiveBoss(enemies);
    if (currentLevel == 3 && activeBoss != nullptr) {
        // La pelea se mantiene activa invocando apoyo si quedan pocos enemigos.
        bossMinionTimer++;
        if (bossMinionTimer >= 90 && activeEnemiesCount < 4) {
            int spawnX = activeBoss->getX() > player->getX()
                ? activeBoss->getX() - 18
                : activeBoss->getX() + 18;

            if (spawnX < 2) {
                spawnX = 2;
            }
            if (spawnX > map.getWidth() - 8) {
                spawnX = map.getWidth() - 8;
            }

            Enemy* minion = createRandomEnemy(spawnX, 5);
            // El jefe solo puede invocar enemigos de apoyo durante su pelea, 
            // asi que no se cuenta para el limite de spawn del nivel.
            enemies.push_back(minion);
            createEnemyThread(minion, &threadManager.gameMutex);
            logEvent("El jefe llamo a un enemigo de apoyo.");
            bossMinionTimer = 0;
        }
    } else {
        bossMinionTimer = 0;
    }

    if (currentLevel < 3 && activeEnemiesCount < 3 && enemigosCreadosEnNivel < 5) {
        // En niveles normales se repone algo de presion, con limite por nivel.
        int spawnX = player->getX() + 40 + (rand() % 20);

        if (spawnX >= map.getWidth() - 10) {
            spawnX = map.getWidth() - 15;
        }

        Enemy* newEnemy = createRandomEnemy(spawnX, 5);
        enemies.push_back(newEnemy);
        enemigosCreadosEnNivel++;
        createEnemyThread(newEnemy, &threadManager.gameMutex);
    }
    // Validacion de proyectiles activos: impacto con escenario o enemigos.
    // El jugador no dispara en el nivel del jefe, asi que no se crean nuevos proyectiles.
    // Esto hace que el hilo de proyectiles se mantenga activo para validar impactos, pero sin crear nuevos.
    for (auto projectile : projectiles) {
        if (projectile->isActive()) {
            // El proyectil se mueve en su hilo; aqui solo se valida impacto.
            // El proyectil se destruye al tocar el escenario o salir de los limites del mapa.
            if (projectile->getX() < 0 || projectile->getX() >= map.getWidth() ||
                map.isSolid(projectile->getX(), projectile->getY())) {
                projectile->takeDamage(projectile->getHealth());
                logEvent("Un ataque se deshizo al tocar el escenario.");
                continue;
            }
            // Validacion de impacto con enemigos activos.
            // El proyectil se destruye al tocar un enemigo, pero el enemigo solo recibe dano si el proyectil estaba activo.
            for (auto enemy : enemies) {
                if (enemy->isActive() && CollisionSystem::checkAABB(
                        projectile->getX(), projectile->getY(), projectile->getWidth(), projectile->getHeight(),
                        enemy->getX(), enemy->getY(), enemy->getWidth(), enemy->getHeight()
                    )) {
                    enemy->takeDamage(projectile->getDamage());
                    projectile->takeDamage(1);
                    player->addScore(100);
                    if (enemy->isActive()) {
                        logEvent("Kirby golpeo a un enemigo con su habilidad.");
                    } else {
                        logEvent("Kirby derroto a un enemigo con su habilidad.");
                    }
                    break;
                }
            }
        }
    }

    if (currentLevel == 3 && getActiveBoss(enemies) == nullptr) {
        // En el nivel 3 la victoria real es derrotar al jefe.
        nodelay(stdscr, FALSE);

        GameOverScreen victoryScreen;
        restartRequested = victoryScreen.showVictory(player->getScore());
        running = false;
        return;
    }

    for (auto food : foods) {
        if (food->isActive()) {
            // Los items mezclan recuperacion y score; Food decide cuanto aporta.
            if (CollisionSystem::checkAABB(
                    player->getX(), player->getY(), player->getWidth(), player->getHeight(),
                    food->getX(), food->getY(), food->getWidth(), food->getHeight()
                )) {

                food->setActive(false);
                player->setHealth(player->getHealth() + food->getHealAmount());
                player->addScore(food->getScoreValue());
                logEvent("Kirby recolecto un item del escenario.");
            }
        }
    }

    camera.update(player->getX());

    if (player->getHealth() <= 0) {
        nodelay(stdscr, FALSE);

        GameOverScreen gameOverScreen;
        restartRequested = gameOverScreen.show(player->getScore());
        running = false;
        return;
    }

    if(player->getX() >= map.getWidth() - 5) {
        // Alcanza la meta del nivel, se avanza o termina segun corresponda.
        //
        currentLevel++;
        if(currentLevel == 2) {
            deactivateLevelEntities(enemies, projectiles, foods);
            loadLevel("assets/levels/level2.txt");
            player->setX(5);
            player->setY(10);

            spawnFoodsOnGround(foods, map, 5);
            logEvent("Kirby llego a la meta y entro al nivel 2.");
        } else if(currentLevel == 3) {
            deactivateLevelEntities(enemies, projectiles, foods);
            showBossScreen();
            loadLevel("assets/levels/boss.txt");
            player->setX(5);
            player->setY(10);

            Boss* boss = new Boss(55, 16);
            enemies.push_back(boss);
            logEvent("Kirby llego a la pelea contra el jefe final.");
        } else {
            logEvent("Kirby alcanzo la meta final.");
            running = false;
        }
    }
}
