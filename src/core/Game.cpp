#include "../../include/Game.h"
#include "../../include/LevelManager.h" 
#include "../../include/CollisionSystem.h"
#include <ncurses.h>
#include <unistd.h>

// Función para actualizar enemigos en un hilo separado
void* enemyThreadFunction(void* arg) {
    Enemy* enemy = (Enemy*)arg;
    while(true) {
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

    // Evitamos que getch() bloquee el hilo principal!
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    player = new Player(10, 10);
    loadLevel("assets/levels/level1.txt");

    enemies.push_back(new Enemy(40, 5));
    enemies.push_back(new Enemy(70, 5));
    
    for(auto enemy : enemies) {
        pthread_t enemyThread;
        pthread_create(&enemyThread, NULL, enemyThreadFunction, enemy);
    }
}

void Game::loadLevel(std::string levelPath) {
    LevelManager levelManager;
    int levelNumber = (levelPath.find("level2") != std::string::npos) ? 2 : 1;
    levelManager.loadLevel(levelNumber, &map);
}

void Game::processInput() {
    int ch = inputManager.getInput();
    
    if (ch == ERR) return;//El juego continua sin esperar input

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
    // Kirby actualiza su propio salto y la gravedad chequea el piso
    player->update();
    gravitySystem.applyGravity(player, &map);

    // Enemigos actualizan su IA y también son afectados por la gravedad
    for(auto enemy : enemies) {
        if (enemy->isActive()) {
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

    // Lanzamos proyectiles y chequeamos colisiones con enemigos
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
}

bool Game::isRunning() { return running; }