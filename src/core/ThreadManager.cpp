#include "../../include/ThreadManager.h"

ThreadManager::ThreadManager() {}

void ThreadManager::init() {
    // Un solo mutex protege el estado compartido de Game: entidades, mapa y log.
    pthread_mutex_init(
        &gameMutex,
        NULL
    );

    // El semaforo arranca en cero para que el hilo de eventos espere dormido.
    sem_init(
        &eventSemaphore,
        0,
        0
    );
}

void ThreadManager::destroy() {
    // Se destruyen al final de la partida, cuando los hilos principales ya cerraron.
    pthread_mutex_destroy(
        &gameMutex
    );

    sem_destroy(
        &eventSemaphore
    );
}
