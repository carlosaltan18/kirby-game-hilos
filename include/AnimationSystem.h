#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H
#include <string>

// Genera pequenas variaciones de sprites ASCII usando un contador de frames.
// Renderer lo actualiza cada vez que dibuja.
class AnimationSystem {
private:
    int frameCounter;
public:
    AnimationSystem();
    void update();
    std::string getEnemySprite(bool isActive);
    std::string getItemSprite();
};

#endif
