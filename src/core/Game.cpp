#include "../../include/Game.h"
#include "../../include/FireEnemy.h"
#include "../../include/LevelManager.h"
#include "GameInternals.h"
#include <cstdlib>
#include <ctime>
#include <ncurses.h>
#include <unistd.h>

int enemigosCreadosEnNivel = 0;
int bossMinionTimer = 0;
int contactDamageCooldown = 0;

Game::Game(bool computerMode) {
    running = true;
    this->computerMode = computerMode;
    playerThreadActive = false;
    eventThreadActive = false;
    restartRequested = false;
    actionCooldown = 0;
    eventSignalCounter = 0;
    aiDecisionTimer = 0;
    aiBehavior = 0;
    aiTargetX = 60;
    currentLevel = 1;
    enemigosCreadosEnNivel = 0;
    bossMinionTimer = 0;
    contactDamageCooldown = 0;
}

void Game::init() {
    // La partida arma primero los sistemas compartidos y luego abre hilos.
    // Asi los threads arrancan con mapa, jugador y mutex ya preparados.
    threadManager.init();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    srand((unsigned int)time(NULL) ^ (unsigned int)clock());
    logEvent(computerMode ? "Modo 2: la computadora controla a Kirby." : "Modo 1: jugador controla a Kirby.");

    player = new Player(10, 10);
    loadLevel("assets/levels/level1.txt");

    spawnFoodsOnGround(foods, map, 5);

    enemigosCreadosEnNivel = 3;
    for(int i = 0; i < 3; i++) {
        Enemy* enemy = nullptr;
        // Garantiza que el jugador vea al menos un enemigo de fuego temprano.
        if (i == 1) {
            enemy = new FireEnemy(30 + (i * 20), 5);
        } else {
            enemy = createRandomEnemy(30 + (i * 20), 5);
        }
        enemies.push_back(enemy);
        createEnemyThread(enemy, &threadManager.gameMutex);
    }

    playerThreadActive = true;
    eventThreadActive = true;
    pthread_create(&playerThread, NULL, Game::playerThreadEntry, this);
    pthread_create(&eventThread, NULL, Game::eventThreadEntry, this);
}

void Game::loadLevel(std::string levelPath) {
    LevelManager levelManager;
    int levelNumber = 1;

    // El resto del juego habla en rutas logicas; LevelManager recibe numero.
    if (levelPath.find("boss") != std::string::npos) {
        levelNumber = 3;
    } else if (levelPath.find("level2") != std::string::npos) {
        levelNumber = 2;
    }

    levelManager.loadLevel(levelNumber, &map);
    enemigosCreadosEnNivel = 0;
}

bool Game::run() {
    while(running) {
        // Update se protege porque toca casi todo el estado compartido.
        pthread_mutex_lock(&threadManager.gameMutex);
        update();
        bool shouldRender = running;
        pthread_mutex_unlock(&threadManager.gameMutex);

        if (!shouldRender) {
            break;
        }

        render();
        usleep(50000);
    }

    playerThreadActive = false;
    eventThreadActive = false;
    // Despierta el hilo de eventos si estaba bloqueado en sem_wait.
    sem_post(&threadManager.eventSemaphore);

    pthread_mutex_lock(&threadManager.gameMutex);
    deactivateLevelEntities(enemies, projectiles, foods);
    pthread_mutex_unlock(&threadManager.gameMutex);

    pthread_join(playerThread, NULL);
    pthread_join(eventThread, NULL);
    usleep(150000);
    threadManager.destroy();
    nodelay(stdscr, FALSE);

    return restartRequested;
}

bool Game::isRunning() { return running; }
