#include "../../include/Game.h"
#include "../../include/LevelManager.h" 
#include "../../include/CollisionSystem.h"
#include <ncurses.h>
#include <unistd.h>

void* enemyThreadFunction(void* arg) {
    Enemy* enemy = (Enemy*)arg;
    while(true) {
        enemy->update();
        usleep(100000);
    }
    return NULL;
}

// ======================================
// CONSTRUCTOR
// ======================================
Game::Game() {
    running = true;
    currentLevel = 1;
}

// ======================================
// INICIALIZACIÓN
// ======================================
void Game::init() {
    threadManager.init();

    player = new Player(10, 10);

    loadLevel("assets/levels/level1.txt");

    enemies.push_back(new Enemy(40, 10));
    enemies.push_back(new Enemy(70, 10));
    for(auto enemy : enemies) {
        pthread_t enemyThread;
        pthread_create(&enemyThread, NULL, enemyThreadFunction, enemy);
    }
}

// ======================================
// LOAD LEVEL 
// ======================================
void Game::loadLevel(std::string levelPath) {
    LevelManager levelManager;
    int levelNumber = (levelPath.find("level2") != std::string::npos) ? 2 : 1;
    levelManager.loadLevel(levelNumber, &map);
}

// ======================================
// INPUT DEL JUGADOR
// ======================================
void Game::processInput() {
    int ch = inputManager.getInput();
    switch(ch) {
        case 'a': case 'A':
            player->moveLeft();
            break;
        case 'd': case 'D':
            player->moveRight();
            break;
        case 'w': case 'W':
            player->jump(); 
            break;

        // ======================================
        // J: ABSORBER ENEMIGOS
        // ======================================
        case 'j': case 'J':
            player->inhale();
            for (auto enemy : enemies) {
                if (enemy->isActive()) {
                    int distance = abs(player->getX() - enemy->getX());
                    if (distance <= 5 && player->getY() == enemy->getY()) {
                        enemy->takeDamage(1); 
                        player->addScore(100);
                        // Aquí podrías cambiar el estado de Kirby a "Con Habilidad"
                    }
                }
            }
            break;

        case 'h': case 'H': 
            player->stopInhaling();
            break;

        // ======================================
        // K: DISPARAR PROYECTIL 
        // ======================================
        case 'k': case 'K':
            // Instancia un proyectil justo enfrente de Kirby
            projectiles.push_back(new Projectile(player->getX() + 8, player->getY()));
            break;

        case 'q': case 'Q':
            running = false;
            break;
    }
}

// ======================================
// ACTUALIZACIÓN LÓGICA 
// ======================================
void Game::update() {
    // 1. Actualizar a Kirby y aplicarle gravedad
    player->update();
    gravitySystem.applyGravity(player, &map);

    // 2. Actualizar la IA de los Enemigos
    for(auto enemy : enemies) {
        if (enemy->isActive()) {
            enemyAI.updateEnemy(enemy, player);
            
            // Colisión directa de Kirby contra Enemigo (Daño)
            if (CollisionSystem::checkAABB(
                    player->getX(), player->getY(), player->getWidth(), player->getHeight(),
                    enemy->getX(), enemy->getY(), enemy->getWidth(), enemy->getHeight()
                )) {
                player->takeDamage(1); // Pierde vida [cite: 66]
            }
        }
    }

    // 3. Actualizar y Mover Proyectiles
    for (auto projectile : projectiles) {
        if (projectile->isActive()) {
            projectile->update(); 
            // Verificar si el proyectil impacta
            for (auto enemy : enemies) {
                if (enemy->isActive() && CollisionSystem::checkAABB(
                        projectile->getX(), projectile->getY(), projectile->getWidth(), projectile->getHeight(),
                        enemy->getX(), enemy->getY(), enemy->getWidth(), enemy->getHeight()
                    )) {
                    enemy->takeDamage(1);   
                    projectile->takeDamage(1);
                    player->addScore(100);     
                }
            }
        }
    }

    camera.update(player->getX());

    if(player->getX() >= map.getWidth() - 5) {
        currentLevel++;
        if(currentLevel == 2) {
            loadLevel("assets/levels/level2.txt");
            player->setX(5);
        }
    }
}

// ======================================
// DIBUJADO DE RENDER 
// ======================================
void Game::render() {
    pthread_mutex_lock(&threadManager.gameMutex);

    renderer.render(map, camera, *player, enemies, projectiles);
    hud.render(player, currentLevel);

    refresh();
    pthread_mutex_unlock(&threadManager.gameMutex);
}

void Game::run() {
    while(running) {
        processInput();
        update();
        render();
        usleep(50000); 
    }
    threadManager.destroy();
}

bool Game::isRunning() {
    return running;
}