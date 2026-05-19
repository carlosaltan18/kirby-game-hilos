#ifndef TIMER_H
#define TIMER_H
#include <chrono>

class Timer {
private:
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    bool isRunning;
public:
    Timer();
    void start();
    void stop();
    int getElapsedSeconds();
};
#endif