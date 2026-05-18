#include "../../include/Game.h"
#include "../../include/LevelManager.h" 
#include "../../include/CollisionSystem.h"
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include "../../include/GameOverScreen.h"

// ======================================
// HILOS OPTIMIZADOS (Auto-Limpieza)
// ======================================
void* enemyThreadFunction(void* arg) {
    Enemy* enemy = (Enemy*)arg;
    
    // El hilo SOLO corre mientras el enemigo tenga vida.
    // Al morir, el bucle se rompe y el hilo se cierra limpiamente.
    while(enemy->isActive()) { 
        enemy->update();
        usleep(100000);
    }
    return NULL;
}

Game::Game() {
    running = true;
    currentLevel = 1;
}

void Game::init() {
    threadManager.init();

    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    player = new Player(10, 10);
    loadLevel("assets/levels/level1.txt");

    // Inicializamos el nivel con 3 enemigos distribuidos
    for(int i = 0; i < 3; i++) {
        Enemy* e = new Enemy(30 + (i * 20), 5);
        enemies.push_back(e);
        
        pthread_t enemyThread;
        pthread_create(&enemyThread, NULL, enemyThreadFunction, e);
        pthread_detach(enemyThread); // liberar recursos
    }
}

void Game::loadLevel(std::string levelPath) {
    LevelManager levelManager;
    int levelNumber = (levelPath.find("level2") != std::string::npos) ? 2 : 1;
    levelManager.loadLevel(levelNumber, &map);
}

void Game::processInput() {
    int ch = inputManager.getInput();
    if (ch == ERR) return;

    switch(ch) {
        case 'a': case 'A': player->moveLeft(); break;
        case 'd': case 'D': player->moveRight(); break;
        case 'w': case 'W': player->jump(); break;
        
        case 'j': case 'J':
            player->inhale();
            for (auto enemy : enemies) {
                if (enemy->isActive()) {
                    int distance = abs(player->getX() - enemy->getX());
                    if (distance <= 5 && player->getY() == enemy->getY()) {
                        enemy->takeDamage(1); 
                        player->addScore(100);
                    }
                }
            }
            break;

        case 'h': case 'H': player->stopInhaling(); break;
        
        case 'k': case 'K':
            projectiles.push_back(new Projectile(player->getX() + 8, player->getY()));
            break;

        case 'q': case 'Q': running = false; break;
    }
}

void Game::update() {
    player->update();
    gravitySystem.applyGravity(player, &map);

    int activeEnemiesCount = 0; //

    // 1. Actualizar Enemigos y contar los sobrevivientes
    for(auto enemy : enemies) {
        if (enemy->isActive()) {
            activeEnemiesCount++; // Suma al censo de enemigos vivos

            enemyAI.updateEnemy(enemy, player);
            gravitySystem.applyGravity(enemy, &map); 
            
            if (CollisionSystem::checkAABB(
                    player->getX(), player->getY(), player->getWidth(), player->getHeight(),
                    enemy->getX(), enemy->getY(), enemy->getWidth(), enemy->getHeight()
                )) {
                player->takeDamage(1);
            }
        }
    }

    // ======================================
    // 2. SISTEMA DE RESPAWN DINÁMICO
    // ======================================
    // Si mataste enemigos generamos reemplazos
    if (activeEnemiesCount < 3) {
        int spawnX = player->getX() + 40 + (rand() % 20); 
        if (spawnX >= map.getWidth() - 10) {
            spawnX = map.getWidth() - 15;
        }

        Enemy* newEnemy = new Enemy(spawnX, 5);
        enemies.push_back(newEnemy);

        pthread_t enemyThread;
        pthread_create(&enemyThread, NULL, enemyThreadFunction, newEnemy);
        pthread_detach(enemyThread); 
    }

    // 3. Actualizar Proyectiles
    for (auto projectile : projectiles) {
        if (projectile->isActive()) {
            projectile->update(); 
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

    // ======================================
    // GAME OVER Y CAMBIO DE NIVEL
    // ======================================
    if (player->getHealth() <= 0) {
        nodelay(stdscr, FALSE); 
        
        GameOverScreen gameOverScreen;
        gameOverScreen.show(player->getScore()); 
        running = false; 
        return;
    }

    if(player->getX() >= map.getWidth() - 5) {
        currentLevel++;
        if(currentLevel == 2) {
            loadLevel("assets/levels/level2.txt");
            player->setX(5);
        }
    }

}

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
    nodelay(stdscr, FALSE);
}

bool Game::isRunning() { return running; }