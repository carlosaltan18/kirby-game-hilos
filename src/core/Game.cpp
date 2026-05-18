#include "../../include/Game.h"

#include <ncurses.h>

#include <unistd.h>

// ======================================
// THREAD FUNCTIONS
// ======================================

void* enemyThreadFunction(
    void* arg
) {

    Enemy* enemy =
        (Enemy*)arg;

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
// INIT
// ======================================

void Game::init() {

    threadManager.init();

    // ======================================
    // PLAYER
    // ======================================

    player = new Player(
        10,
        10
    );

    // ======================================
    // MAPA
    // ======================================

    loadLevel(
        "assets/levels/level1.txt"
    );

    // ======================================
    // ENEMIGOS
    // ======================================

    enemies.push_back(
        new Enemy(40,10)
    );

    enemies.push_back(
        new Enemy(70,10)
    );

    // ======================================
    // THREADS ENEMIGOS
    // ======================================

    for(auto enemy : enemies) {

        pthread_t enemyThread;

        pthread_create(
            &enemyThread,
            NULL,
            enemyThreadFunction,
            enemy
        );
    }
}

// ======================================
// LOAD LEVEL
// ======================================

void Game::loadLevel(
    std::string levelPath
) {

    map.load(levelPath);
}

// ======================================
// INPUT
// ======================================

void Game::processInput() {

    int ch =
        inputManager.getInput();

    switch(ch) {

        case 'a':

            player->moveLeft();
            break;

        case 'd':

            player->moveRight();
            break;

        case 'w':

            player->jump();
            break;

        case 'q':

            running = false;
            break;
    }
}

// ======================================
// UPDATE
// ======================================

void Game::update() {

    // ======================================
    // PLAYER
    // ======================================

    player->update();

    gravitySystem.applyGravity(
        player
    );

    // ======================================
    // ENEMIGOS
    // ======================================

    for(auto enemy : enemies) {

        enemyAI.updateEnemy(
            enemy,
            player
        );
    }

    // ======================================
    // CAMARA
    // ======================================

    camera.update(
        player->getX()
    );

    // ======================================
    // CAMBIO NIVEL
    // ======================================

    if(player->getX()
        >=
        map.getWidth() - 5
    ) {

        currentLevel++;

        if(currentLevel == 2) {

            loadLevel(
                "assets/levels/level2.txt"
            );

            player->setX(5);
        }
    }
}

// ======================================
// RENDER
// ======================================

void Game::render() {

    pthread_mutex_lock(
        &threadManager.gameMutex
    );

    renderer.render(
        map,
        camera,
        *player,
        enemies,
        projectiles
    );

    hud.render(
        player,
        currentLevel
    );

    refresh();

    pthread_mutex_unlock(
        &threadManager.gameMutex
    );
}

// ======================================
// MAIN LOOP
// ======================================

void Game::run() {

    while(running) {

        processInput();

        update();

        render();

        usleep(50000);
    }

    threadManager.destroy();
}

// ======================================
// STATUS
// ======================================

bool Game::isRunning() {

    return running;
}