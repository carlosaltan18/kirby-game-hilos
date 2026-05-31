#ifndef TIMER_H
#define TIMER_H
#include <chrono>

class Timer {
private:
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    bool isRunning;
public:
    // Crea el temporizador detenido.
    Timer();

    // Empieza a medir desde el momento actual.
    void start();

    // Detiene la medicion.
    void stop();

    // Devuelve segundos transcurridos desde start.
    int getElapsedSeconds();
};
#endif
