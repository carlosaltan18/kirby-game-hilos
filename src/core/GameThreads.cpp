#include "GameInternals.h"
#include <unistd.h>

struct EnemyThreadData {
    Enemy* enemy;
    pthread_mutex_t* mutex;
};

// Cada proyectil tambien recibe el mutex porque se mueve en paralelo al update.
struct ProjectileThreadData {
    Projectile* projectile;
    pthread_mutex_t* mutex;
};

static void* enemyThreadFunction(void* arg) {
    EnemyThreadData* data = (EnemyThreadData*)arg;

    while(data->enemy->isActive()) {
        // El hilo toca posicion/estado del enemigo, por eso toma el mutex global.
        pthread_mutex_lock(data->mutex);
        if (data->enemy->isActive()) {
            data->enemy->update();
        }
        pthread_mutex_unlock(data->mutex);
        usleep(100000);
    }

    delete data;
    return NULL;
}

static void* projectileThreadFunction(void* arg) {
    ProjectileThreadData* data = (ProjectileThreadData*)arg;

    while(data->projectile->isActive()) {
        // El update principal revisa colisiones mientras este hilo mueve el ataque.
        pthread_mutex_lock(data->mutex);
        if (data->projectile->isActive()) {
            data->projectile->update();
        }
        pthread_mutex_unlock(data->mutex);
        usleep(50000);
    }

    delete data;
    return NULL;
}

void createEnemyThread(Enemy* enemy, pthread_mutex_t* mutex) {
    EnemyThreadData* data = new EnemyThreadData{enemy, mutex};
    pthread_t enemyThread;
    pthread_create(&enemyThread, NULL, enemyThreadFunction, data);
    // Se separa porque el juego no espera individualmente a cada enemigo.
    pthread_detach(enemyThread);
}

void createProjectileThread(Projectile* projectile, pthread_mutex_t* mutex) {
    ProjectileThreadData* data = new ProjectileThreadData{projectile, mutex};
    pthread_t projectileThread;
    pthread_create(&projectileThread, NULL, projectileThreadFunction, data);
    pthread_detach(projectileThread);
}

void* Game::playerThreadEntry(void* arg) {
    Game* game = (Game*)arg;

    while(true) {
        // Input y modo computadora modifican a Kirby y pueden crear proyectiles.
        pthread_mutex_lock(&game->threadManager.gameMutex);
        if (!game->playerThreadActive || !game->running) {
            pthread_mutex_unlock(&game->threadManager.gameMutex);
            break;
        }
        game->processInput();
        pthread_mutex_unlock(&game->threadManager.gameMutex);
        usleep(45000);
    }

    return NULL;
}

void* Game::eventThreadEntry(void* arg) {
    Game* game = (Game*)arg;

    while(true) {
        // El semaforo evita que este hilo haga polling constante.
        sem_wait(&game->threadManager.eventSemaphore);

        pthread_mutex_lock(&game->threadManager.gameMutex);
        if (!game->eventThreadActive || !game->running) {
            pthread_mutex_unlock(&game->threadManager.gameMutex);
            break;
        }

        if (game->currentLevel < 3 && countActiveFoods(game->foods) < 3) {
            spawnFoodsOnGround(game->foods, game->map, 2);
            game->logEvent("Aparecieron nuevos items en el escenario.");
        }

        pthread_mutex_unlock(&game->threadManager.gameMutex);
    }

    return NULL;
}
