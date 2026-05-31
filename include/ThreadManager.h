#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include <pthread.h>
#include <semaphore.h>

class ThreadManager {

public:

    pthread_mutex_t gameMutex;
    sem_t eventSemaphore;

    // Constructor liviano; init prepara los recursos reales.
    ThreadManager();

    // Inicializa mutex y semaforo compartidos.
    void init();

    // Libera mutex y semaforo al terminar la partida.
    void destroy();
};

#endif
