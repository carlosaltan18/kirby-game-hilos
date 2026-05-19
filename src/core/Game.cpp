#include "../../include/Game.h"
#include "../../include/LevelManager.h" 
#include "../../include/CollisionSystem.h"
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include "../../include/GameOverScreen.h"
#include "../../include/SpawnSystem.h"
#include "../../include/Boss.h"
#include "../../include/Food.h"


int enemigosCreadosEnNivel = 0;

void* enemyThreadFunction(void* arg);

void createEnemyThread(Enemy* enemy) {
    pthread_t enemyThread;
    pthread_create(&enemyThread, NULL, enemyThreadFunction, enemy);
    pthread_detach(enemyThread);
}

void deactivateLevelEntities(
    std::vector<Enemy*> &enemies,
    std::vector<Projectile*> &projectiles,
    std::vector<Food*> &foods
) {
    for (auto enemy : enemies) {
        if (enemy->isActive()) {
            enemy->takeDamage(enemy->getHealth());
        }
    }

    for (auto projectile : projectiles) {
        if (projectile->isActive()) {
            projectile->takeDamage(projectile->getHealth());
        }
    }

    for (auto food : foods) {
        if (food->isActive()) {
            food->setActive(false);
        }
    }
}

void showBossScreen() {
    nodelay(stdscr, FALSE);
    clear();

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    const char* title = "!!! JEFE FINAL !!!";
    const char* boss = "Preparate: el jefe aparecio";
    const char* prompt = "Presiona cualquier tecla para entrar";

    mvprintw(maxY / 2 - 3, (maxX - 25) / 2, "=========================");
    mvprintw(maxY / 2 - 2, (maxX - 17) / 2, "%s", title);
    mvprintw(maxY / 2 - 1, (maxX - 25) / 2, "=========================");
    mvprintw(maxY / 2 + 1, (maxX - 28) / 2, "%s", boss);
    mvprintw(maxY / 2 + 4, (maxX - 38) / 2, "%s", prompt);

    refresh();
    flushinp();
    getch();
    nodelay(stdscr, TRUE);
}

int findGroundY(TileMap &map, int x) {
    for (int y = map.getHeight() - 2; y >= 0; y--) {
        if (!map.isSolid(x, y) && map.isSolid(x, y + 1)) {
            return y;
        }
    }

    return -1;
}

void placeFoodsOnGround(std::vector<Food*> &foods, TileMap &map) {
    for (auto food : foods) {
        if (!food->isActive()) {
            continue;
        }

        int x = food->getX();
        if (x >= map.getWidth() - 2) {
            x = map.getWidth() - 3;
        }
        if (x < 1) {
            x = 1;
        }

        int groundY = findGroundY(map, x);
        for (int offset = 1; groundY == -1 && offset < map.getWidth(); offset++) {
            if (x + offset < map.getWidth() - 2) {
                groundY = findGroundY(map, x + offset);
                if (groundY != -1) {
                    x += offset;
                    break;
                }
            }

            if (x - offset > 1) {
                groundY = findGroundY(map, x - offset);
                if (groundY != -1) {
                    x -= offset;
                    break;
                }
            }
        }

        if (groundY == -1) {
            groundY = map.getHeight() > 2 ? map.getHeight() - 2 : 1;
        }

        food->setX(x);
        food->setY(groundY);
    }
}

Boss* getActiveBoss(std::vector<Enemy*> &enemies) {
    for (auto enemy : enemies) {
        Boss* boss = dynamic_cast<Boss*>(enemy);
        if (boss != nullptr && boss->isActive()) {
            return boss;
        }
    }

    return nullptr;
}

//Limpieza de hilos
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

    SpawnSystem spawner;
    spawner.spawnFood(foods, 2);
    placeFoodsOnGround(foods, map);

    // Iniciamos creando 3 enemigos
    enemigosCreadosEnNivel = 3;
    for(int i = 0; i < 3; i++) {
        Enemy* e = new Enemy(30 + (i * 20), 5);
        enemies.push_back(e);
        createEnemyThread(e);
    }
}

void Game::loadLevel(std::string levelPath) {
    LevelManager levelManager;
    int levelNumber = 1;

    if (levelPath.find("boss") != std::string::npos) {
        levelNumber = 3;
    } else if (levelPath.find("level2") != std::string::npos) {
        levelNumber = 2;
    }

    levelManager.loadLevel(levelNumber, &map);
    enemigosCreadosEnNivel = 0;
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

    int activeEnemiesCount = 0;

    // 1. Actualizar Enemigos y contar los sobrevivientes
    for(auto enemy : enemies) {
        if (enemy->isActive()) {
            activeEnemiesCount++; // Suma al censo de enemigos vivos

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
                player->takeDamage(1);
            }
        }
    }

    // RESPAWN DINÁMICO
    // Si mataste enemigos se generanr máximo 5
    if (currentLevel < 3 && activeEnemiesCount < 3 && enemigosCreadosEnNivel < 5) {
        int spawnX = player->getX() + 40 + (rand() % 20);

        if (spawnX >= map.getWidth() - 10) {
            spawnX = map.getWidth() - 15;
        }

        Enemy* newEnemy = new Enemy(spawnX, 5);
        enemies.push_back(newEnemy);
        enemigosCreadosEnNivel++;
        createEnemyThread(newEnemy);
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

    if (currentLevel == 3 && getActiveBoss(enemies) == nullptr) {
        nodelay(stdscr, FALSE);

        GameOverScreen victoryScreen;
        victoryScreen.showVictory(player->getScore());
        running = false;
        return;
    }

    // ======================================
    // COLISIONES CON COMIDA (CURACIÓN)
    // ======================================
    for (auto food : foods) {
        if (food->isActive()) {
            if (CollisionSystem::checkAABB(
                    player->getX(), player->getY(), player->getWidth(), player->getHeight(),
                    food->getX(), food->getY(), food->getWidth(), food->getHeight()
                )) {

                food->setActive(false);
                player->setHealth(player->getHealth() + 1);
                player->addScore(50);
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
            deactivateLevelEntities(enemies, projectiles, foods);
            loadLevel("assets/levels/level2.txt");
            player->setX(5);
            player->setY(10);

            SpawnSystem spawner;
            spawner.spawnFood(foods, 2);
            placeFoodsOnGround(foods, map);
        } else if(currentLevel == 3) {
            deactivateLevelEntities(enemies, projectiles, foods);
            showBossScreen();
            loadLevel("assets/levels/boss.txt");
            player->setX(5);
            player->setY(10);

            Boss* boss = new Boss(55, 16);
            enemies.push_back(boss);
        } else {
            running = false;
        }
    }

}

void Game::render() {
    pthread_mutex_lock(&threadManager.gameMutex);

    renderer.render(map, camera, *player, enemies, projectiles, foods);
    hud.render(player, currentLevel, getActiveBoss(enemies));
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
