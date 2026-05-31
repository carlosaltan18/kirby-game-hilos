#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H
#include <string>

// Genera pequenas variaciones de sprites ASCII usando un contador de frames.
// Renderer lo actualiza cada vez que dibuja.
class AnimationSystem {
private:
    int frameCounter;
public:
    // Arranca el contador interno de frames.
    AnimationSystem();

    // Avanza un frame de animacion.
    void update();

    // Devuelve el sprite del enemigo segun el frame actual.
    std::string getEnemySprite(bool isActive);

    // Devuelve el sprite alternante usado por items.
    std::string getItemSprite();
};

#endif
