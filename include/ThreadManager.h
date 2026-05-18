#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include <pthread.h>

class ThreadManager {

public:

    pthread_mutex_t gameMutex;

    ThreadManager();

    void init();

    void destroy();
};

#endif