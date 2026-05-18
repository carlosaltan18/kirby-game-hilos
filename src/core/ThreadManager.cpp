#include "../../include/ThreadManager.h"

ThreadManager::ThreadManager() {}

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