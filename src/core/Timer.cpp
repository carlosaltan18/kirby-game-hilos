#include "../../include/Timer.h"

Timer::Timer() { isRunning = false; }

void Timer::start() {
    startTime = std::chrono::steady_clock::now();
    isRunning = true;
}

void Timer::stop() {
    isRunning = false;
}

int Timer::getElapsedSeconds() {
    if (!isRunning) return 0;
    auto currentTime = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
}