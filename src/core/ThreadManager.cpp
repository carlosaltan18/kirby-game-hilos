#include "../../include/ThreadManager.h"

ThreadManager::ThreadManager() {}

//Inicializa el mutex para proteger recursos compartidos
void ThreadManager::init() {

    pthread_mutex_init(
        &gameMutex,
        NULL
    );
}

void ThreadManager::destroy() {

    pthread_mutex_destroy(
        &gameMutex
    );
}