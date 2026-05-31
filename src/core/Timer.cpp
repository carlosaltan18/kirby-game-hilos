#include "../../include/Timer.h"

Timer::Timer() { isRunning = false; }

void Timer::start() {
    // Guarda el instante inicial usando reloj monotono.
    startTime = std::chrono::steady_clock::now();
    isRunning = true;
}

void Timer::stop() {
    // Detenerlo hace que la lectura vuelva a cero.
    isRunning = false;
}

int Timer::getElapsedSeconds() {
    if (!isRunning) return 0;
    // Convierte la diferencia a segundos enteros para mostrar/usar facilmente.
    auto currentTime = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
}
